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

#include "ArchiveExtractCallbackAdaptor.h"

#include "ArchivePropertyHelper.h"



ArchiveExtractCallbackAdaptor::ArchiveExtractCallbackAdaptor(const ArchiveExtractParam& param)
	: m_passwd(param.passwd)
	, m_archive(param.archive)
	, m_resultStorage(param.resultStorage)
	, m_outMemStream(nullptr)
	, m_outFileStream()
	, m_enableSecrecy(param.enableSecrecy)
{
}

ArchiveExtractCallbackAdaptor::~ArchiveExtractCallbackAdaptor()
{
}


STDMETHODIMP ArchiveExtractCallbackAdaptor::SetTotal(UInt64 /*size*/)
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallbackAdaptor::SetCompleted(const UInt64* /*completeValue*/)
{
	return S_OK;
}


STDMETHODIMP ArchiveExtractCallbackAdaptor::GetStream(UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode)
{
	*outStream = nullptr;
	m_outFileStream.Release();

	auto fileSize = ArchivePropertyHelper::GetFileSize(m_archive, index);
	if (askExtractMode != NArchive::NExtract::NAskMode::kExtract)
		return S_OK;

	auto fileName = ArchivePropertyHelper::GetFileName(m_archive, index);
	auto newBuffer = std::make_shared<Buffer>(fileSize, m_enableSecrecy);
	m_resultStorage->emplace_back(fileName, newBuffer);

	m_outMemStream = new COutMemStream(newBuffer);
	CMyComPtr<ISequentialOutStream> outStreamLoc(m_outMemStream);
	m_outFileStream = outStreamLoc;
	*outStream = outStreamLoc.Detach();

	return S_OK;
}

STDMETHODIMP ArchiveExtractCallbackAdaptor::PrepareOperation(Int32 /*askExtractMode*/)
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallbackAdaptor::SetOperationResult(Int32 /*operationResult*/)
{
	m_outFileStream.Release();
	return S_OK;
}


STDMETHODIMP ArchiveExtractCallbackAdaptor::CryptoGetTextPassword(BSTR *password)
{
	const std::wstring& passwd = m_passwd;
	return StringToBstr(passwd.c_str(), password);
}

