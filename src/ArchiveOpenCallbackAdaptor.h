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

#pragma once

#include "Password.h"

#include <Common/Common.h>
#include <Common/MyCom.h>
#include <7zip/Archive/IArchive.h>
#include <7zip/IPassword.h>



class ArchiveOpenCallbackAdaptor : public IArchiveOpenCallback, public ICryptoGetTextPassword, public CMyUnknownImp
{
public:
	ArchiveOpenCallbackAdaptor(Password& passwd);
	virtual ~ArchiveOpenCallbackAdaptor();

	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

	STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes) override;
	STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes) override;

	STDMETHOD(CryptoGetTextPassword)(BSTR *password) override;


private:
	Password& m_passwd;
};

