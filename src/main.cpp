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
 *      File src/main.cpp
 *
*/

#include "typedefs.h"
#include "IconEditor.h"

#include <iostream>
#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

void printUsage();

int main(int argc, char* argv[]){
    const char      *input_file = nullptr,
                    *exe_input = _SM2EXE_DEFAULT_INPUT,
                    *exe_output = _SM2EXE_DEFAULT_OUTPUT,
                    *exe_temp = _SM2EXE_DEFAULT_TEMP;
    std::vector<const char*> icons;

    for(int i = 1; i != argc; ++i){
        if(argv[i][0] == '-'){
            #define _SM2EXE_OPT(mode1, mode2) \
                (!std::strcmp(argv[i], "-" mode1) || !std::strcmp(argv[i], "--" mode2))

            if(_SM2EXE_OPT("o", "output")){
                if(++i == argc)
                    printUsage();
                exe_output = argv[i];
            } else if(_SM2EXE_OPT("i", "input")){
                if(++i == argc)
                    printUsage();
                exe_input = argv[i];
            } else if(_SM2EXE_OPT("I", "icon")){
                if(++i == argc)
                    printUsage();
                icons.emplace_back(argv[i]);
            } else {
                printUsage();
            }

            #undef _SM2EXE_OPT
        } else {
            input_file = argv[i];
            break;
        }
    }

    if(!input_file)
        printUsage();

    // copy the PE file before
    if(!::CopyFileA(exe_input, exe_temp, FALSE)){
        std::cerr << "fatal error: cannot copy file " << exe_input << " to file " << exe_temp << std::endl;
        return 1;
    }

    HGLOBAL handle = ::BeginUpdateResourceA(exe_temp, TRUE);
    std::string data;

    {
        std::ifstream fs(input_file, std::ios::binary);
        if(!fs){
            std::cerr << "fatal error: cannot load input file " << input_file << std::endl;
            return 1;
        }

        fs.seekg(0, std::ios::end);
        size_t size = fs.tellg();
        fs.seekg(0, std::ios::beg);

        data.resize(size);
        fs.read(&data[0], size);
    }
    
    if(!UpdateResource(handle, RT_RCDATA, MAKEINTRESOURCE(_SM2EXE_CODE_ID), LANG_NEUTRAL, &data[0], data.size())){
        std::cerr << "fatal error: cannot update resource in " << exe_temp << std::endl;
        return 1;
    }

    if(!icons.empty()){
        sm2exe::IconEditor ie(handle, input_file);
        for(auto* icon : icons){
            if(!ie.push_icon(icon)){
                std::cerr << "fatal error: cannot load correctly icon " << icon << std::endl;
                return 1;
            }
        }

        if(!ie.apply()){
            std::cerr << "fatal error: cannot create group icon resource!" << std::endl;
            return 1;
        }
    }

    if(!EndUpdateResource(handle, FALSE)){
        std::cerr << "fatal error: cannot apply changes to " << exe_temp << std::endl;
        return 1;
    }

    if(!::MoveFileExA(exe_temp, exe_output, MOVEFILE_REPLACE_EXISTING)){
        std::cerr << "fatal error: cannot move file " << exe_temp << " to file " << exe_output << std::endl;
    }
    return 0;
}

const char* usage =     "Usage: sm2exe options... [File]\n"
                        "Reads bytecode from [File] and copies it to the generated PE's resources.\n"
                        "The PE file is copied from a pre-compiled one (you can s)\n\n"
                        "Options:\n"
                        "-i, --input <file>          Specify <file> as the PE input file.\n"
                        "-I, --icon <icon>           Add <icon> (.ico image) to the icons of the executable\n"
                        "                               -> You can give more than one icon\n"
                        "-o, --output <file>         Set <file> as the output file name.\n\n"
                        "Program " _SM2EXE_PROGRAM_NAME " is licensed under the Apache License 2.0\n"
                        "Copyright 2017 Riccardo Musso\n"
                    ;

void printUsage(){
    std::cout << usage << std::endl;
    std::exit(1);
}

