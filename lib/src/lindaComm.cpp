#include "lindaComm.h"

const char * LindaComm::TUPLES_FILE_PATH_ENV = "LINDA_TUPLES";
const char * LindaComm::TUPLE_TEMPLATES_FILE_PATH_ENV = "LINDA_TUPLE_TEMPLATES";
const char * LindaComm::DAEMON_PATH_ENV = "LINDA_DAEMON";
const char * LindaComm::DEFAULT_TUPLES_FILE_PATH = "/tmp/tuples";
const char * LindaComm::DEFAULT_TEMPLATES_FILE_PATH = "/tmp/tuple_templates";
const char * LindaComm::DEFAULT_DAEMON_PATH = "/usr/bin/lindacommd";

LindaComm::LindaComm() {
	//create files
	closeFile(getFile(TUPLES_FILE_PATH_ENV));
	closeFile(getFile(TUPLE_TEMPLATES_FILE_PATH_ENV));
	//wake up daemon
	const char * daemonCommandName = getenv(DAEMON_PATH_ENV);
	if(daemonCommandName == NULL) {
		daemonCommandName = DEFAULT_DAEMON_PATH;
	}
	system(daemonCommandName);

}

Tuple * LindaComm::tupleInput(TupleTemplate * tupleTemplate, int timeout){
	return tupleGet(tupleTemplate, timeout, true);
}

Tuple * LindaComm::tupleRead(TupleTemplate * tupleTemplate, int timeout){
	return tupleGet(tupleTemplate, timeout, false);
}

Tuple * LindaComm::tupleGet(TupleTemplate * tupleTemplate, int timeout, bool remove){
	Tuple * tuple = findTupleMatchingTemplate(tupleTemplate, remove);
	if(tuple == NULL){
		saveTupleTemplate(tupleTemplate);
		if(tupleTemplate->semWait(timeout) == 0)
	    	return findTupleMatchingTemplate(tupleTemplate, remove);
	    else{
	    	removeTupleTemplate(tupleTemplate);
	    	return NULL;
	    }

	}
	return tuple;
}


Tuple * LindaComm::findTupleMatchingTemplate(TupleTemplate * tupleTemplate, bool remove){
	int fd = getFile(TUPLES_FILE_PATH_ENV);
	int position = 0;
	unsigned int size;
	while(read(fd, &size, sizeof(int)) > 0){
		if(matchBinaryMasks(fd, tupleTemplate)){
			Tuple * t = matchTupleToTupleTemplate(fd, size, tupleTemplate, remove);
			if(t != NULL){
				closeFile(fd);
				return t;
			}
		}
		position += size;
		lseek(fd, position, SEEK_SET);
	}
	closeFile(fd);
	return NULL;
}

bool LindaComm::matchBinaryMasks(int fd, TupleTemplate * tupleTemplate){
	unsigned int fileBinaryMask;
	read(fd, &fileBinaryMask, sizeof(unsigned int));
	unsigned int objectBinaryMask = tupleTemplate->createBinaryMask();
	return fileBinaryMask == objectBinaryMask;
}

bool LindaComm::matchBinaryMasks(int fd, Tuple * tuple){
	unsigned int fileBinaryMask;
	read(fd, &fileBinaryMask, sizeof(unsigned int));
	unsigned int objectBinaryMask = tuple->createBinaryMask();
	return fileBinaryMask == objectBinaryMask;
}

Tuple * LindaComm::matchTupleToTupleTemplate(int fd, unsigned int tupleSize, TupleTemplate * tupleTemplate, bool remove){
	lseek(fd, -8, SEEK_CUR);
	byte * binaryTuple = (byte *) malloc(tupleSize);
	read(fd, binaryTuple, tupleSize);
	Tuple * tuple = Tuple::fromBinary(binaryTuple);
	free(binaryTuple);
	for(int i=0; i<tuple->size(); ++i){
		if(!matchTupleFieldToTupleTemplateField(tuple, tupleTemplate, i))
			return NULL;
	}
	if(remove){
		lseek(fd, (off_t) (4 - ((int) tupleSize)), SEEK_CUR);
		byte zero[4] = {'\0', '\0', '\0', '\0'};
		write(fd, zero, 4);
	}
	return tuple;
}

bool LindaComm::matchTupleFieldToTupleTemplateField(Tuple * tuple, TupleTemplate * tupleTemplate, int i){
	Quantifier quantifier = tupleTemplate->get(i).quantifier;
	if(quantifier == ANY)
		return true;
	try{
		std::string tupleString = tuple->getString(i);
		std::string tupleTemplateString = tupleTemplate->get(i).getStringArgument();
		return matchStringWithQuantifier(tupleString, tupleTemplateString, quantifier);
	} catch (std::bad_cast& bc) {
		try{
			int tupleInt = tuple->getInteger(i);
			int tupleTemplateInt = tupleTemplate->get(i).getIntegerArgument();
			return matchIntegerWithQuantifier(tupleInt, tupleTemplateInt, quantifier);
		} catch (std::bad_cast& bc) {
			float tupleFloat = tuple->getFloat(i);
			float tupleTemplateFloat = tupleTemplate->get(i).getFloatArgument();
			return matchFloatWithQuantifier(tupleFloat, tupleTemplateFloat, quantifier);
		}
	}
}

