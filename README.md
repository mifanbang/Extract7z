# Extract7z
In-memory decompression of *.7z archives with a single function call. I am too stupid to use it the COM way. Windows only.

### Prerequisites

1. The DLL file `7z.dll` that comes along with your 7-Zip installation (typically in the folder `C:\Program Files\7-Zip\`).
2. The latest package of "7-Zip Source Code" or "LZMA SDK" in [this page](https://www.7-zip.org/download.html).

### Building the Code

1. Download a source code package mentioned in the prerequisite above. Extract its contents into the folder `external/7zip/` so that you have the following path: `external/7zip/CPP/`.
2. Build with Visual Studio (better use the latest version) and find your static library `extract7z.lib` inside the proper subfolder under `bin/`.

### Using the Library

1. Add folder `include/` to the include paths of your code.
2. Add file `bin/$(Platform)/$(Configuration)/extract7z.lib` to the library list of your code.
3. `#include <Extractor7Z.h>` and make a call to the function `Extractor7Z::ExtractFrom()`. That's all.

Please note that the target machine, i.e., either x64 or x86, must be identical to the one of the 7-Zip DLL file you chose. Also, extract7z by default links dynamically to the C runtime.

### Important API

Most classes and functions should be self-explanatory enough so I don't want to waste your time. What listed below are those I expect to be crucial.

#### Class Extractor7Z

- **static bool CheckLibrary()**<br>
As 7z.dll may be missing during runtime, this function checks if the required DLL exists and functions properly.

- **static std::shared_ptr&lt;FileArchive&gt; ExtractFrom(const std::wstring& path, Password& passwd)**<br>
If an error occurred during decompression/decryption, the returned shared pointer will be empty, i.e., casting it to type `bool` yields `false`. As for explanation and usage of the class `Password`, please read on to the next subsection.

#### Class Password

This is an instantiation of the class template `LazyInit`. It accepts a `PasswordCallback`-compatible callback function as the parameter in its constructor. The callback will only be invoked should the extractor need a key, or password, to decrypt an encrypted archive. For archives with no encryption applied, the callback will never be invoked.

#### Class PasswordCallback

An alias of `std::function<bool(std::wstring&)>`. The callback function accepts a reference to a `std::wstring` object as both its input and output. The callback can bring up some user interface to ask for the password and then writes back to the string. If the same output string is supposed to be used for all subsequent decryption, return `true`; otherwise, returning `false` hints the extractor to invoke the callback again next time a password is needed.

### Sample Code

    Password password( [](std::wstring& outPasswd) -> bool {
        wchar_t buf[32];
        fgetws(buf, sizeof(buf) / sizeof(buf[0]), stdin);
        outPasswd = buf;  // including '\n' in the end
        return true;  // never ask again
    } );
    auto archive = Extractor7Z::ExtractFrom(L"sample.7z", password);
    if (!archive) {
        puts("Failed to decompress the archive.");
        return;
    }
    // do something with archive

### Copyright

Copyright (C) 2018 Mifan Bang <https://debug.tw>.

This library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

