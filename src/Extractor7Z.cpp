/*
 *  This file is part of the library Extract7Z.
 *  Copyright (C) 2018 Mifan Bang <https://debug.tw>
 *
 *  This library is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Extractor7Z.h"

#include <Common/Common.h>
#include <Common/MyInitGuid.h>
#include <Windows/DLL.h>
#include <7zip/Archive/IArchive.h>
#include <7zip/Common/FileStreams.h>

#include "ArchiveExtractCallbackAdaptor.h"
#include "ArchiveOpenCallbackAdaptor.h"
#include "ArchivePropertyHelper.h"



DEFINE_GUID(CLSID_CFormat7z, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);

HINSTANCE g_hInstance = 0;



namespace {


class Archive7Z
{
public:
	Archive7Z(const std::wstring& path, std::shared_ptr<FileArchive>& storage)
		: m_path(path)
		, m_archive()
		, m_storage(storage)
		, m_enableSecrecy(false)
	{
		CreateArchiveObj();
	}

	void SetSecrecy(bool enabled)
	{
		m_enableSecrecy = enabled;
	}

	bool Open(Password& passwd)
	{
		if (!IsValid())
			return false;

		CInFileStream *fileSpec = new CInFileStream;
		CMyComPtr<IInStream> file = fileSpec;
		if (!fileSpec->Open(m_path.c_str()))
			return false;

		ArchiveOpenCallbackAdaptor* openCallbackObj = new ArchiveOpenCallbackAdaptor(passwd);
		CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackObj);

		constexpr UInt64 scanSize = 1 << 23;
		if (m_archive->Open(file, &scanSize, openCallback) != S_OK)
			return false;

		return true;
	}

	bool Extract(Password& passwd)
	{
		if (!IsValid())
			return false;

		ArchiveExtractParam param = { passwd, m_archive, m_storage.get(), m_enableSecrecy };
		ArchiveExtractCallbackAdaptor* extractCallbackSpec = new ArchiveExtractCallbackAdaptor(param);

		CMyComPtr<IArchiveExtractCallback> extractCallback(extractCallbackSpec);
		if (m_archive->Extract(nullptr, -1, false, extractCallback) != S_OK)
			return false;

		return true;
	}

	size_t GetUncompressedSize()
	{
		if (!IsValid())
			return 0;

		size_t totalSize = 0;
		uint32_t numItems;
		m_archive->GetNumberOfItems(&numItems);
		for (uint32_t i = 0; i < numItems; ++i)
			totalSize += ArchivePropertyHelper::GetFileSize(m_archive, i);
		return totalSize;
	}

	bool IsValid()
	{
		return s_lib.IsLoaded() && m_archive != nullptr;
	}


private:
	bool CreateArchiveObj() {
		if (!s_lib.IsLoaded() && !s_lib.Load(NWindows::NDLL::GetModuleDirPrefix() + L"7z.dll"))
			return false;

		auto createObjectFunc = reinterpret_cast<Func_CreateObject>(s_lib.GetProc("CreateObject"));
		if (createObjectFunc == nullptr)
			return false;

		if (createObjectFunc(&CLSID_CFormat7z, &IID_IInArchive, reinterpret_cast<void **>(&m_archive)) != S_OK)
			return false;

		return true;
	}


	static NWindows::NDLL::CLibrary s_lib;

	std::wstring m_path;
	CMyComPtr<IInArchive> m_archive;
	std::shared_ptr<FileArchive>& m_storage;
	bool m_enableSecrecy;
};

NWindows::NDLL::CLibrary Archive7Z::s_lib;


bool EmptyPasswordFunc(std::wstring& out)
{
	out.clear();
	return true;
}


}  // unnamed namespace



bool Extractor7Z::CheckLibrary()
{
	auto bufferedFiles = std::make_shared<FileArchive>();
	Archive7Z archive(L"", bufferedFiles);
	return archive.IsValid();
}


bool Extractor7Z::GetUncompressedSize(const std::wstring& path, Password* password, size_t& out)
{
	Password myPasswd(EmptyPasswordFunc);
	if (password == nullptr)
		password = &myPasswd;

	auto bufferedFiles = std::make_shared<FileArchive>();
	Archive7Z archive(path, bufferedFiles);

	if (!archive.Open(*password))
		return false;
	out = archive.GetUncompressedSize();
	return true;
}


std::shared_ptr<FileArchive> Extractor7Z::ExtractFrom(const std::wstring& path, ExtractOptions& options)
{
	ExtractOptions optCopy = options;

	// use our Password object if none is passed in
	Password myPasswd(EmptyPasswordFunc);
	if (optCopy.passwd == nullptr)
		optCopy.passwd = &myPasswd;

	auto bufferedFiles = std::make_shared<FileArchive>();
	Archive7Z archive(path, bufferedFiles);
	archive.SetSecrecy(optCopy.isSecrecy);

	bool noError = true;
	noError = noError && archive.Open(*optCopy.passwd);
	noError = noError && archive.Extract(*optCopy.passwd);
	return noError ? bufferedFiles : nullptr;
}

