/*
 * main.cpp
 *
 *  Created on: 6 cze 2014
 *      Author: Łukasz Kamiński
 */

#include "daemon.h"
#include <unistd.h>
int main() {
	Daemon d;
	if(d.isUnique()) {
		//daemon(0, 0);
		d.run();
	}
	return 0;
}
