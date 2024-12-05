#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

// Function to read MNIST images
std::vector<std::vector<uint8_t>> readImages(const std::string &filePath, int &imageCount, int &rows, int &cols) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    uint32_t magicNumber = 0, numImages = 0, numRows = 0, numCols = 0;
    file.read(reinterpret_cast<char*>(&magicNumber), 4);
    file.read(reinterpret_cast<char*>(&numImages), 4);
    file.read(reinterpret_cast<char*>(&numRows), 4);
    file.read(reinterpret_cast<char*>(&numCols), 4);

    // Convert from big-endian to little-endian
    magicNumber = __builtin_bswap32(magicNumber);
    numImages = __builtin_bswap32(numImages);
    numRows = __builtin_bswap32(numRows);
    numCols = __builtin_bswap32(numCols);

    imageCount = numImages;
    rows = numRows;
    cols = numCols;

    std::vector<std::vector<uint8_t>> images(numImages, std::vector<uint8_t>(numRows * numCols));
    for (int i = 0; i < numImages; ++i) {
        file.read(reinterpret_cast<char*>(images[i].data()), numRows * numCols);
    }

    file.close();
    return images;
}

// Function to read MNIST labels
std::vector<uint8_t> readLabels(const std::string &filePath, int &labelCount) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    uint32_t magicNumber = 0, numLabels = 0;
    file.read(reinterpret_cast<char*>(&magicNumber), 4);
    file.read(reinterpret_cast<char*>(&numLabels), 4);

    // Convert from big-endian to little-endian
    magicNumber = __builtin_bswap32(magicNumber);
    numLabels = __builtin_bswap32(numLabels);

    labelCount = numLabels;

    std::vector<uint8_t> labels(numLabels);
    file.read(reinterpret_cast<char*>(labels.data()), numLabels);

    file.close();
    return labels;
}

int main() {
    try {
        int imageCount, rows, cols;
        int labelCount;

        // Paths to the dataset files
        std::string trainImagesPath = "/mnt/data/train-images.idx3-ubyte";
        std::string trainLabelsPath = "/mnt/data/train-labels.idx1-ubyte";

        // Read images and labels
        auto images = readImages(trainImagesPath, imageCount, rows, cols);
        auto labels = readLabels(trainLabelsPath, labelCount);

        // Print some information about the dataset
        std::cout << "Number of Images: " << imageCount << "\n";
        std::cout << "Image Dimensions: " << rows << "x" << cols << "\n";
        std::cout << "Number of Labels: " << labelCount << "\n";

        // Example: Print the first label and its corresponding image
        std::cout << "First Label: " << static_cast<int>(labels[0]) << "\n";
        std::cout << "First Image:\n";
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                std::cout << (images[0][r * cols + c] > 128 ? "#" : ".") << " ";
            }
            std::cout << "\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}