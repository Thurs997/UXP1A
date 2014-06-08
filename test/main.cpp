#include "lindacomm.h"
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>

int process;

using namespace std;

ifstream::pos_type filesize(const char *filename);
void simpleTest(LindaComm *lc);
void generateTuples(LindaComm *lc, int count, bool verbose = true);
int readTuples(LindaComm *lc, int count, Quantifier q, bool verbose = true, int it = 1);
int inputTuples(LindaComm *lc, int count, Quantifier q, bool verbose = true, int it = 1);
bool Test1(LindaComm *lc);
bool Test2(LindaComm *lc);
bool Test3(LindaComm *lc);
bool Test4(LindaComm *lc);
bool Test5(LindaComm *lc);

int main(int argc, char *argv[]){

	if(argc == 2) 
		process =  atoi(argv[1]);
	else 
		process = 1;
	bool success = true;
	LindaComm *lc = new LindaComm();
//	if(!Test1(lc))
//		success = false;
//	if(!Test2(lc))
//		success = false;
	if(!Test3(lc))
		success = false;
//	if(!Test4(lc))
//		success = false;
//	if(!Test5(lc))
//		success = false;
	delete lc;
	if(success)
		printf("%d Wszystkie testy zakończone sukcesem\n", process);
	else
		printf("%d Jeden lub wiecej testow nie zakonczylo sie sukcesem\n", process);
	printf("%d KONIEC PROCESU\n", process);

	return 0;
}

bool Test1(LindaComm *lc) {
	printf("%d ***** TEST 1 *****\n",process);
	bool success = true;
	Tuple *tuple = new Tuple();
	for(int i = 0; i < 16; i++)
		tuple->addInteger(i);
	lc->tupleOutput(tuple);
	printf("%d Stworzono ktotke z 16 parametrami\n", process);
	TupleTemplate *tupleTemplate = new TupleTemplate();
	for(int i = 0; i < 16; i++)
		tupleTemplate->add(TupleTemplateArg::createFromInteger(0, ANY));
	Tuple *foundTuple = lc->tupleInput(tupleTemplate, 1);

	if(foundTuple != NULL)
		printf("%d Pobrano krotke z 16 parametrami\n", process);
	else {
		success = false;
		printf("%d Nie mozna znalezc krotki\n", process);
	}

	delete foundTuple;
	delete tuple;
	delete tupleTemplate;
	tuple = new Tuple();
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	tuple->addInteger(5);
	lc->tupleOutput(tuple);
	tupleTemplate = new TupleTemplate();
	tupleTemplate->add(TupleTemplateArg::createFromInteger(0, ANY));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(5, EQUAL));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(6, LESS));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(4, MORE));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(5, LESS_OR_EQUAL));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(5, MORE_OR_EQUAL));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(6, LESS_OR_EQUAL));
	tupleTemplate->add(TupleTemplateArg::createFromInteger(4, MORE_OR_EQUAL));

	foundTuple = lc->tupleInput(tupleTemplate, 1);
	if(foundTuple != NULL)
		printf("%d Udalo sie pobrac krotke spelniajaca kryteria\n", process);
	else {
		success = false;
		printf("%d Nie mozna znalezc krotki\n", process);
	}

	delete tuple;
	delete tupleTemplate;
	delete foundTuple;	

	if(success) {
		printf("%d Test1 zakończony sukcesem!\n", process);
	}
	else {
		printf("%d Test1 zakończony niepowodzeniem\n", process);
	}
	return success;
}

bool Test2(LindaComm *lc) {

	printf("%d ***** TEST 2 *****\n",process);
	int tuplesNumber = 50;
	bool success = true;
	generateTuples(lc, tuplesNumber);
	if(readTuples(lc, tuplesNumber, EQUAL) == tuplesNumber)
		printf("%d Udalo sie wczytac wszystkie krotki\n", process);
	else {
		success = false;
		printf("%d Nie udalo sie wczytac wszystkich krotek\n", process);
	}
	if(inputTuples(lc, tuplesNumber, EQUAL) == tuplesNumber)
		printf("%d Udalo sie wczytac i usunac wszystkie krotki\n", process);
	else {
		success = false;
		printf("%d Nie udalo sie wczytac i usunac wszystkich krotek\n", process);
	}
	if(readTuples(lc, 1, ANY) == 0)
		printf("%d Plik wymiany jest pusty\n", process);
	else {
		success = false;
		printf("%d Plik wymiany nie jest pusty\n", process);
	}
	if(success) {
		printf("%d Test2 zakończony sukcesem!\n", process);
	}
	else {
		printf("%d Test2 zakończony niepowodzeniem\n", process);
	}
	return success;		
}

