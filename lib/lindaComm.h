/*
 * lindaComm.h
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */
#ifndef LINDACOMM_H_
#define LINDACOMM_H_
#include <string>
#include <vector>
#include "parameter.h"

class TupleTemplate {
private:
};

class Tuple {
private:
	vector<ParameterBase*> params;
public:
	template<typename T> void addArg<T>(T arg) {params.push_back(&(Parameter<T>(arg)));}

};

void output(Tuple);
Tuple input(std::string, int);
void read(std::string, int);



#endif /* LINDACOMM_H_ */
