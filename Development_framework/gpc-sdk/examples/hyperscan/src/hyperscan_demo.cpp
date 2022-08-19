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
 * Description: hyperscan_demo.cpp
 * Create: 2022-07-26
 */

#include "CommonGrep.h"

#include <iostream>
#include "hyperscan/hs.h"
#include "hyperscan/hs_version.h"

#define ARG_NUM 3

using namespace std;

int main(int argc, char *argv[])
{
    int i = 0;
    if (argc != ARG_NUM) {
        std::cout << "Usage: " << argv[i] << " <pattern> <input file>" << std::endl;
        return -1;
    }

    std::cout << "Hyperscan: " << HS_VERSION_STRING << std::endl;

    char *pattern = argv[++i];
    std::string filename = argv[++i];

    hs_database_t *database;
    hs_compile_error_t *compileErr;

    // First, compile the pattern
    if (hs_compile(pattern, HS_FLAG_DOTALL, HS_MODE_BLOCK, nullptr, &database,
                   &compileErr) != HS_SUCCESS) {
        std::cout << "ERROR: Unable to compile pattern \"" << pattern << "\": " << compileErr->message << std::endl;
        hs_free_compile_error(compileErr);
        return -1;
    }

    // Next, we read the input data file into a buffer.
    unsigned int length;
    char *inputData = CommonGrep::ReadInputData(filename, &length);
    if (!inputData) {
        hs_free_database(database);
        return -1;
    }

    // Finally, we issue a call to hs_scan, which will search the input buffer for the pattern
    hs_scratch_t *scratch = nullptr;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        std::cout << "ERROR: Unable to allocate scratch space. Exiting." << std::endl;
        delete[] inputData;
        hs_free_database(database);
        return -1;
    }

    std::cout << "Scanning " << length << " bytes with Hyperscan" << std::endl;

    if (hs_scan(database, inputData, length, 0, scratch, CommonGrep::EventHandler,
                pattern) != HS_SUCCESS) {
        std::cout << "ERROR: Unable to scan input buffer. Exiting." << std::endl;
        hs_free_scratch(scratch);
        delete[] inputData;
        hs_free_database(database);
        return -1;
    }

    // Scanning is complete, clean up and exit.
    hs_free_scratch(scratch);
    delete[] inputData;
    hs_free_database(database);
    return 0;
}
