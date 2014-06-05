/*
 * tuple.h
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */
#ifndef TUPLE_H_s
#define TUPLE_H_
#include <string>
#include <vector>
#include "parameter.h"

typedef unsigned char byte;

class Tuple {
public: //private:
	std::vector<ParameterBase*> params;
public:
	Tuple();
	static Tuple * fromBinary(byte binaryArray[]);
	byte * toBinary();
	void addString(std::string arg){
		addArg<std::string>(arg);
	}
	void addInteger(int arg){
		addArg<int>(arg);
	}
	void addFloat(float arg){
		addArg<float>(arg);
	}
private:
	template<typename T>
	void addArg(T arg) {
		params.push_back(new Parameter<T>(arg));
	}
	int addStringFromBinary(byte binaryArray[], int position);
	void addIntFromBinary(byte binaryArray[], int position);
	void addFloatFromBinary(byte binaryArray[], int position);
	unsigned int binaryLength();
	unsigned int oneArgLength(ParameterBase *);
	unsigned int createBinaryMask();
	void copyBinaryContent(std::vector<byte> * tuple);
	void copyBinaryField(std::vector<byte> * tuple, ParameterBase * field);
	void copyStringField(std::vector<byte> * tuple, std::string stringField);
	void copyIntField(std::vector<byte> * tuple, int intField);
	void copyFloatField(std::vector<byte> * tuple, float floatField);
};

#endif /* TUPLE_H_ */
