/*
 * daemon.cpp
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */

#include "daemon.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sys/stat.h>


void Daemon::init() {
	//get file names
	tupleFilePath = getenv(LINDACOMM_TUPLE_FILE_ENV);
	queueFilePath = getenv(LINDACOMM_QUEUE_FILE_ENV);
	if(!(tupleFilePath && queueFilePath)) {
		exit(1);
	}
	//check if files exist

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
	while(runningLibInstances() > 0) {
		std::this_thread::sleep_for(std::chrono::seconds(SLEEP_SEC));
		defragment();
	}
}

int Daemon::runningLibInstances() {
	//TODO
	//count results of 'find /tmp | grep lindacomm.pid_*'
}

void Daemon::defragment() {
	defragmentTuples();
	defragmentQueue();
}

void Daemon::defragmentTuple() {
	//TODO
}

void Daemon::defragmentQueue() {
	//TODO
}

int main() {
	Daemon d;
	d.daemonize();
	d.init();
	d.run();
	return 0;
}





