#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>

const char *FILE_NAME = "testfile.dat";

int main(int argc, char **argv) {
    size_t blockSize = 4096;
    size_t repeat = 1000;

    if (argc < 2) {
        std::cout << "Usage: ./xxx filePath\n";
        return 0;
    }
    const char *filePath = argv[1];
    FILE *file = std::fopen(filePath, "rb");
    if (!file) {
        std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
        return 1;
    }

    if (argc > 3) {
        blockSize = std::atoi(argv[2]);
        repeat = std::atoi(argv[3]);
    }

    std::cout << filePath << " blockSize=" << blockSize << " repeat=" << repeat << std::endl;

    std::vector<char> buffer(blockSize);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < repeat; ++i) {
        std::fread(buffer.data(), 1, blockSize, file);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    double total_read_size = static_cast<double>(repeat * blockSize) / (1024 * 1024); 
    double speed = total_read_size / duration.count(); 

    int iops = repeat / duration.count();
    std::cout << "SeqRead IOPS: " << iops << std::endl;
    std::cout << "SeqRead throughput: " << speed << " MB/s" << std::endl;

    std::fclose(file);

    return 0;
}
