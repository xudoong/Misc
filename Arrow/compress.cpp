#include <iostream>
#include <arrow/api.h>
#include <arrow/util/compression.h>

// Function to compress a string using the specified compression type
std::string CompressString(const std::string& input, arrow::Compression::type compression_type) {
    std::unique_ptr<arrow::util::Codec> codec;
    auto result = arrow::util::Codec::Create(compression_type);
    if (!result.ok()) {
        throw std::runtime_error("Failed to create codec: " + result.status().ToString());
    }
    codec = std::move(result.ValueUnsafe());

    // Allocate memory for the compressed output
    int64_t max_compressed_len = codec->MaxCompressedLen(input.size(), nullptr);
    std::string compressed;
    compressed.resize(max_compressed_len);

    // Perform the compression
    auto result2 = codec->Compress(input.size(), reinterpret_cast<const uint8_t*>(input.data()),
                                  max_compressed_len, reinterpret_cast<uint8_t*>(&compressed[0]));
    if (!result2.ok()) {
        throw std::runtime_error("Compression failed");
    }
    auto actual_compressed_len = result2.ValueUnsafe();


    // Resize the compressed string to the actual compressed length
    compressed.resize(actual_compressed_len);
    return compressed;
}

// Function to decompress a string using the specified compression type
std::string DecompressString(const std::string& compressed, arrow::Compression::type compression_type, int64_t decompressed_size) {
    std::unique_ptr<arrow::util::Codec> codec;
    auto result = arrow::util::Codec::Create(compression_type);
    if (!result.ok()) {
        throw std::runtime_error("Failed to create codec: " + result.status().ToString());
    }
    codec = std::move(result.ValueUnsafe());

    // Allocate memory for the decompressed output
    std::string decompressed(decompressed_size, '\0');

    // Perform the decompression
    auto result2 = codec->Decompress(compressed.size(), reinterpret_cast<const uint8_t*>(compressed.data()),
                                    decompressed_size, reinterpret_cast<uint8_t*>(&decompressed[0]));
    if (!result2.ok()) {
        throw std::runtime_error("Decompression failed");
    }

    decompressed_size = result2.ValueUnsafe();
    decompressed.resize(decompressed_size);
    return decompressed;
}

int main() {
    std::string original_string = "This is a string that will be compressed and then decompressed using Arrow.";
    arrow::Compression::type compression_type = arrow::Compression::SNAPPY;

    // Compress the string
    std::string compressed_string = CompressString(original_string, compression_type);
    std::cout << "Compressed string size: " << compressed_string.size() << std::endl;

    // Decompress the string
    std::string decompressed_string = DecompressString(compressed_string, compression_type, original_string.size());
    std::cout << "Decompressed string: " << decompressed_string << std::endl;

    // Check if the decompressed string matches the original string
    if (decompressed_string == original_string) {
        std::cout << "Decompression was successful!" << std::endl;
    } else {
        std::cerr << "Decompression failed. The decompressed string does not match the original." << std::endl;
    }

    return 0;
}
