#include "lindacomm.h"
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>

int process;

using namespace std;

void simpleTest(LindaComm *lc);
void generateTuples(LindaComm *lc, int count);
int readTuples(LindaComm *lc, int count);
int inputTuples(LindaComm *lc, int count);
void Test1(LindaComm *lc);

int main(int argc, char *argv[]){

	if(argc == 2) 
		process =  atoi(argv[1]);
	else 
		process = 1;
	LindaComm *lc = new LindaComm();
//	simpleTest(lc);
	Test1(lc);
	delete lc;
	
	cout << "KONIEC PROCESU " << process <<'\n';

	return 0;
}

void Test1(LindaComm *lc) {

	int tuplesNumber = 50;
	generateTuples(lc, tuplesNumber);
	if(readTuples(lc, tuplesNumber) == tuplesNumber)
		printf("Udalo sie wczytac wszystkie krotki\n");
	else
		printf("Nie udalo sie wczytac wszystkich krotek\n");
	if(inputTuples(lc, tuplesNumber) == tuplesNumber)
		printf("Udalo sie wczytac i usunac wszystkie krotki\n");
	else
		printf("Nie udalo sie wczytac i usunac wszystkich krotek\n");
	if(readTuples(lc, 1) == 0)
		printf("Plik wymiany jest pusty\n");
	else
		printf("Plik wymiany nie jest pusty\n");
		
}

void generateTuples(LindaComm *lc, int count) {
//	cout << "** Generowanie krotek: " << count <<" **\n";
	printf("** Generowanie krotek: %d ** Proces: %d\n", count, process);
	Tuple *tuple;

	for(int i = 0; i < count; i++) {
		printf("GENEROWANA KROTKA:\ttestString\t%d\t%d\n", i, process);
		tuple = new Tuple();
		tuple->addString("testString");
		tuple->addInteger(i);
		tuple->addInteger(process);
		lc->tupleOutput(tuple);
		delete tuple;
	}

}

int readTuples(LindaComm *lc, int count) {
	printf("** Przegladanie krotek bez usuwania: %d ** Proces: %d\n", count, process);
//	cout << "** Przegladanie krotek bez usuwania: " << count <<" **\n";
	string text;
	int number, processNumber, counter = 0;
	Tuple *tuple;
	TupleTemplate *tupleTemplate;

	for(int i = 0; i < count; i++) {
		tupleTemplate = new TupleTemplate();
		tupleTemplate->add(TupleTemplateArg::createFromString("testString", EQUAL));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(i, EQUAL));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(process, EQUAL));
		tuple = lc->tupleRead(tupleTemplate, 5);
		if(tuple != NULL) {
			counter++;
			text = tuple->getString(0);
			number = tuple->getInteger(1);
			processNumber = tuple->getInteger(2);
			delete tuple;
			printf("POBRANA KROTKA:\t%s\t%d\t%d\n", text.c_str(), number, processNumber);
		}
		else {
			printf("Nie udalo sie pobrac krotki:\ttestString\t%d\t%d\n", i, process);
		}
		delete tupleTemplate;
	}
	return counter;
}

int inputTuples(LindaComm *lc, int count) {
	printf("** Przegladanie krotek z usuwaniem: %d ** Proces: %d\n", count, process);
//	cout << "** Przegladanie krotek z usuwaniem: " << count <<" **\n";
	string text;
	int number, processNumber, counter = 0;
	Tuple *tuple;
	TupleTemplate *tupleTemplate;

	for(int i = 0; i < count; i++) {
		tupleTemplate = new TupleTemplate();
		tupleTemplate->add(TupleTemplateArg::createFromString("testString", EQUAL));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(i, EQUAL));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(process, EQUAL));
		tuple = lc->tupleInput(tupleTemplate, 5);
		if(tuple != NULL) {
			counter++;
			text = tuple->getString(0);
			number = tuple->getInteger(1);
			processNumber = tuple->getInteger(2);
			delete tuple;
			printf("USUWANA KROTKA:\t%s\t%d\t%d\n", text.c_str(), number, processNumber);
		}
		else {
			printf("Nie udalo sie pobrac krotki:\ttestString\t%d\t%d\n", i, process);
		}
		delete tupleTemplate;
	}
	return counter;
}

void simpleTest(LindaComm *lc) {
	cout << "** TUPLE **\n";
	Tuple *t = new Tuple();
	t->addString("test");                                                                       
	t->addInteger(5);
	 t->addFloat(0.123);
	cout << "* Wprowadzone dane *\n";
	cout << "1: " << t->getString(0) << '\n';
	cout << "2: " << t->getInteger(1) << '\n';
	cout << "3: " << t->getFloat(2) << '\n';

	cout << "** TUPLE TEMPLATE **\n";
	TupleTemplate *tt = new TupleTemplate();
	tt->add(TupleTemplateArg::createFromString("bla", EQUAL));
	tt->add(TupleTemplateArg::createFromString("", ANY));
	tt->add(TupleTemplateArg::createFromInteger(10, LESS));
	tt->add(TupleTemplateArg::createFromFloat(15.4321, MORE_OR_EQUAL));
	cout << "* Wprowadzone dane *\n";
	cout << "1: " << tt->get(0).quantifier << " :: " << tt->get(0).getStringArgument() << "\n";
	cout << "2: " << tt->get(1).quantifier << " :: " << tt->get(1).getStringArgument() << "\n";
	cout << "3: " << tt->get(2).quantifier << " :: " << tt->get(2).getIntegerArgument() << "\n";
	cout << "4: " << tt->get(3).quantifier << " :: " << tt->get(3).getFloatArgument() << "\n";

	cout << "Tworzenie LindaComm\n";
	cout << "tupleOutput\n";
	lc->tupleOutput(t);
	TupleTemplate *ttMatchingTuple = new TupleTemplate();
	ttMatchingTuple->add(TupleTemplateArg::createFromString("", ANY));
	ttMatchingTuple->add(TupleTemplateArg::createFromInteger(0, ANY));
	ttMatchingTuple->add(TupleTemplateArg::createFromFloat(0, ANY));
	cout << "przed szukaniem\n";
	Tuple *foundTuple = lc->tupleInput(ttMatchingTuple, 5);
	cout << "po szukaniu\n";
	if(foundTuple == NULL)
		cout << "Nie znaleziono krotki!\n";
	else{
		cout << "Znaleziono krotke:\n";
		cout << foundTuple->getString(0) << '\n';
		cout << foundTuple->getInteger(1) << '\n';
		cout << foundTuple->getFloat(2) << '\n';
	}
	delete t;
	delete tt;
	delete ttMatchingTuple;
	delete foundTuple;
}
