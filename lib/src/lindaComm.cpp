#include "lindaComm.h"
#include <iostream>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>

void LindaComm::saveTupleTemplate(TupleTemplate tupleTemplate){
	int fd = getFile(TUPLE_TEMPLATES_FILE_PATH_ENV);
	lseek(fd, 0, SEEK_END);
	int tupleTemplateSize;
	byte * binaryTupleTemplate = tupleTemplate.toBinary(tupleTemplateSize);
	write(fd, binaryTupleTemplate, tupleTemplateSize);
	closeFile(fd);
}

void LindaComm::saveTuple(Tuple tuple){
	int fd = getFile(TUPLES_FILE_PATH_ENV);
	lseek(fd, 0, SEEK_END);
	int tupleSize;
	byte * binaryTuple = tuple.toBinary(tupleSize);
	write(fd, binaryTuple, tupleSize);
	closeFile(fd);
}

int LindaComm::getFile(const char * envName){
	const char * tuplesFileName = getenv(envName);
	int fd = open (tuplesFileName, O_RDWR | O_CREAT, 0777);
	lockf(fd, F_LOCK, 0);
	return fd;
}

void LindaComm::closeFile(int fd){
	lseek (fd, 0, SEEK_SET);
	lockf(fd, F_ULOCK, 0);
	close(fd);
}
