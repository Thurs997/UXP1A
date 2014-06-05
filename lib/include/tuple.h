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
	void copyBinaryContent(byte * tuple);
	int copyBinaryField(byte * tuple, ParameterBase * field, int size);
	int copyStringField(byte * tuple, std::string stringField, int size);
	void copyIntField(byte * tuple, int intField, int size);
	void copyFloatField(byte * tuple, float floatField, int size);
};

#endif /* TUPLE_H_ */
