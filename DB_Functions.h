#ifndef DB_FUNCTIONS_H
#define DB_FUNCTIONS_H

#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include "Relation.h"

//Command functions

void write (Relation table);
void open (string file_name);
void close (string table_name);
void show (Relation table);
void insertInto(string table, vector<string> entries);

class Database {
	private:
		vector<Relation> db;
	public:
		Database() { //constructor
			db.clear();
		}
		~Database() { //destructor
			db.clear();
		}
		void add(Relation rel) {
			db.push_back(rel);
		}
		Relation* getRelation(string name) { //returns pointer to the address of relation with name in the vector<Relation>db
			int k=0;
			for (std::vector< Relation >::iterator i=db.begin();
				i!=db.end(); ++i) {
					if (i->getName().compare(name)==0) {
						return &db[k];
					}
					k++;	
			}
		}
		
		Relation dProduct(string table1, string table2); // creates a new table that is the cross product of table 1 & 2
		Relation dProject(string c);

		int getSize (); // gets the size of vector<Relation> db
		void add (Relation table, string relation_name); //adds relation, table, with name, relation_name, into db
		void remove (int vector_position); //removes relation from db at position, vector_position

		bool relation_exists(string relation_name); // returns true if a relation of name relation_name is found in db
		int get_relation_position(string relation_name);
		Relation justGetRelation(string relation_name);

		Relation selection(Relation a, string c, string rel);
		//Relation projection(string attList, string atom);

		void print_out_all_table_names();
};

#endif
