#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char **argv) {
    size_t blockSize = 4096;
    size_t repeat = 1000;

    if (argc < 2) {
        std::cout << "Usage: ./xxx filePath\n";
        return 0;
    }
    const char *filePath = argv[1];
    FILE *file = std::fopen(filePath, "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 1;
    }

    if (argc > 3) {
        blockSize = std::atoi(argv[2]);
        repeat = std::atoi(argv[3]);
    }

    std::cout << filePath << " blockSize=" << blockSize << " repeat=" << repeat << std::endl;

    std::vector<char> buffer(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = 'a' + (i % 26);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < repeat; ++i) {
        std::fwrite(buffer.data(), 1, blockSize, file);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    double total_size = static_cast<double>(repeat * blockSize) / (1024 * 1024); 
    double speed = total_size / duration.count(); 

    int iops = repeat / duration.count();
    std::cout << "SeqWrite IOPS: " << iops << std::endl;
    std::cout << "SeqWrite throughput: " << speed << " MB/s" << std::endl;

    std::fclose(file);

    return 0;
}
