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

#include "ArchiveOpenCallbackAdaptor.h"



ArchiveOpenCallbackAdaptor::ArchiveOpenCallbackAdaptor(Password& passwd)
	: m_passwd(passwd)
{
}

ArchiveOpenCallbackAdaptor::~ArchiveOpenCallbackAdaptor()
{
}


STDMETHODIMP ArchiveOpenCallbackAdaptor::SetTotal(const UInt64 * /* files */, const UInt64 * /* bytes */)
{
	return S_OK;
}

STDMETHODIMP ArchiveOpenCallbackAdaptor::SetCompleted(const UInt64 * /* files */, const UInt64 * /* bytes */)
{
	return S_OK;
}


STDMETHODIMP ArchiveOpenCallbackAdaptor::CryptoGetTextPassword(BSTR* password)
{
	const std::wstring& passwd = m_passwd;
	return StringToBstr(passwd.c_str(), password);
}
