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

#include "BufferedFile.h"
#include "Password.h"

#include <memory>



class Extractor7Z
{
public:
	struct ExtractOptions
	{
		Password* passwd;
		bool isSecrecy;  // DOES NOT guarantee never being swapped out

		ExtractOptions()
			: passwd(nullptr)
			, isSecrecy(false)
		{ }
	};

	static bool CheckLibrary();
	static std::shared_ptr<FileArchive> ExtractFrom(const std::wstring& path, ExtractOptions& options);
};

