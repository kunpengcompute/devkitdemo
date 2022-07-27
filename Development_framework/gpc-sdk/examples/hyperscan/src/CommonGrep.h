/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: CommonGrep.h
 * Create: 2022-07-26
 */

#ifndef COMMON_GREP_H
#define COMMON_GREP_H

#include <string>
#include <vector>

class CommonGrep {
public:
    static int EventHandler(unsigned int id, unsigned long long from,
        unsigned long long to, unsigned int flags, void *ctx);
    static char *ReadInputData(const std::string &filename, unsigned int *length);
    static void ParseFile(const char *filename, std::vector<std::string> &patterns,
        std::vector<unsigned> &flags, std::vector<unsigned> &ids);
    static unsigned ParseFlags(const std::string &flagsStr);
};

#endif