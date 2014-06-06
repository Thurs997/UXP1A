#include "lindaComm.h"
#include <iostream>
#include <sys/file.h>

void LindaComm::saveTuple(Tuple tuple){
	int fd = getTuplesFile();
	int a;
	std::cin >> a;
	write(fd, "bla", 4);
	flock(fd, LOCK_UN);
	//lockf(fd, F_ULOCK, 0);
}

int LindaComm::getTuplesFile(){
	const char * tuplesFileName = getenv(TUPLES_FILE_PATH_ENV);
	int fd = open (tuplesFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	flock(fd, LOCK_EX);
	//lockf(fd, F_LOCK, 0);
	return fd;
}

