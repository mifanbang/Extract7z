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

#include "Buffer.h"



Buffer::Buffer(size_t size)
	: m_size(size)
	, m_data(new uint8_t[size])
{
}

Buffer::~Buffer()
{
	if (m_data != nullptr)
		delete[] m_data;
}

Buffer::Buffer(Buffer&& other)
	: m_size(other.m_size)
	, m_data(other.m_data)
{
	other.m_size = 0;
	other.m_data = nullptr;
}

Buffer& Buffer::operator=(Buffer&& other)
{
	if (m_data != nullptr)
		delete[] m_data;

	m_size = other.m_size;
	m_data = other.m_data;
	other.m_size = 0;
	other.m_data = nullptr;

	return *this;
}

