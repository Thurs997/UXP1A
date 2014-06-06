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

class LindaComm {
public:
	LindaComm() {}

// INPUT,READ //
public:
	void tupleInput(TupleTemplate tupleTemplate, int timeout);
	void tupleRead(TupleTemplate tupleTemplate, int timeout);
private:
	bool findTupleMatchingTemplate(TupleTemplate tupleTemplate);
public:
	void saveTupleTemplate(TupleTemplate tupleTemplate);

// OUTPUT //
public:
	void tupleOutput(Tuple tuple);
private:
	bool findTemplateMatchingTuple(Tuple tuple);
public:
	void saveTuple(Tuple tuple);

// FS //
private:
	int getFile(const char * envName);
	void closeFile(int fd);
};

#endif /* LINDACOMM_H_ */