bool LindaComm::matchStringWithQuantifier(std::string tupleArg, std::string tupleTemplateArg, Quantifier quantifier){
	switch(quantifier){
		case EQUAL:
			return tupleArg == tupleTemplateArg;
		case LESS:
			return tupleArg < tupleTemplateArg;
		case MORE:
			return tupleArg > tupleTemplateArg;
		case LESS_OR_EQUAL:
			return tupleArg <= tupleTemplateArg;
		case MORE_OR_EQUAL:
			return tupleArg >= tupleTemplateArg;
		default:
			return true;
	}
}

bool LindaComm::matchIntegerWithQuantifier(int tupleArg, int tupleTemplateArg, Quantifier quantifier){
	switch(quantifier){
		case EQUAL:
			return tupleArg == tupleTemplateArg;
		case LESS:
			return tupleArg < tupleTemplateArg;
		case MORE:
			return tupleArg > tupleTemplateArg;
		case LESS_OR_EQUAL:
			return tupleArg <= tupleTemplateArg;
		case MORE_OR_EQUAL:
			return tupleArg >= tupleTemplateArg;
		default:
			return true;
	}
}

bool LindaComm::matchFloatWithQuantifier(float tupleArg, float tupleTemplateArg, Quantifier quantifier){
	switch(quantifier){
		case EQUAL:
			return tupleArg == tupleTemplateArg;
		case LESS:
			return tupleArg < tupleTemplateArg;
		case MORE:
			return tupleArg > tupleTemplateArg;
		case LESS_OR_EQUAL:
			return tupleArg <= tupleTemplateArg;
		case MORE_OR_EQUAL:
			return tupleArg >= tupleTemplateArg;
		default:
			return true;
	}
}

// Return: 0 = consumated; 1 = posted in file
int LindaComm::tupleOutput(Tuple * tuple){
	int fd = saveTuple(tuple);
	bool found = findTemplateMatchingTuple(tuple);
	closeFile(fd);
	return (found ? 0 : 1);
}

bool LindaComm::findTemplateMatchingTuple(Tuple * tuple){
	int fd = getFile(TUPLE_TEMPLATES_FILE_PATH_ENV);
	int position = 0;
	unsigned int size;
	while(read(fd, &size, sizeof(int)) > 0){
		if(matchBinaryMasks(fd, tuple) && matchTupleTemplateToTuple(fd, size, tuple)){
			closeFile(fd);
			return true;
		}
		position += size;
		lseek(fd, position, SEEK_SET);
	}
	closeFile(fd);
	return false;
}

bool LindaComm::matchTupleTemplateToTuple(int fd, unsigned int size, Tuple * tuple){
	lseek(fd, -8, SEEK_CUR);
	byte * binaryTupleTemplate = (byte *) malloc(size);
	read(fd, binaryTupleTemplate, size);
	TupleTemplate * tupleTemplate = TupleTemplate::fromBinary(binaryTupleTemplate);
	free(binaryTupleTemplate);
	for(int i=0; i<tuple->size(); ++i){
		if(!matchTupleFieldToTupleTemplateField(tuple, tupleTemplate, i))
			return false;
	}
	lseek(fd, (off_t) (4 - ((int) size)), SEEK_CUR);
	byte zero[4] = {'\0', '\0', '\0', '\0'};
	write(fd, zero, 4);
	tupleTemplate->semPost();
	return true;
}

void LindaComm::removeTupleTemplate(TupleTemplate * tupleTemplate){
	int fd = getFile(TUPLE_TEMPLATES_FILE_PATH_ENV);
	unsigned int size;
	unsigned int position = 0;
	key_t key;
	while(read(fd, &size, sizeof(unsigned int)) > 0){
		lseek(fd, 4, SEEK_CUR);
		read(fd, &key, sizeof(key_t));
		if(key == tupleTemplate->getSemKey()){
			lseek(fd, -sizeof(key_t)-4, SEEK_CUR);
			byte zero[4] = {'\0', '\0', '\0', '\0'};
			write(fd, zero, 4);
			break;
		}
		position += size;
		lseek(fd, position, SEEK_SET);
	}
	closeFile(fd);
}

void LindaComm::saveTupleTemplate(TupleTemplate * tupleTemplate){
	int fd = getFile(TUPLE_TEMPLATES_FILE_PATH_ENV);
	lseek(fd, 0, SEEK_END);
	int tupleTemplateSize;
	byte * binaryTupleTemplate = tupleTemplate->toBinary(tupleTemplateSize);
	write(fd, binaryTupleTemplate, tupleTemplateSize);
	closeFile(fd);
}

int LindaComm::saveTuple(Tuple * tuple){
	int fd = getFile(TUPLES_FILE_PATH_ENV);
	lseek(fd, 0, SEEK_END);
	int tupleSize;
	byte * binaryTuple = tuple->toBinary(tupleSize);
	write(fd, binaryTuple, tupleSize);
	return fd;
}

int LindaComm::getFile(const char * envName){
	const char * tuplesFileName = getenv(envName);
	if(tuplesFileName == NULL)
		tuplesFileName = getDefaultFileName(envName);
	int fd = open (tuplesFileName, O_RDWR | O_CREAT, 0777);
	lockf(fd, F_LOCK, 0);
	return fd;
}

const char * LindaComm::getDefaultFileName(const char * envName){
	if(envName == TUPLES_FILE_PATH_ENV)
		return DEFAULT_TUPLES_FILE_PATH;
	else
		return DEFAULT_TEMPLATES_FILE_PATH;
}

void LindaComm::closeFile(int fd){
	lseek (fd, 0, SEEK_SET);
	lockf(fd, F_ULOCK, 0);
	close(fd);
}
