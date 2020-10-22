#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include "huffmanHelper.h"
#include "dataType.h"

void huffmanHelper::encode(const std::string &input, const std::string &output) {
    std::map<byte, int> freqs;
    std::set<byte> byteSet;

    rawBytes.clear();
    table.clear();
    tree.clear();

#ifdef COUT_IN_CONSOLE
    double progress = 0;
    double proc = 0;
    std::cout<<"Starting reading file..."<<std::endl;
#endif // COUT_IN_CONSOLE

    readFromFile(input.c_str());
    for (auto &i: rawBytes) {
        if (byteSet.find(i) == byteSet.end()) {
            byteSet.insert(i);
            freqs[i] = 1;
        } else {
            freqs[i] += 1;
        }
    }
    for (auto &i: byteSet) {
        ByteInfo b = ByteInfo(i, freqs[i]);
        table.push_back(b);
    }
#ifdef COUT_IN_CONSOLE
    std::cout<<"Starting coding..."<<std::endl;
#endif
    
    tree.encode(table);

    fout.open(output, std::ios::out | std::ios::binary);
    if (!fout.is_open()) return;

    std::map<byte, std::string> dict;

    char cnt = 0;
    char buffer = 0;

    size_t dict_cnt = table.size();
    fout.write((char*)&dict_cnt, sizeof(dict_cnt));

#ifdef COUT_IN_CONSOLE
    progress = 0;
    proc = 0;
    std::cout<<"Starting writing into file..."<<std::endl;
    std::cout<<"Progress: "<<proc<<std::endl;
#endif

    for (auto &i: table) {
        dict[i.value] = i.word;
		fout.write((char*)&i.value, sizeof(i.value));
		char tmp = i.word.length();
		fout.write((char*)&tmp, sizeof(tmp));

		for (auto &j : i.word)
		{
			if (cnt < 8)
			{
				buffer = (buffer << 1) + (j - '0');
				cnt++;
			}
			if (cnt == 8)
			{
				fout.write((char*)&buffer, sizeof(buffer));
				cnt = 0;
				buffer = 0;
			}
		}
		if (cnt != 0)
		{
			buffer = buffer << (8 - cnt);
			fout.write((char*)&buffer, sizeof(buffer));
			cnt = 0;
			buffer = 0;
		}
    }
    cnt = 0;
	buffer = 0;
	
	int resBit = 0;
	for (auto &i : rawBytes)resBit += dict[i].length();
	resBit %= 8;

	fout.write((char*)&resBit, sizeof(resBit));

    for (auto &i: rawBytes)
    {
#ifdef COUT_IN_CONSOLE
		progress++;
		if (progress / rawBytes.size() > proc + 0.01)
		{
			proc = progress / rawBytes.size();
			std::cout << "Progress:" << proc << std::endl;
		}
#endif
		for (auto &j : dict[i])
		{
			if (cnt < 8)
			{
				buffer = (buffer << 1) + (j - '0');
				cnt++;
			}
			if (cnt == 8)
			{
				fout.write((char*)&buffer, sizeof(buffer));
				cnt = 0;
				buffer = 0;
			}
		}
    }
    if (cnt != 0)
	{
		buffer = buffer << (8 - cnt);
		fout.write((char*)&buffer, sizeof(buffer));
		cnt = 0;
		buffer = 0;
	}
	fin.close();
	fout.close();
#ifdef COUT_IN_CONSOLE
	std::cout << "Finish..." << std::endl;
#endif
}

void huffmanHelper::decode(const std::string& input, const std::string &output) {
	fout.open(output, std::ios::out | std::ios::binary);
	if (!fout.is_open()) return;

	rawBytes.clear();
	table.clear();
	std::map<std::string, byte> dict;
	std::set<std::string> codeSet;

#ifdef COUT_IN_CONSOLE
	double progress = 0;
	double proc = 0;
	std::cout << "Start reading file..." << std::endl;
#endif

	readFromFile(input.c_str());

	size_t dict_cnt = 0;
	auto pos = rawBytes.begin();
	for (int i=0; i<sizeof(dict_cnt); i++) {
		char* ptr = (char*)&dict_cnt;
		*(ptr+i) = *pos;
		pos++;
	}

#ifdef COUT_IN_CONSOLE
	std::cout<<"Start decoding..."<<std::endl;
#endif

	for (size_t i=0; i<dict_cnt; i++) {
		ByteInfo bi;
		bi.value = *pos;
		pos++;
		char length = 0, buffer = 0;
		length = *pos;
		pos++;

		bi.word = "";
		for (int j = 0, cnt = 0; j < length / 8 + 1; j++)
		{
			if (cnt >= length)break;
			buffer = *pos;
			pos++;
			for (int k = 7; k >= 0; k--)
			{
				if (cnt >= length)break;
				bi.word += '0' + ((buffer >> k) & 1);
				cnt++;
			}
		}
		dict[bi.word] = bi.value;
		codeSet.insert(bi.word);
	}

	int resBit;
	for (int i=0; i<sizeof(resBit); i++) {
		char *ptr = (char*)&resBit;
		*(ptr+i) = *pos;
		pos++;
	}
#ifdef COUT_IN_CONSOLE
	std::cout << "Start writing file..." << std::endl;
	std::cout << "Progress:" << 0 << std::endl;
#endif

	std::string s = "";
	int j = 0;
	byte buffer = 0;
	while(pos < rawBytes.end())
	{
#ifdef COUT_IN_CONSOLE
		progress++;
		if (progress / rawBytes.size() > proc + 0.01)
		{
			proc = progress / rawBytes.size();
			std::cout << "Progress:" << proc << std::endl;
		}
#endif
		byte i = *pos;
		int th = ((pos + 1 == rawBytes.end()) && resBit != 0) ? resBit : 8;
		while(j < th)
		{
			s += '0' + ((i >> (7 - j)) & 1);
			if (codeSet.find(s) != codeSet.end())
			{
				buffer = dict[s];
				//cout << (char)buffer;
				fout.write((char*)&buffer, sizeof(buffer));
				s.clear();
			}
			j++;
		}
		if (j >= th)
		{
			pos++;
			j -= th;
		}
	}
	fout.close();
#ifdef COUT_IN_CONSOLE
	std::cout<<"finish"<<std::endl;
#endif
}

size_t huffmanHelper::getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t read_len = ftell(file);
    fseek(file, 0, SEEK_SET);
    return read_len;
}

size_t huffmanHelper::getFileSize(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		return 0;
	}
	size_t size = getFileSize(file);
	fclose(file);
	return size;
}

void huffmanHelper::readFromFile(const char* filePath) {
    rawBytes.clear();

    FILE *file = fopen(filePath, "rb");

    if (file == nullptr) return;

    size_t fileSize = getFileSize(file);
    if (fileSize != 0) {
        rawBytes.resize(fileSize);
        size_t n = fread(&rawBytes[0], 1, fileSize, file);
        if (n != fileSize) rawBytes.resize(n);
    }

    const size_t read_len = 1024;
    char buf[read_len];
    for (;;) {
        size_t n = fread(buf, 1, read_len, file);
        rawBytes.insert(rawBytes.end(), buf, buf+n);
        if (n<read_len) break;
    }
    fclose(file);
}

bool cmpByByte(const ByteInfo &x, const ByteInfo &y)
{
	return x.value < y.value;
}
bool cmpByWord(const ByteInfo &x, const ByteInfo &y)
{
	return x.word < y.word;
}