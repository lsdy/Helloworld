//============================================================================
// Name        : Helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <leveldb/db.h>
#include <iostream>

using namespace leveldb;
using std::string;
using std::cout;

int main(int argc, char **argv) {
	DB *db=NULL;
	Status s;
	Options options;
	options.create_if_missing=true;
	s=DB::Open(options,"TestDB",&db);
	if(!s.ok()){
		exit(1);
	}
	for(int i=0;i<10000;i++){
		db->Put(WriteOptions(),std::to_string(i),"dadsad");
	}
	string value;
	db->GetProperty("leveldb.approximate-memory-usage",&value);
	cout<<value<<"\n";
	delete db;
}

