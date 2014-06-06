#ifndef TUPLETEMPLATE_H_
#define TUPLETEMPLATE_H_
#include "tupleSerializer.h"
#include "parameter.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>

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

class TupleTemplate : public TupleSerializer {
private:
	std::vector<TupleTemplateArg> params;
	key_t semKey;
	int semId;
public:
	TupleTemplate() : semKey(-1), semId(-1) { }
	TupleTemplate(key_t key){ initSemaphore(key); }
	~TupleTemplate(){ }
	static TupleTemplate * fromBinary(byte binaryArray[]);
	byte * toBinary(int & size);
	int semWait(int timeout);
	int semPost();
	key_t getSemKey(){ return semKey; };
	void add(TupleTemplateArg arg){
		params.push_back(arg);
	}
	TupleTemplateArg get(int index){
		return params[index];
	}
	int size() {
		return params.size();
	}
	unsigned int createBinaryMask();
private:
	//toBinary
	void createSemaphore();
	void initSemaphore(key_t key);
	void deleteSemaphore();
	void addStringArgFromBinary(byte binaryArray[], int & position);
	void addIntegerArgFromBinary(byte binaryArray[], int & position);
	void addFloatArgFromBinary(byte binaryArray[], int & position);
	Quantifier getQuantifierFromByte(byte byteQuantifier);
	//fromBinary
	unsigned int binaryLength();
	unsigned int oneArgLength(TupleTemplateArg arg);
	void copyBinaryContent(std::vector<byte> * tupleTemplate);
	void copyBinaryArg(std::vector<byte> * tupleTemplate, TupleTemplateArg arg);
	void addQuantifierToBin(std::vector<byte> * tupleTemplate, Quantifier quantifier);
};

#endif /* TUPLETEMPLATE_H_ */
