#ifndef LINDACOMM_H_
#define LINDACOMM_H_
#define TUPLES_FILE_PATH_ENV "LINDA_TUPLES"
#define TUPLE_TEMPLATES_FILE_PATH_ENV "LINDA_TUPLE_TEMPLATES"
#include "tuple.h"
#include "tupleTemplate.h"
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

class LindaComm {
public:
	LindaComm() { srand(time(0)); }

// INPUT,READ //
public:
	Tuple * tupleInput(TupleTemplate tupleTemplate, int timeout);
	Tuple * tupleRead(TupleTemplate tupleTemplate, int timeout);
private:
	Tuple * tupleGet(TupleTemplate tupleTemplate, int timeout, bool remove);
	Tuple * findTupleMatchingTemplate(TupleTemplate tupleTemplate, bool remove);
public:
	void saveTupleTemplate(TupleTemplate tupleTemplate);
	void removeTupleTemplate(TupleTemplate tupleTemplate);

// OUTPUT //
public:
	void tupleOutput(Tuple tuple);
private:
	bool findTemplateMatchingTuple(Tuple tuple);
public:
	void saveTuple(Tuple tuple);

// UTIL //
private:
	bool matchBinaryMasks(int fd, TupleTemplate tupleTemplate);
	bool matchBinaryMasks(int fd, Tuple tuple);
	Tuple * matchTupleToTupleTemplate(int fd, unsigned int tupleSize, TupleTemplate tupleTemplate, bool remove);
	bool matchTupleFieldToTupleTemplateField(Tuple * tuple, TupleTemplate tupleTemplate, int i);
	bool matchStringWithQuantifier(std::string tupleArg, std::string tupleTemplateArg, Quantifier quantifier);
	bool matchIntegerWithQuantifier(int tupleArg, int tupleTemplateArg, Quantifier quantifier);
	bool matchFloatWithQuantifier(float tupleArg, float tupleTemplateArg, Quantifier quantifier);
// FS //
private:
	int getFile(const char * envName);
	const char * getDefaultFileName(const char * envName);
	void closeFile(int fd);
};

#endif /* LINDACOMM_H_ */
