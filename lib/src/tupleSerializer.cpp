/*
 * tupleSerializer.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: kokoss
 */

#include "tupleSerializer.h"
#include <iostream>

std::string TupleSerializer::binToString(byte binaryArray[], int & position){
	int i;
	std::string str;
	for(i = position; binaryArray[i] != 0; ++i)
		str += binaryArray[i];
	position = i+1;
	return str;
}

int TupleSerializer::binToInt(byte binaryArray[], int position){
	byte byteArg[] = {	binaryArray[position], binaryArray[position+1],
						binaryArray[position+2], binaryArray[position+3]};
	int const * arg = reinterpret_cast<int const *>(&byteArg);
	return *arg;
}

float TupleSerializer::binToFloat(byte binaryArray[], int position){
	byte byteArg[] = {	binaryArray[position], binaryArray[position+1],
						binaryArray[position+2], binaryArray[position+3]};
	float const * arg = reinterpret_cast<float const *>(&byteArg);
	return *arg;
}

void TupleSerializer::addStringToBin(std::vector<byte> * bin, std::string stringField){
	const char * charField = stringField.c_str();
	for(unsigned int i = 0; i < stringField.size()+1; ++i)
		bin->push_back(charField[i]);
}
void TupleSerializer::addIntToBin(std::vector<byte> * bin, int intField){
	byte const * p = reinterpret_cast<byte const *>(&intField);
	bin->push_back(p[0]);
	bin->push_back(p[1]);
	bin->push_back(p[2]);
	bin->push_back(p[3]);

}
void TupleSerializer::addFloatToBin(std::vector<byte> * bin, float floatField){
	byte const * p = reinterpret_cast<byte const *>(&floatField);
	bin->push_back(p[0]);
	bin->push_back(p[1]);
	bin->push_back(p[2]);
	bin->push_back(p[3]);
}
