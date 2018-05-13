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

#include <cstdint>


class Buffer
{
public:
	Buffer(size_t size);
	~Buffer();

	inline size_t GetSize() const { return m_size; }
	inline const uint8_t* GetData() const { return m_data; }
	inline uint8_t* GetData() { return m_data; }

	// non-copyable
	Buffer(const Buffer& other) = delete;
	Buffer& operator=(const Buffer& other) = delete;

	// movable
	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);


private:
	size_t m_size;
	uint8_t* m_data;
};

