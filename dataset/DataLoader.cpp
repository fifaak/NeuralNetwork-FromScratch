#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

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


std::vector<int> load_img(const std::string &trainImagesPath,const std::string &trainLabelsPath, int &labelCount,int &imageCount, int &rows, int &cols,int id){
        auto images = readImages(trainImagesPath, imageCount, rows, cols);
        auto labels = readLabels(trainLabelsPath, labelCount);
        std::vector<int> output;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                output.push_back(images[id][r * cols + c]);
            }
        }
        return output;
}

int main() {
    std::string trainImagesPath = "./MnistData/train-images.idx3-ubyte";
    std::string trainLabelsPath = "./MnistData/train-labels.idx1-ubyte";
    int labelCount,imageCount,rows,cols,id;
    id = 0;
    std::vector<int> output = load_img(trainImagesPath,trainLabelsPath,labelCount,imageCount,rows,cols,id);
    for (auto i : output) std::cout << i << " ";

    return 0;
}