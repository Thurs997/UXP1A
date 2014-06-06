#ifndef TUPLESERIALIZER_H_
#define TUPLESERIALIZER_H_
#include <string>
#include <vector>
#include <typeinfo>

typedef unsigned char byte;

class TupleSerializer {
public:
	TupleSerializer() { };
	std::string binToString(byte binaryArray[], int & position);
	int binToInt(byte binaryArray[], int & position);
	float binToFloat(byte binaryArray[], int & position);
	void addStringToBin(std::vector<byte> * bin, std::string stringField);
	void addIntToBin(std::vector<byte> * bin, int intField);
	void addFloatToBin(std::vector<byte> * bin, float floatField);
};

#endif /* TUPLESERIALIZER_H_ */
