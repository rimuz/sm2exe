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
 *      File src/typedefs.h
 *
*/

#ifndef _SM2EXE__SRC__TYPEDEFS_H
#define _SM2EXE__SRC__TYPEDEFS_H

#ifndef _WIN32
#   error Sm2Exe must be compiled (or cross-compiled) for Windows only.
#endif

#define _SM2EXE_DEFAULT_INPUT "latest-console.exe"
#define _SM2EXE_DEFAULT_OUTPUT "smout.exe"
#define _SM2EXE_DEFAULT_TEMP "_temp.exe" 
#define _SM2EXE_PROGRAM_NAME "sm2exe"

#define _SM2EXE_ICON_GROUP_ID 101
#define _SM2EXE_ICON_START_ID 102
#define _SM2EXE_CODE_ID 201 // leaving enough room for lots of icons.

#endif
