/*
 * daemon.h
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#define LOCK_FILE "/tmp/lindacommd.lock"
#define SLEEP_SEC 4
#define LINDACOMM_TUPLE_FILE_ENV "$LINDACOMM_TUPLE_FILE"
#define LINDACOMM_QUEUE_FILE_ENV "$LINDACOMM_QUEUE_FILE"


class Daemon {
public:
	void daemonize();
	void init();
	void run();
private:
	char * tupleFileName;
	char * queueFileName;
	int runningLibInstances();
	void defragment();
	void defragmentTuple();
	void defragmentQueue();
	bool fileExists(const char * file);
};

#endif /* DAEMON_H_ */
