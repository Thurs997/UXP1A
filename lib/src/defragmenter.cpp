/*
 * defragmenters.cpp
 *
 *  Created on: 6 cze 2014
 *      Author: Łukasz Kamiński
 */

#include "defragmenter.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

void Defragmenter::defragment() {
	off_t writerPos, readerPos, endPos;
	writerPos = readerPos = lseek(fd, 0, SEEK_CUR);
	endPos = lseek(fd, 0, SEEK_END);
	do {
		writerPos = findEmptySpace(writerPos);
		readerPos = findUsedSpace(writerPos);
		moveTuple(&readerPos, &writerPos);
	} while(writerPos < endPos && readerPos < endPos);
	if(writerPos < endPos) {
		ftruncate(fd, writerPos);
	}
}

off_t Defragmenter::findEmptySpace(off_t startPos) {
	int length, mask;
	off_t endPos;
	do{
		if(read(fd, &length, sizeof(int)) <= 0 || read(fd, &mask, sizeof(int)) <= 0) {
			return -1;
		}
		if(mask != 0) {
			lseek(fd, length - 2 * sizeof(int), SEEK_CUR);
		}
	} while(mask != 0);
	endPos = lseek(fd, length - 2 * sizeof(int), SEEK_CUR);
	lseek(fd, startPos, SEEK_SET);
	return endPos;
}

off_t Defragmenter::findUsedSpace(off_t startPos)  {
	int length, mask;
	off_t endPos;
	do{
		if(read(fd, &length, sizeof(int)) <= 0 || read(fd, &mask, sizeof(int)) <= 0) {
			return -1;
		}
		if(mask == 0) {
			lseek(fd, length - 2 * sizeof(int), SEEK_CUR);
		}
	} while(mask == 0);
	endPos = lseek(fd, length - 2 * sizeof(int), SEEK_CUR);
	lseek(fd, startPos, SEEK_SET);
	return endPos;
}

void Defragmenter::moveTuple(off_t *from, off_t *to) {
	if(from == to)
		return;
	int length;
	lseek(fd, *from, SEEK_SET);
	read(fd, &length, sizeof(length));
	byte * bytes = (byte *) malloc(length);
	lseek(fd, *from, SEEK_SET);
	read(fd, bytes, length);
	lseek(fd, *to, SEEK_SET);
	write(fd, bytes, length);
	free(bytes);
	clearTuple(*from);
	*from = lseek(fd, *from + length, SEEK_SET);
	*to = lseek(fd, *to + length, SEEK_SET);
}

void Defragmenter::clearTuple(off_t pos) {
	off_t myPos = lseek(fd, 0, SEEK_CUR);
	lseek(fd, pos + 4, SEEK_SET);
	int val = 0;
	write(fd, &val, sizeof(int));
	lseek(fd, myPos, SEEK_SET);
}



