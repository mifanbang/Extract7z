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

#include "ArchivePropertyHelper.h"

#include <Common/Common.h>
#include <7zip/Archive/IArchive.h>
#include <Windows/PropVariant.h>
#include <Windows/PropVariantConv.h>



std::wstring ArchivePropertyHelper::GetFileName(IInArchive* archive, size_t index) {
	NWindows::NCOM::CPropVariant prop;
	bool hasProp = (archive->GetProperty(static_cast<UInt32>(index), kpidPath, &prop) == S_OK);
	return hasProp && prop.vt == VT_BSTR ? prop.bstrVal : L"";
}


size_t ArchivePropertyHelper::GetFileSize(IInArchive* archive, size_t index) {
	NWindows::NCOM::CPropVariant prop;
	if (archive->GetProperty(static_cast<UInt32>(index), kpidSize, &prop) != S_OK)
		return 0;

	UInt64 fileSize;
	ConvertPropVariantToUInt64(prop, fileSize);
	return static_cast<size_t>(fileSize);
}
