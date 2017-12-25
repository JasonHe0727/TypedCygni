#ifndef DECODER_H
#define DECODER_H

#include "Any.h"
#include "Function.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using ConstantPool = std::vector<Any>;

class Decoder
{
public:
	vector<byte> stream;
	ConstantPool pool;
	vector<byte> code;
	vector<Function> functions;
    Decoder(string path);
	Decoder(vector<byte> stream);
	void Decode();
private:
	i32 LoadConstantPool(i32 index);
	i32 LoadFunctions(i32 index);
	i32 LoadOneFunction(i32 index, i32 parametersSize, i32 frameSize);
	i32 ReadUShort(i32 offset);
	i32 ReadInt32(i32 offset);
	i64 ReadInt64(i32 offset);
	f64 ReadFloat64(i32 offset);
};

#endif // DECODER_H 