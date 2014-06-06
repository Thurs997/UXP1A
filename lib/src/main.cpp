/*
 * main.cpp
 *
 *  Created on: 6 cze 2014
 *      Author: Łukasz Kamiński
 */

#include "daemon.h"

int main() {
	Daemon d;
	d.daemonize();
	d.run();
	return 0;
}
