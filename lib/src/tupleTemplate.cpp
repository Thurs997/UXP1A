#include "tupleTemplate.h"

TupleTemplate * TupleTemplate::fromBinary(byte binaryArray[]){
	TupleTemplate * tupleTemplate = new TupleTemplate();
	byte byteMask[] = {binaryArray[4], binaryArray[5], binaryArray[6], binaryArray[7]};
	unsigned int const * mask = reinterpret_cast<unsigned int const *>(&byteMask);
	int shift = 28;
	int position = 8;
	while(((*mask & (0xC << shift)) > 0) && shift >= 0){
		if(((*mask >> shift) & 0xC) == 0xC)
			tupleTemplate->addStringArgFromBinary(binaryArray, position);
		else if(((*mask >> shift) & 0xC) == 0x4){
			tupleTemplate->addIntegerArgFromBinary(binaryArray, position);
		} else {
			tupleTemplate->addFloatArgFromBinary(binaryArray, position);
		}
		shift -= 2;
	}
	return tupleTemplate;
}

void TupleTemplate::addStringArgFromBinary(byte binaryArray[], int & position){
	Quantifier q = getQuantifierFromByte(binaryArray[position++]);
	std::string str = binToString(binaryArray, position);
	add(TupleTemplateArg::createFromString(str, q));
}

void TupleTemplate::addIntegerArgFromBinary(byte binaryArray[], int & position){
	Quantifier q = getQuantifierFromByte(binaryArray[position++]);
	int arg = binToInt(binaryArray, position);
	add(TupleTemplateArg::createFromInteger(arg, q));
}

void TupleTemplate::addFloatArgFromBinary(byte binaryArray[], int & position){
	Quantifier q = getQuantifierFromByte(binaryArray[position++]);
	float arg = binToFloat(binaryArray, position);
	add(TupleTemplateArg::createFromFloat(arg, q));
}

Quantifier TupleTemplate::getQuantifierFromByte(byte byteQuantifier){
	return static_cast<Quantifier>(byteQuantifier);
}

byte * TupleTemplate::toBinary(int & size){
	std::vector<byte> * binaryTuple = new std::vector<byte>();
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
	size = binaryTuple->size();
	return &(*binaryTuple)[0];
}

unsigned int TupleTemplate::binaryLength(){
	unsigned int length = 0;
	std::vector<TupleTemplateArg>::iterator it = params.begin();
	for(; it != params.end(); ++it)
		length += oneArgLength(*it);
	return length;
}

unsigned int TupleTemplate::oneArgLength(TupleTemplateArg arg){
	ParameterBase * argValue = arg.arg;
	try{
		std::string stringArg = argValue->get<std::string>();
		return stringArg.size()+2;
	} catch (std::bad_cast& bc) {
		try{
			argValue->get<int>();
			return sizeof(int)+1;
		} catch (std::bad_cast& bc) {
			return sizeof(float)+1;
		}
	}
}

unsigned int TupleTemplate::createBinaryMask(){
	int shift = 28;
	unsigned int mask = 0x00000000;
	std::vector<TupleTemplateArg>::iterator it = params.begin();
	for(; it != params.end() && shift >= 0; ++it){
		try{
			(*it).arg->get<std::string>();
			mask |= (0xC << shift);				//11
		} catch (std::bad_cast& bc) {
			try{
				(*it).arg->get<int>();
				mask |= (0x4 << shift);			//01
			} catch (std::bad_cast& bc) {
				mask |= (0x8 << shift);	//10
			}
		}
		shift -= 2;
	}
	return mask;
}

void TupleTemplate::copyBinaryContent(std::vector<byte> * tupleTemplate){
	std::vector<TupleTemplateArg>::iterator it = params.begin();
	for(; it != params.end(); ++it)
		copyBinaryArg(tupleTemplate, *it);
}

void TupleTemplate::copyBinaryArg(std::vector<byte> * tupleTemplate, TupleTemplateArg arg){
	addQuantifierToBin(tupleTemplate, arg.quantifier);
	try{
		addStringToBin(tupleTemplate, arg.arg->get<std::string>());
	} catch (std::bad_cast& bc) {
		try{
			addIntToBin(tupleTemplate, arg.arg->get<int>());
		} catch (std::bad_cast& bc) {
			addFloatToBin(tupleTemplate, arg.arg->get<float>());
		}
	}
}

void TupleTemplate::addQuantifierToBin(std::vector<byte> * tupleTemplate, Quantifier quantifier){
	tupleTemplate->push_back(quantifier);
}
