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

#define NOMINMAX

#include "COutMemStream.h"

#include <algorithm>



COutMemStream::COutMemStream(std::shared_ptr<Buffer>& storage)
	: m_buffer(storage)
	, m_pos(0)
{
}


COutMemStream::~COutMemStream()
{
}


HRESULT COutMemStream::Write(const void* data, UInt32 size, UInt32* processedSize)
{
	size_t freeSpace = m_buffer->GetSize() - m_pos;
	size = std::min(static_cast<UInt32>(freeSpace), size);
	if (processedSize != nullptr)
		*processedSize = size;
	if (size == 0)
		return S_FALSE;

	memcpy(m_buffer->GetData() + m_pos, data, size);
	m_pos += size;

	return S_OK;
}

