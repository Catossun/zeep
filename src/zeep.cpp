#include <iostream>
#include <fstream>
#include <string>
#include <huffman.h>
#include <compressor.h>
#include <arithmetic.h>

#define THROW_ILLEGAL_ARGUMENTS_ERROR \
    printf("ERROR: Illegal arguments!");     \
    exit(1);

using namespace std;

enum class CompressorType {
    HUFFMAN,
    ARITHMETIC
};

string readFromFile(const string &path) {
    ifstream ifs;
    ifs.open(path);
    stringstream str;
    char c;
    while (ifs.get(c)) {
        str << c;
    }
    ifs.close();
    return str.str();
}

void writeToFile(const string &path, const string &content) {
    ofstream ofs;
    ofs.open(path);
    ofs.write(content.c_str(), content.size());
    ofs.close();
}

void printHelpMessage() {
    printf(R"(Normal Options:
    -h, --help                  Show this help message.
    -c, --compress <type>       Compress the input file.
    -d, --decompress <type>     Decompress the input file.

Compress type:
    --huffman <tree_file>       Use Huffman Coding.
    --arithmetic <prob_file>    Use Arithmetic Coding.
)");
}

int main(int argsCount, char **args) {
    auto selectedMode = string(args[1]);
    if (argsCount == 2 && "-h" == selectedMode) {
        printHelpMessage();
        exit(0);
    }
    if (argsCount < 5) {
        THROW_ILLEGAL_ARGUMENTS_ERROR
    }
    bool isDoCompress = false;
    auto compressType = CompressorType::HUFFMAN;
    Compressor *cmp = nullptr;
    if ("-c" == selectedMode) {
        isDoCompress = true;
    } else if ("-d" == selectedMode) {
        isDoCompress = false;
    }
    auto selectedCompressorType = string(args[2]);
    auto compressorRequiredFilePath = string(args[3]);
    if ("--huffman" == selectedCompressorType) {
        compressType = CompressorType::HUFFMAN;
        auto *hf = new Huffman();
        if (!isDoCompress) {
            ifstream ifs(compressorRequiredFilePath);
            hf->readTree(ifs);
            ifs.close();
        }
        cmp = hf;
    } else if ("--arithmetic" == selectedCompressorType) {
        compressType = CompressorType::ARITHMETIC;
        auto *arith = new Arithmetic();
        if (!isDoCompress) {
            ifstream ifs(compressorRequiredFilePath);
            // TODO: Set probability to compressor.
            ifs.close();
        }
        cmp = arith;
    }
    string sourceFileContent = readFromFile(args[4]);
    auto destinationFilePath = string(args[5]);
    if (isDoCompress) {
        string src = sourceFileContent;
        string compressedData = cmp->compress(src);
        if (compressType == CompressorType::HUFFMAN) {
            auto *hf = (Huffman *) cmp;
            ofstream ofs(args[3]);
            hf->writeTree(ofs);
            ofs.close();
        } else if (compressType == CompressorType::ARITHMETIC) {
            auto *arith = (Arithmetic *) cmp;
            ofstream ofs(args[3]);
            // TODO: Save probability.
            ofs.close();
        }
        writeToFile(destinationFilePath, compressedData);
        // Print info
        printf("Input file size: %d bytes\n", src.size());
        printf("Output file size: %f bytes\n", compressedData.size() / 8.0);
        printf(
                "Compress rate: %f%%\n",
                compressedData.size() / 8.0 * 100.0 / src.size()
        );
    } else {
        string src = sourceFileContent;
        string decompressedData = cmp->decompress(src);
        writeToFile(destinationFilePath, decompressedData);
        // Print info
        printf("Input file size: %f bytes\n", src.size() / 8.0);
        printf("Output file size: %d bytes\n", decompressedData.size());
    }
    delete cmp;
    return 0;
}