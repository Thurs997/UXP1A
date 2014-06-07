/*
 * daemon.cpp
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */

#include "daemon.h"
#include "defragmenter.h"
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <chrono>
#include <thread>

const char * Daemon::TUPLES_FILE_PATH_ENV = "LINDA_TUPLES";
const char * Daemon::TUPLE_TEMPLATES_FILE_PATH_ENV = "LINDA_TUPLE_TEMPLATES";
const char * Daemon::DEFAULT_TUPLES_FILE_PATH = "/tmp/tuples";
const char * Daemon::DEFAULT_TEMPLATES_FILE_PATH = "/tmp/tuple_templates";

Daemon::Daemon() {
	tupleFilePath = getenv(TUPLES_FILE_PATH_ENV);
	queueFilePath = getenv(TUPLE_TEMPLATES_FILE_PATH_ENV);

	if(tupleFilePath == NULL)
		tupleFilePath = DEFAULT_TUPLES_FILE_PATH;
	if(queueFilePath == NULL)
		queueFilePath = DEFAULT_TEMPLATES_FILE_PATH;
}

bool Daemon::fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}

bool Daemon::isUnique() {
    lockfd = open(LOCK_FILE, O_RDWR|O_CREAT, 0640);
    if (lockfd < 0)
        return false;
    if (lockf(lockfd, F_TLOCK, 0) < 0) // do a trylock
        return false;
    return true;
}

void Daemon::run() {
	while((fileExists(tupleFilePath) && fileExists(queueFilePath))) {
	        std::this_thread::sleep_for (std::chrono::seconds(SLEEP_SEC));
		//tuple
		int tupleFileDes = open(tupleFilePath, O_RDWR, 0777);
		lockf(tupleFileDes, F_LOCK, 0);
		Defragmenter tupleDefragmenter(tupleFileDes);
		tupleDefragmenter.defragment();
		lockf(tupleFileDes, F_ULOCK, 0);
		close(tupleFileDes);
		//queue
		int queueFileDes = open(queueFilePath, O_RDWR, 0777);
		lockf(queueFileDes, F_LOCK, 0);
		Defragmenter queueDefragmenter(queueFileDes);
		queueDefragmenter.defragment();
		lockf(queueFileDes, F_ULOCK, 0);
		close(queueFileDes);
	}
}

Daemon::~Daemon() {
	lockf(lockfd, F_ULOCK, 0);
	close(lockfd);
}







