#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void createRandomFile(const std::string &filename, std::size_t size) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to create file" << std::endl;
        return;
    }

    std::srand(std::time(nullptr)); 

    for (std::size_t i = 0; i < size; ++i) {
        char randomChar = static_cast<char>(std::rand() % 256); 
        file.put(randomChar);
    }

    file.close();
}

int main(int argc, char **argv) {
    std::string filename;
    std::size_t size;

    if (argc < 3) {
        std::cout << "Usage: ./xxx filePath size(Bytes)\n";
        return 0;
    }
    
    filename = argv[1];
    size = std::atol(argv[2]);
    std::cout << filename << " " << size << std::endl;
    createRandomFile(filename, size);

    return 0;
}
