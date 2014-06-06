#ifndef TUPLE_H_
#define TUPLE_H_
#include <string>
#include <vector>
#include "parameter.h"
#include "tupleSerializer.h"

class Tuple : TupleSerializer {
private:
	std::vector<ParameterBase*> params;
public:
	Tuple(){ }
	~Tuple(){ }
	static Tuple * fromBinary(byte binaryArray[]);
	byte * toBinary(int & tupleSize);
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
	//toBinary
	void addStringFromBinary(byte binaryArray[], int & position);
	void addIntFromBinary(byte binaryArray[], int & position);
	void addFloatFromBinary(byte binaryArray[], int & position);
	//fromBinary
	unsigned int binaryLength();
	unsigned int oneArgLength(ParameterBase *);
	unsigned int createBinaryMask();
	void copyBinaryContent(std::vector<byte> * tuple);
	void copyBinaryField(std::vector<byte> * tuple, ParameterBase * field);
};

#endif /* TUPLE_H_ */
