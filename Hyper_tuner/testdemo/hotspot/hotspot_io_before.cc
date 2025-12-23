#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cctype>

using namespace std;

// 负责读取文件到缓冲区
vector<char> read_file_to_buffer(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return {};
    }
    // 获取文件大小
    file.seekg(0, ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);
    file.close();

    return buffer;
}

void process_buffer(std::vector<char>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c >= 97 && c <= 122) {
            buffer[i] = static_cast<char>((c - 32) % 256);
        }
    }
}

int main(int argc, const char *argv[]) {
    string filename = argv[1];
    auto buffer = read_file_to_buffer(filename);
    process_buffer(buffer);

    return 0;
}
