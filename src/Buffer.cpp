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

#include <windows.h>


namespace {


inline uint8_t* AllocateNormal(size_t size)
{
	return new uint8_t[size]();
}

inline void ReleaseNormal(uint8_t* ptr)
{
	delete[] ptr;
}

inline uint8_t* AllocateProtected(size_t size)
{
	auto newMem = reinterpret_cast<uint8_t*>(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
	if (newMem != nullptr)
		VirtualLock(newMem, size);
	return newMem;
}

inline void ReleaseProtected(uint8_t* ptr, size_t size)
{
	ZeroMemory(ptr, size);
	VirtualFree(ptr, 0, MEM_RELEASE);
}


// hubs
inline uint8_t* Allocate(bool protection, size_t size)
{
	return protection ? AllocateProtected(size) : AllocateNormal(size);
}

inline void Release(bool protection, uint8_t* ptr, size_t size)
{
	if (protection)
		ReleaseProtected(ptr, size);
	else
		ReleaseNormal(ptr);
}


}  // unnamed namespace



Buffer::Buffer(size_t size, bool protection)
	: m_size(size)
	, m_data(nullptr)
	, m_protection(protection)
{
	m_data = Allocate(m_protection, m_size);
}

Buffer::~Buffer()
{
	if (m_data != nullptr)
		Release(m_protection, m_data, m_size);
}

Buffer::Buffer(Buffer&& other)
	: m_size(other.m_size)
	, m_data(other.m_data)
	, m_protection(other.m_protection)
{
	other.m_size = 0;
	other.m_data = nullptr;
}

Buffer& Buffer::operator=(Buffer&& other)
{
	if (m_data != nullptr)
		Release(m_protection, m_data, m_size);

	m_size = other.m_size;
	m_data = other.m_data;
	m_protection = other.m_protection;
	other.m_size = 0;
	other.m_data = nullptr;

	return *this;
}

