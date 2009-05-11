//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2008, Daniel �nnerby
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <core/config.h>

namespace musik{ namespace core{

    /*****************************
    Path to where the executable is located.
    *****************************/
    utfstring GetApplicationDirectory();

    /*****************************
    Path to where the executable is located.
    *****************************/
    utfstring GetDataDirectory();

    /*****************************
    Get the full path of the sFile
    *****************************/
    utfstring GetPath(const utfstring &sFile);

    /*****************************
    Path to where the webfiles are located.
    *****************************/
    utfstring GetWebfilesDirectory();

    /*****************************
    Path to where plugins are located.
    *****************************/
    utfstring GetPluginDirectory();

    std::string ConvertUTF8(const std::wstring &sString);
    std::wstring ConvertUTF16(const std::string &sString);
    std::wstring ConvertUTF16(const char *string);

    UINT64 Checksum(char *data,unsigned int bytes);

} }

// UTF Conversion MACROS
#ifdef UTF_WIDECHAR

#define UTF_TO_UTF8(s)  musik::core::ConvertUTF8(s)
#define UTF_TO_UTF16(s) s
#define UTF8_TO_UTF(s)  musik::core::ConvertUTF16(s)
#define UTF16_TO_UTF(s) s

#else

#define UTF_TO_UTF8(s)  s
#define UTF_TO_UTF16(s) musik::core::ConvertUTF16(s)
#define UTF8_TO_UTF(s)  s
#define UTF16_TO_UTF(s) musik::core::ConvertUTF8(s)

#endif

