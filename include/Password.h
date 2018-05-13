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

#include <functional>
#include <string>



template <typename T, typename F>
class LazyInit
{
public:
	LazyInit(const F& func)
		: m_shouldInit(true)
		, m_func(func)
		, m_data()
	{ }

	operator const T&()
	{
		if (m_shouldInit)
			m_shouldInit = !m_func(m_data);
		return m_data;
	}
	operator T&()
	{
		return const_cast<T&>(reinterpret_cast<const LazyInit<T, F>*>(this)->operator const T&());
	}


private:
	bool m_shouldInit;
	F m_func;
	T m_data;
};


using PasswordCallback = std::function<bool(std::wstring&)>;
using Password = LazyInit<std::wstring, PasswordCallback>;

