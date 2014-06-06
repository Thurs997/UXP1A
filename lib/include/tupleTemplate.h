#ifndef TUPLETEMPLATE_H_
#define TUPLETEMPLATE_H_
#include "tupleSerializer.h"
#include "parameter.h"

enum Quantifier{
	ANY = 0x01, EQUAL = 0x02, LESS = 0x03, MORE = 0x04, LESS_OR_EQUAL = 0x05, MORE_OR_EQUAL = 0x06
};

struct TupleTemplateArg{
	ParameterBase * arg;
	Quantifier quantifier;

	TupleTemplateArg(ParameterBase * p, Quantifier q){
		arg = p;
		quantifier = q;
	}

	static TupleTemplateArg createFromString(std::string str, Quantifier q){
		return TupleTemplateArg(new Parameter<std::string>(str), q);
	}

	static TupleTemplateArg createFromInteger(int arg, Quantifier q){
		return TupleTemplateArg(new Parameter<int>(arg), q);
	}

	static TupleTemplateArg createFromFloat(float arg, Quantifier q){
		return TupleTemplateArg(new Parameter<float>(arg), q);
	}

	std::string getStringArgument(){
		return arg->get<std::string>();
	}

	int getIntegerArgument(){
		return arg->get<int>();
	}

	float getFloatArgument(){
		return arg->get<float>();
	}
};

class TupleTemplate : TupleSerializer {
private:
	std::vector<TupleTemplateArg> params;
public:
	TupleTemplate(){ }
	~TupleTemplate(){ }
	static TupleTemplate * fromBinary(byte binaryArray[]);
	byte * toBinary(int & size);
	void add(TupleTemplateArg arg){
		params.push_back(arg);
	}
	TupleTemplateArg get(int index){
		return params[index];
	}
	int size() {
		return params.size();
	}
private:
	//toBinary
	void addStringArgFromBinary(byte binaryArray[], int & position);
	void addIntegerArgFromBinary(byte binaryArray[], int & position);
	void addFloatArgFromBinary(byte binaryArray[], int & position);
	Quantifier getQuantifierFromByte(byte byteQuantifier);
	//fromBinary
	unsigned int binaryLength();
	unsigned int oneArgLength(TupleTemplateArg arg);
	unsigned int createBinaryMask();
	void copyBinaryContent(std::vector<byte> * tupleTemplate);
	void copyBinaryArg(std::vector<byte> * tupleTemplate, TupleTemplateArg arg);
	void addQuantifierToBin(std::vector<byte> * tupleTemplate, Quantifier quantifier);
};

#endif /* TUPLETEMPLATE_H_ */
