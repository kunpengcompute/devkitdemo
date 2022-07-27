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
 * Description: ZSTD demo
 * Create: 2022-07-24
 */
#include <iostream>
#include "command.h"
#include <chrono>

enum tpye {
    compress = 1,
    decompress = 2,
    multipleCompress = 3,
    multipleDecompress = 4,
    multipleDictionaryCompress = 5
};

namespace {

void ErrorArg(const char *str) {
    std::cout << "wrong arguments" << std::endl
                     << "usage:" << std::endl
                     << str << std::endl;
    return;
}

}

int main(int argc, const char **argv) {
    unsigned mode;
    std::cin >> mode;
    switch (mode) {
        case compress: {
            if (argc != 2) {
                ErrorArg(argv[0]);
                return 1;
            }
            const char *const inFilename = argv[1];
            TestZSTD test(argc, argv);
            std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
            test.Compress();
            std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> diff = endTime - startTime;
            double delayTime = diff.count(); // uint is second
            std::cout << "Compress using time: " << delayTime << std::endl; 
        } break;
        case decompress: {
            if (argc != 2) {
                ErrorArg(argv[0]);
                return 1;
            }
            TestZSTD test(argc, argv);
            std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
            test.Decompress();
            std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> diff = endTime - startTime;
            double delayTime = diff.count(); // uint is second
            std::cout << "Decompress using time: " << delayTime << std::endl; 
            std::cout << argv[1] << " correctly decoded (in memory)." << std::endl;
        } break;
        case multipleCompress: {
            if (argc < 2) {
                ErrorArg(argv[0]);
                return 1;
            }
            TestZSTD test(argc, argv);
            test.MultipleCompress();
            std::cout << "compressed" << argc - 1 << " files" << std::endl;
        } break;
        case multipleDecompress: {
            if (argc < 2) {
                ErrorArg(argv[0]);
                return 1;
            }
            TestZSTD test(argc, argv);
            test.MultipleDecompress();
            std::cout << "decompressed" << argc - 1 << " files" << std::endl;
        } break;
        case multipleDictionaryCompress: {
            if (argc < 3) {
                ErrorArg(argv[0]);
                return 1;
            }
            TestZSTD test(argc, argv);
            test.MultipleDictionaryCompress();
            std::cout << "compressed" << argc - 2 << " files" << std::endl;
        } break;
        default: 
            break;
    }
    return 0;
}