bool Test3(LindaComm *lc) {
	printf("%d ***** TEST 3 *****\n",process);
	int tuplesNumber = 100;
	bool success = true;
	int fileSize1, fileSize2;
	generateTuples(lc, tuplesNumber);
	fileSize1 = (int)filesize("/tmp/tuples");
	printf("%d Rozmiar pliku po wygenerowaniu krotek: %d\n", process, fileSize1);	
	
	if(inputTuples(lc, tuplesNumber, EQUAL, true, 2) == tuplesNumber/2)
		printf("%d Udalo sie wczytac i usunac wszystkie krotki\n", process);
	else {
		success = false;
		printf("%d Nie udalo sie wczytac i usunac wszystkich krotek\n", process);
	}
	sleep(5);
	fileSize2 = (int)filesize("/tmp/tuples");
	printf("%d Rozmiar pliku po usunieciu polowy krotek: %d\n", process, fileSize2);
	printf("%d Rozmiar pliku przed: %d Rozmiar pliku po: %d\n", process, fileSize1, fileSize2);

	if(fileSize2 >= fileSize1)
		success = false;
	
	if(inputTuples(lc, tuplesNumber/2, ANY) == tuplesNumber/2)
		printf("%d Udalo sie wczytac i usunac wszystkie krotki\n", process);
	else {
		success = false;
		printf("%d Nie udalo sie wczytac i usunac wszystkich krotek\n", process);
	}
	if(success) {
		printf("%d Test3 zakończony sukcesem!\n", process);
	}
	else {
		printf("%d Test3 zakończony niepowodzeniem\n", process);
	}

	return success;
}
bool Test4(LindaComm *lc) {

	printf("%d ***** TEST 4 *****\n",process);
	int tuplesNumber = 50;
	bool success = true;
	if(fork())
		generateTuples(lc, tuplesNumber*2);
	else {
		process++;
		if(!fork())
			process++;
		printf("%d Stworzono proces potomny\n", process);
		sleep(3);
		if(inputTuples(lc, tuplesNumber, ANY) == tuplesNumber)
			printf("%d Udalo sie wczytac i usunac wszystkie krotki\n", process);
		else {
			success = false;
			printf("%d Nie udalo sie wczytac i usunac wszystkich krotek\n", process);
		}
	}
	if(success) {
		printf("%d Test4 zakończony sukcesem!\n", process);
	}
	else {
		printf("%d Test4 zakończony niepowodzeniem\n", process);
	}
	return success;	
}

bool Test5(LindaComm *lc) {
	printf("%d ***** TEST 5 *****\n",process);
	int tuplesNumber = 5000;
	bool success = true;
	double seconds;
	time_t timer = time(NULL);
	generateTuples(lc, tuplesNumber, false);
	if(inputTuples(lc, tuplesNumber, ANY, false) == tuplesNumber)
		printf("%d Udalo sie wczytac i usunac wszystkie krotki\n", process);
	else {
		success = false;
		printf("%d Nie udalo sie wczytac i usunac wszystkich krotek\n", process);
	}
	seconds = difftime(time(NULL), timer);
	if(success) {
		printf("%d Test5 zakończony sukcesem! Czas testu: %f\n", process, seconds);
	}
	else {
		printf("%d Test5 zakończony niepowodzeniem. Czas testu %f\n", process, seconds);
	}
	return success;
}

void generateTuples(LindaComm *lc, int count, bool verbose) {
	printf("%d ** Generowanie krotek: %d **\n", process, count);
	Tuple *tuple;

	for(int i = 0; i < count; i++) {
		if(verbose)
			printf("%d GENEROWANA KROTKA:\ttestString\t%d\t%d\n",process, i, process);
		tuple = new Tuple();
		tuple->addString("testString");
		tuple->addInteger(i);
		tuple->addInteger(process);
		lc->tupleOutput(tuple);
		delete tuple;
	}

}

int readTuples(LindaComm *lc, int count, Quantifier q, bool verbose, int it) {
	printf("%d ** Przegladanie krotek bez usuwania: %d **\n",process , count/it);
	string text;
	int number, processNumber, counter = 0;
	Tuple *tuple;
	TupleTemplate *tupleTemplate;

	for(int i = 0; i < count; i += it) {
		tupleTemplate = new TupleTemplate();
		tupleTemplate->add(TupleTemplateArg::createFromString("testString", q));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(i, q));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(process, q));
		tuple = lc->tupleRead(tupleTemplate, 5);
		if(tuple != NULL) {
			counter++;
			text = tuple->getString(0);
			number = tuple->getInteger(1);
			processNumber = tuple->getInteger(2);
			delete tuple;
			if(verbose)
				printf("%d POBRANA KROTKA:\t%s\t%d\t%d\n",process, text.c_str(), number, processNumber);
		}
		else {
			if(verbose)
				printf("%d Nie udalo sie pobrac krotki:\ttestString\t%d\t%d\n",process, i, process);
		}
		delete tupleTemplate;
	}
	return counter;
}

int inputTuples(LindaComm *lc, int count, Quantifier q, bool verbose, int it) {
	printf("%d ** Przegladanie krotek z usuwaniem: %d **\n", process, count/it);
	string text;
	int number, processNumber, counter = 0;
	Tuple *tuple;
	TupleTemplate *tupleTemplate;

	for(int i = 0; i < count; i += it) {
		tupleTemplate = new TupleTemplate();
		tupleTemplate->add(TupleTemplateArg::createFromString("testString", q));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(i, q));
		tupleTemplate->add(TupleTemplateArg::createFromInteger(process, q));
		tuple = lc->tupleInput(tupleTemplate, 5);
		if(tuple != NULL) {
			counter++;
			text = tuple->getString(0);
			number = tuple->getInteger(1);
			processNumber = tuple->getInteger(2);
			delete tuple;
			if(verbose)
				printf("%d USUWANA KROTKA:\t%s\t%d\t%d\n",process , text.c_str(), number, processNumber);
		}
		else {
			if(verbose)
				printf("%d Nie udalo sie pobrac krotki:\ttestString\t%d\t%d\n",process, i, process);
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

ifstream::pos_type filesize(const char *filename) {
	ifstream in(filename, ifstream::in | ifstream::binary);
	in.seekg(0, ifstream::end);
	return in.tellg();
}
