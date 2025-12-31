#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <errno.h>

using namespace std;

pair<char*, size_t> map_file_to_buffer(const string& filename, int& fd) {
    fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        cerr << "Error opening file! errno: " << errno << " (" << strerror(errno) << ")" << endl;
        return {nullptr, 0};
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        cerr << "Error getting file stats! errno: " << errno << " (" << strerror(errno) << ")" << endl;
        close(fd);
        return {nullptr, 0};
    }
    size_t file_size = sb.st_size;
    if (file_size == 0) {
        cerr << "File is empty!" << endl;
        close(fd);
        return {nullptr, 0};
    }
    void* addr = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        cerr << "Error mapping file! errno: " << errno << " (" << strerror(errno) << ")" << endl;
        close(fd);
        return {nullptr, 0};
    }

    return {static_cast<char*>(addr), file_size};
}

void process_buffer(char* buffer, size_t file_size) {
    for (size_t i = 0; i < file_size; ++i) {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c >= 97 && c <= 122) {
            buffer[i] = static_cast<char>((c - 32) % 256);
        }
    }
}

int main(int argc, const char *argv[]) {
    string filename = argv[1];
    int fd;
    clock_t start = clock();
    auto [buffer, file_size] = map_file_to_buffer(filename, fd);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "读取数据时间: " << time_spent << " 秒" << std::endl;
    clock_t start1 = clock();
    if (buffer && file_size > 0) {
        process_buffer(buffer, file_size);
        munmap(buffer, file_size);
        close(fd);
    }
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    std::cout << "处理数据时间: " << time_spent1 << " 秒" << std::endl;
    return 0;
}
