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

void Daemon::daemonize() {
    int pid = fork();
    if (pid < 0)
        exit(1); // fork failed
    if (pid > 0)
        exit(0); // still in parent process, so quit

    // in child process, obtain new session and process group at the same time
    setsid();

    int devnull = open("/dev/null", O_RDWR);

    // close stdin stdout and stderr which are connected to current terminal
    close(0);
    close(1);
    close(2);

    // attach stdin stdout and stderr to /dev/null
    // by dup-ing the devnull descriptor
    dup2(devnull, 0);
    dup2(devnull, 1);
    dup2(devnull, 2);

    // get rid of devnull
    close(devnull);

    // open a lockfile
    int lockfd = open(LOCK_FILE, O_RDWR|O_CREAT, 0640);
    if (lockfd < 0)
        exit(1); // Error opening the file
    if (lockf(lockfd, F_TLOCK, 0) < 0) // do a trylock
        exit(0); // if already locked or error, exit (ensures a single process in the system)
}

void Daemon::run() {
	while((fileExists(tupleFilePath) && fileExists(queueFilePath))) {
		usleep(SLEEP_SEC * 1000);
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







