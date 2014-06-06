/*
 * defragmenters.h
 *
 *  Created on: 6 cze 2014
 *      Author: Łukasz Kamiński
 */

#ifndef DEFRAGMENTERS_H_
#define DEFRAGMENTERS_H_

#include <fcntl.h>

typedef unsigned char byte;

class Defragmenter {
public:
	Defragmenter(int fileDescriptor) : fd(fileDescriptor){}
	void defragment();
private:
	int fd;
	off_t findEmptySpace(off_t startPos);
	off_t findUsedSpace(off_t startPos);
	void moveTuple(off_t *from, off_t *to);
	void clearTuple(off_t pos);
};

#endif /* DEFRAGMENTERS_H_ */
