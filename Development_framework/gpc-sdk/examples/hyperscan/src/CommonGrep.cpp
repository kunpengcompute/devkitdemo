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
 * Description: CommonGrep.cpp
 * Create: 2022-07-26
 */

#include "CommonGrep.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <hs.h>


using namespace std;

/**
This is the function that will be called for each match that occurs.
 */
int CommonGrep::EventHandler(unsigned int id, unsigned long long from,
    unsigned long long to, unsigned int flags, void *ctx)
{
    std::cout << "Match for pattern \"" << static_cast<char *>(ctx) << "\" at offset " << to << std::endl;
    return 0;
}

/**
 * Fill a data buffer from the given filename, returning it and filling @a
 * length with its length. Returns NULL on failure.
 */
char *CommonGrep::ReadInputData(const std::string &filename, unsigned int *length)
{
    std::ifstream isFile(filename, std::ifstream::in);
    if (!isFile.is_open()) {
        isFile.close();
        return nullptr;
    }

    isFile.seekg(0, isFile.end);
    *length = isFile.tellg();
    isFile.seekg(0, isFile.beg);

    // allocate memory
    char *buffer = new char [*length];

    // read data as a block
    if (!isFile.read(buffer, *length)) {
        isFile.close();
        delete[] buffer;
        return nullptr;
    }

    isFile.close();
    return buffer;
}

void CommonGrep::ParseFile(const char *filename, vector<string> &patterns,
    vector<unsigned> &flags, vector<unsigned> &ids)
{
    std::ifstream inFile(filename);
    if (!inFile.good()) {
        cerr << "ERROR: Can't open pattern file \"" << filename << "\"" << endl;
        exit(-1);
    }

    for (unsigned i = 1; !inFile.eof(); ++i) {
        string line;
        getline(inFile, line);

        // if line is empty, or a comment, we can skip it
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // otherwise, it should be ID:PCRE, e.g.
        //  10001:/foobar/is

        size_t colonIdx = line.find_first_of(':');
        if (colonIdx == string::npos) {
            cerr << "ERROR: Could not parse line " << i << endl;
            exit(-1);
        }

        // we should have an unsigned int as an ID, before the colon
        unsigned id = std::stoi(line.substr(0, colonIdx).c_str());

        // rest of the expression is the PCRE
        const string expr(line.substr(colonIdx + 1));

        size_t flagsStart = expr.find_last_of('/');
        if (flagsStart == string::npos) {
            cerr << "ERROR: no trailing '/' char" << endl;
            exit(-1);
        }

        string pcre(expr.substr(1, flagsStart - 1));
        string flagsStr(expr.substr(flagsStart + 1, expr.size() - flagsStart));
        unsigned flag = ParseFlags(flagsStr);

        patterns.push_back(pcre);
        flags.push_back(flag);
        ids.push_back(id);
    }
}

unsigned CommonGrep::ParseFlags(const string &flagsStr)
{
    unsigned flags = 0;
    for (const auto &c : flagsStr) {
        switch (c) {
            case 'i':
                flags |= HS_FLAG_CASELESS; break;
            case 'm':
                flags |= HS_FLAG_MULTILINE; break;
            case 's':
                flags |= HS_FLAG_DOTALL; break;
            case 'H':
                flags |= HS_FLAG_SINGLEMATCH; break;
            case 'V':
                flags |= HS_FLAG_ALLOWEMPTY; break;
            case '8':
                flags |= HS_FLAG_UTF8; break;
            case 'W':
                flags |= HS_FLAG_UCP; break;
            case '\r': // stray carriage-return
                break;
            default:
                cerr << "Unsupported flag \'" << c << "\'" << endl;
                exit(-1);
        }
    }
    return flags;
}