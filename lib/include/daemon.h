/*
 * daemon.h
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#include <fcntl.h>

#define LOCK_FILE "/tmp/lindacommd.lock"
#define SLEEP_SEC 4

class Daemon {
public:
	Daemon();
	void run();
	bool isUnique();
	~Daemon();
private:
	int lockfd;
	static const char * TUPLES_FILE_PATH_ENV;
	static const char * TUPLE_TEMPLATES_FILE_PATH_ENV;
	static const char * DEFAULT_TUPLES_FILE_PATH;
	static const char * DEFAULT_TEMPLATES_FILE_PATH;
	const char * tupleFilePath;
	const char * queueFilePath;
	bool fileExists(const char * file);
};

#endif /* DAEMON_H_ */
