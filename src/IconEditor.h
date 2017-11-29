/*
 *      Copyright 2017 Riccardo Musso
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      File src/IconEditor.h
 *
*/

#ifndef _SM2EXE__SRC__ICONEDITOR_H
#define _SM2EXE__SRC__ICONEDITOR_H

#include <windows.h>
#include <vector>

namespace sm2exe {
    class IconEditor {
    private:
        std::vector<WORD> icon_ids;
        std::vector<unsigned char> output;

        const char* bytecode;
        HGLOBAL exe_handle;
        size_t icon_count = 0;


        bool extract_word(HANDLE handle, unsigned char*& out);
        
    public:
        IconEditor(HGLOBAL handle, const char* bytecode);
        
        bool push_icon(const char* icon);
        bool apply();

        static constexpr unsigned WSize = sizeof(WORD), 
                         BlockSize = sizeof(BYTE)*4 + WSize*2 + sizeof(DWORD)*2;
    };
}

#endif
