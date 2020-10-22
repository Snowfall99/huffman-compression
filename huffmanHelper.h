#ifndef HUFFMANHELPER_H
#define HUFFMANHELPER_H

#include <fstream>
#include "DataType.h"
#include "huffman.h"

bool cmpByByte(const ByteInfo &x, const ByteInfo &y);
bool cmpByWord(const ByteInfo &x, const ByteInfo &y);

class huffmanHelper {
private:
    huffmanTree tree;
    std::fstream fout;
    std::fstream fin;
    std::vector<ByteInfo> table;
    std::vector<byte> rawBytes;
    huffmanHelper() {}
    huffmanHelper(const huffmanHelper &);
    size_t getFileSize(FILE* file);
    size_t getFileSize(const char* filePath);
    void readFromFile(const char* filePath);
public:
    static huffmanHelper* getInstance() {
        static huffmanHelper instance;
        return &instance;
    }
    void encode(const std::string &input, const std::string &output);
    void decode(const std::string &input, const std::string &output);
};

#endif