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
	void input(TupleTemplate tupleTemplate, int timeout);
	void read(TupleTemplate tupleTemplate, int timeout);
private:
	bool findTupleMatchingTemplate(TupleTemplate tupleTemplate);
	void saveTupleTemplate(TupleTemplate tupleTemplate);

// OUTPUT //
public:
	void output(Tuple tuple);
private:
	bool findTemplateMatchingTuple(Tuple tuple);
public:
	void saveTuple(Tuple tuple);

// FS //
private:
	int getTuplesFile();
	int getTupleTemplatesFile();
};

#endif /* LINDACOMM_H_ */
