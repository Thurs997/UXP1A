#include "tuple.h"

Tuple * Tuple::fromBinary(byte binaryArray[]){
	Tuple * tuple = new Tuple();
	byte byteMask[] = {binaryArray[4], binaryArray[5], binaryArray[6], binaryArray[7]};
	unsigned int const * mask = reinterpret_cast<unsigned int const *>(&byteMask);
	int shift = 28;
	int position = 8;
	while(((*mask & (0xC << shift)) > 0) && shift >= 0){
		if(((*mask >> shift) & 0xC) == 0xC)
			tuple->addStringFromBinary(binaryArray, position);
		else if(((*mask >> shift) & 0xC) == 0x4){
			tuple->addIntFromBinary(binaryArray, position);
		} else {
			tuple->addFloatFromBinary(binaryArray, position);
		}
		shift -= 2;
	}
	return tuple;
}

void Tuple::addStringFromBinary(byte binaryArray[], int & position){
	addString(binToString(binaryArray, position));
}

void Tuple::addIntFromBinary(byte binaryArray[], int & position){
	addInteger(binToInt(binaryArray, position));
}

void Tuple::addFloatFromBinary(byte binaryArray[], int & position){
	addFloat(binToFloat(binaryArray, position));
}

std::vector<byte> * Tuple::toBinary(){
	std::vector<byte> *binaryTuple = new std::vector<byte>();
	unsigned int length = 8 + binaryLength();
	byte const * bLength = reinterpret_cast<byte const *>(&length);
	binaryTuple->push_back(bLength[0]);
	binaryTuple->push_back(bLength[1]);
	binaryTuple->push_back(bLength[2]);
	binaryTuple->push_back(bLength[3]);
	unsigned int binaryMask = createBinaryMask();
	byte const * bMask = reinterpret_cast<byte const *>(&binaryMask);
	binaryTuple->push_back(bMask[0]);
	binaryTuple->push_back(bMask[1]);
	binaryTuple->push_back(bMask[2]);
	binaryTuple->push_back(bMask[3]);
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

void Tuple::copyBinaryContent(std::vector<byte> * tuple){
	std::vector<ParameterBase*>::iterator it = params.begin();
	for(; it != params.end(); ++it)
		copyBinaryField(tuple, *it);
}

void Tuple::copyBinaryField(std::vector<byte> * tuple, ParameterBase * field){
	try{
		addStringToBin(tuple, field->get<std::string>());
	} catch (std::bad_cast& bc) {
		try{
			addIntToBin(tuple, field->get<int>());
		} catch (std::bad_cast& bc) {
			addFloatToBin(tuple, field->get<float>());
		}
	}
}
