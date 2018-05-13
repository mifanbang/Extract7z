/*
 *  This file is part of the library Extract7Z.
 *  Copyright (C) 2018 Mifan Bang <https://debug.tw>
 *
 *  This library is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Buffer.h"

#include <cstdint>
#include <memory>

#include <Common/MyCom.h>
#include <7zip/IStream.h>



class COutMemStream : public ISequentialOutStream, public CMyUnknownImp
{
public:
	COutMemStream(std::shared_ptr<Buffer>& storage);
	virtual ~COutMemStream();

	MY_UNKNOWN_IMP1(ISequentialOutStream)
	STDMETHOD(Write)(const void *data, UInt32 size, UInt32 *processedSize) override;

	inline size_t GetSize() const	{ return m_buffer->GetSize(); }
	inline uint32_t GetData() const { return reinterpret_cast<const uint32_t*>(m_buffer->GetData())[0]; }


private:
	std::shared_ptr<Buffer> m_buffer;
	size_t m_pos;
};

