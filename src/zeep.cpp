#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <huffman.h>
#include <compressor.h>

#define THROW_ILLEGAL_ARGUMENTS_ERROR \
    printf("ERROR: Illegal arguments!");     \
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

void printHelpMessage() {
    printf(R"(Normal Options:
    -h, --help                  Show this help message.
    -c, --compress <type>       Compress the input file.
    -d, --decompress <type>     Decompress the input file.

Compress type:
    --huffman <tree_file>       Use Huffman Coding.
)");
}

int main(int argsCount, char **args) {
    if (argsCount == 2 && "-h" == string(args[1])) {
        printHelpMessage();
        exit(0);
    }
    if (argsCount < 5) {
        THROW_ILLEGAL_ARGUMENTS_ERROR
    }
    bool isDoCompress = false;
    Compressor *cmp = nullptr;
    if ("-c" == string(args[1])) {
        isDoCompress = true;
    } else if ("-d" == string(args[1])) {
        isDoCompress = false;
    }
    if ("--huffman" == string(args[2])) {
        auto *hf = new Huffman();
        string treeFile = string(args[3]);
        if (!isDoCompress) {
            ifstream ifs(treeFile);
            hf->readTree(ifs);
            ifs.close();
        }
        cmp = hf;
    }
    if (isDoCompress) {
        string src = readFromFile(args[4]);
        string compressedData = cmp->compress(src);
        auto *hf = (Huffman *) cmp;
        ofstream ofs(args[3]);
        hf->writeTree(ofs);
        ofs.close();
        writeToFile(args[5], compressedData);
        // Print info
        printf("Input file size: %d bytes\n", src.size());
        printf("Output file size: %f bytes\n", compressedData.size() / 8.0);
        printf(
                "Compress rate: %f%%\n",
                compressedData.size() / 8.0 * 100.0 / src.size()
        );
    } else {
        string src = readFromFile(args[4]);
        string decompressedData = cmp->decompress(src);
        writeToFile(args[5], decompressedData);
        // Print info
        printf("Input file size: %f bytes\n", src.size() / 8.0);
        printf("Output file size: %d bytes\n", decompressedData.size());
    }
    delete cmp;
    return 0;
}