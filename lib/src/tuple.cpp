/*
 * tuple.cpp
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */
#include "tuple.h"
#include <string>
#include <cstdlib>
#include <typeinfo>

Tuple::Tuple(){

}

Tuple Tuple::fromBinary(byte binaryArray[]){
	Tuple t;
	return t;
}

byte * Tuple::toBinary(){
	byte * binaryTuple = (byte *) malloc(8 * sizeof (unsigned char));
	unsigned int length = 8 + binaryLength();
	unsigned char const * bLength = reinterpret_cast<unsigned char const *>(&length);
	binaryTuple[0] = bLength[0];
	binaryTuple[1] = bLength[1];
	binaryTuple[2] = bLength[2];
	binaryTuple[3] = bLength[3];
	unsigned int binaryMask = createBinaryMask();
	unsigned char const * bMask = reinterpret_cast<unsigned char const *>(&binaryMask);
	binaryTuple[4] = bMask[0];
	binaryTuple[5] = bMask[1];
	binaryTuple[6] = bMask[2];
	binaryTuple[7] = bMask[3];
	copyBinaryContent(binaryTuple);
	return binaryTuple;
}

unsigned int Tuple::binaryLength(){
	unsigned int length = 0;
	std::vector<ParameterBase*>::iterator it = params.begin();
	for(; it != params.end(); ++it)
		length += oneArgLength(*it);
	return length;
}

unsigned int Tuple::oneArgLength(ParameterBase * arg){
	try{
		std::string stringArg = arg->get<std::string>();
		return stringArg.size()+1;
	} catch (std::bad_cast& bc) {
		try{
			arg->get<int>();
			return sizeof(int);
		} catch (std::bad_cast& bc) {
			return sizeof(float);
		}
	}
}

unsigned int Tuple::createBinaryMask(){
	int shift = 28;
	unsigned int mask = 0x00000000;
	std::vector<ParameterBase*>::iterator it = params.begin();
	for(; it != params.end() && shift >= 0; ++it){
		try{
			(*it)->get<std::string>();
			mask |= (0xC << shift);				//11
		} catch (std::bad_cast& bc) {
			try{
				(*it)->get<int>();
				mask |= (0x4 << shift);			//01
			} catch (std::bad_cast& bc) {
				mask |= (0x8 << shift);	//10
			}
		}
		shift -= 2;
	}
	return mask;
}

void Tuple::copyBinaryContent(byte * tuple){
	int size = 8;
	std::vector<ParameterBase*>::iterator it = params.begin();
	for(; it != params.end(); ++it)
		size += copyBinaryField(tuple, *it, size);
}

int Tuple::copyBinaryField(byte * tuple, ParameterBase * field, int size){
	try{
		std::string stringField = field->get<std::string>();
		return copyStringField(tuple, stringField, size);
	} catch (std::bad_cast& bc) {
		try{
			int intField = field->get<int>();
			copyIntField(tuple, intField, size);
			return sizeof(int);
		} catch (std::bad_cast& bc) {
			float floatField = field->get<float>();
			copyFloatField(tuple, floatField, size);
			return sizeof(float);
		}
	}
}

int Tuple::copyStringField(byte * tuple, std::string stringField, int size){
	const char * charField = stringField.c_str();
	tuple = (byte *) realloc(tuple, size + stringField.size()+1);
	for(unsigned int i = size; i < size + stringField.size()+1; i++)
		tuple[i] = charField[i-size];
	return stringField.size()+1;
}
void Tuple::copyIntField(byte * tuple, int intField, int size){
	tuple = (byte *) realloc(tuple, size + sizeof(int));
	unsigned char const * p = reinterpret_cast<unsigned char const *>(&intField);
	tuple[size] = p[0];
	tuple[size+1] = p[1];
	tuple[size+2] = p[2];
	tuple[size+3] = p[3];

}
void Tuple::copyFloatField(byte * tuple, float floatField, int size){
	tuple = (byte *) realloc(tuple, size + sizeof(float));
	unsigned char const * p = reinterpret_cast<unsigned char const *>(&floatField);
	tuple[size] = p[0];
	tuple[size+1] = p[1];
	tuple[size+2] = p[2];
	tuple[size+3] = p[3];
}
