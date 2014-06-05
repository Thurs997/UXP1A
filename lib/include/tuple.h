#ifndef TUPLE_H_
#define TUPLE_H_
#include <string>
#include <vector>
#include "parameter.h"

typedef unsigned char byte;

class Tuple {
private:
	std::vector<ParameterBase*> params;
public:
	Tuple(){ }
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
	std::string getString(int index){
		return getArg<std::string>(index);
	}
	int getInteger(int index){
		return getArg<int>(index);
	}
	float getFloat(int index){
		return getArg<float>(index);
	}
	int size() {
		return params.size();
	}
private:
	template<typename T>
	void addArg(T arg) {
		params.push_back(new Parameter<T>(arg));
	}
	template<typename T>
	T getArg(int index){
		return (params[index])->get<T>();
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
