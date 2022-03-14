#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <huffman.h>
#include <compressor.h>

#define THROW_ILLEGAL_ARGUMENTS_ERROR \
    printf("Illegal arguments!");     \
    exit(1);

using namespace std;

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

int main(int argsCount, char **args) {
    if (argsCount < 5) {
        THROW_ILLEGAL_ARGUMENTS_ERROR
    }
    bool isDoCompress = false;
    bool isAdaptiveHuffman = false;
    Compressor *cmp = nullptr;
    if ("-c" == string(args[1])) {
        isDoCompress = true;
    } else if ("-d" == string(args[1])) {
        isDoCompress = false;
    }
    if ("--huffman" == string(args[2])) {
        isAdaptiveHuffman = false;
        auto *hf = new Huffman();
        string treeFile = string(args[3]);
        if (!isDoCompress) {
            ifstream ifs(treeFile);
            hf->readTree(ifs);
            ifs.close();
        }
        cmp = hf;
    } else if ("--huffman-adaptive" == string(args[2])) {
        isAdaptiveHuffman = true;
        // TODO: Create an instance of HuffmanAdaptive and assign it to cmp.
    }
    if (isDoCompress) {
        string src = readFromFile(args[isAdaptiveHuffman ? 3 : 4]);
        string compressedData = cmp->compress(src);
        if (!isAdaptiveHuffman) {
            auto *hf = (Huffman *) cmp;
            ofstream ofs(args[3]);
            hf->writeTree(ofs);
            ofs.close();
        }
        writeToFile(args[isAdaptiveHuffman ? 4 : 5], compressedData);

    } else {
        string src = readFromFile(args[isAdaptiveHuffman ? 3 : 4]);
        string decompressedData = cmp->decompress(src);
        writeToFile(args[isAdaptiveHuffman ? 4 : 5], decompressedData);
    }
    delete cmp;
    return 0;
}