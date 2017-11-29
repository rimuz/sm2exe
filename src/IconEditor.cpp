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
 *      File src/IconEditor.cpp
 *
*/

#include "IconEditor.h"
#include "typedefs.h"

#include <iostream>

namespace sm2exe {
    IconEditor::IconEditor(HGLOBAL h, const char* bc) : bytecode(bc), exe_handle(h) {}

    bool IconEditor::extract_word(HANDLE handle,  unsigned char*& ptr){
        DWORD read;

        if(ptr){
            bool res = ::ReadFile(handle, ptr, sizeof(WORD), &read, NULL);
            ptr += sizeof(WORD);
            return res;
        } else {
            WORD dummy;
            return ::ReadFile(handle, &dummy, sizeof(WORD), &read, NULL);
        }
    }

    bool IconEditor::push_icon(const char* path){
        HANDLE handle = ::CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(!handle)
            return false;

        DWORD junk;
        bool res;

        if(!icon_count){
            output.resize(3*WSize);
            auto* ptr = output.data();

            res = extract_word(handle, ptr); // reserved
            res = res && extract_word(handle, ptr); // type
            res = res && extract_word(handle, ptr); // count (ignored)
        } else {
            res = ::SetFilePointer(handle, sizeof(WORD), NULL, FILE_CURRENT);
            res = res && ::SetFilePointer(handle, sizeof(WORD), NULL, FILE_CURRENT);
            res = res && ::SetFilePointer(handle, sizeof(WORD), NULL, FILE_CURRENT);
        }

        if(!res){
            ::CloseHandle(handle);
            return false;
        }
        
        size_t old_buffer_size = output.size();
        output.resize(old_buffer_size + BlockSize);
        auto* work = &output[old_buffer_size];
        
        ::ReadFile(handle, work, BlockSize, &junk, NULL);
        work += BlockSize;

        DWORD* temp = reinterpret_cast<DWORD*>(work) -2; // to read last two DWORDs
        DWORD img_size = *temp++;
        DWORD img_pos = *temp; // no advance! See next instr.
        *temp = _SM2EXE_ICON_START_ID + icon_count; // *temp contains the ID

        auto* icon = new unsigned char[img_size];
        ::SetFilePointer(handle, img_pos, NULL, FILE_BEGIN);
        ::ReadFile(handle, icon, img_size, &junk, NULL);

        res = UpdateResource(exe_handle, RT_ICON, MAKEINTRESOURCE(*temp), LANG_NEUTRAL, icon, img_size);
        if(res) ++icon_count;

        delete [] icon;
        ::CloseHandle(handle);
        return res;
    }

    bool IconEditor::apply(){
        if(!icon_count)
            return false;
        
        reinterpret_cast<WORD&>(output[WSize*2]) = icon_count;
        if(!UpdateResource(exe_handle, RT_GROUP_ICON, MAKEINTRESOURCE(_SM2EXE_ICON_GROUP_ID), LANG_NEUTRAL, output.data(), output.size()))
            return false;

        output.clear();
        return true;
    }

}
