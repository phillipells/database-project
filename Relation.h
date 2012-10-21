#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>

using namespace std;

class tuple {
	private:
		vector<string>data;
	public:
		tuple(){
			data.clear();
		}
		~tuple(){
			data.clear();
		}

		void add(string dat) {
			data.push_back(dat);
		}

		string get(int i) {
			data[i];
		}

		void print() {
			cout << "tuple: ";
			for (std::vector< string >::iterator i=data.begin();
				i!=data.end(); ++i) {
					cout << *i << " ";
			}
			cout << endl;
		}

};  

class Relation {
	private:
		string name; // table name
		vector<tuple> table;

		int columns;
		int rows;

	public:
		enum entry { INTEGER, VARCHAR }; // "storage" vector
		vector<vector<string> > relation; // stores everything
		
		// "informative" vectors
		vector<string> attributes;	// column names
		vector<entry> types;		// will help specify since everything is a string
		vector<int> sizes;			// string sizes (0 or not applicable to integers)
		vector<bool> isPrimary;		// true for primary key columns	
		
		Relation (string name){ // constructor
			this->name = name;
			table.clear();
		}

		~Relation (){			//destructor
			table.clear();
		}
		void add(tuple t){
			table.push_back(t);
		}
		void print(){
			cout <<"RELATION [" <<name<<"]"<<endl;
			for (std::vector< tuple >::iterator i=table.begin();
				i!=table.end(); ++i) {
					cout << i-table.begin() << " ";
					i->print();
			}
		}
		
		Relation(const Relation& p, string name) {
			this->name = name;
			//name = p.name;
			table = p.table;
			columns = p.columns;
			rows = p.rows;
			relation = p.relation;
			attributes = p.attributes;
			types = p.types;
			sizes = p.sizes;
			isPrimary = p.isPrimary;
		}
			
		string getName(){
			return name;
		}
		
		// functions **************************
		void addColumn(string name, entry type, int size, bool primary);
		void addValue(string value, int column_number);
		void setPrimary(string name);
		void setName(string n);
		void insertInto(string attributes);
		void deleteFrom(string cond);
		void selectFrom(string cond);
		vector <int> found_in_these_rows (string cond);
		void update(string set, string cond);
		void addRow();
		
		string getColumnName(int i);			// for testing
		string getTableName();					// returns table name
		string getValue(int column, int row);	// returns table string
		string getRow(int row);
		
		int getColumnSize(int i);
		entry getColumnType(int i);
		bool getColumnPrimary(int i);
		
		int numColumns();	// returns the number of columns
		int getRows();		// returns the number of rows
		int getColumnNumber(string n);
		//*************************************
};

#endif