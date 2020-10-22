#pragma once

#ifndef DATATYPE_H
#define DATATYPE_H
#define COUT_IN_CONSOLE
#include <vector>
#include <string>
typedef unsigned char byte;
struct ByteInfo
{
	byte value;
	std::string word;
	int prob;
	ByteInfo(byte c = 0, int p = 0)
	{
		value = c;
		prob = p;
	}
	ByteInfo(byte c, const std::string &w)
	{
		value = c;
		word = w;
	}
};
#endif // !DATATYPE_H
