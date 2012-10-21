#include "DB_Functions.h"
#include "Utility Functions.h"
#include "Shared Functions.h"
#include <iostream>

extern Database dbase;

// Commands **************************************************************************/
void open(string file_name){
	file_name += ".db"; //append file type (.db)
	string current_line; //string containing current line text
	int number_of_lines = 0;

	ifstream number_counter (file_name.c_str()); //Get file with name: file_name
	if (number_counter.is_open()){ // counts the number of lines in the .db file
		while (number_counter.good()){
			getline (number_counter, current_line);
			number_of_lines++;
		}
		number_of_lines--;
	}
	
	ifstream relation_file (file_name.c_str()); //Get file with name: file_name
	if (relation_file.is_open()){
		cout<<"File open successful"<<endl;
		cout<<"Placing table into virtual memory..."<<endl;
		
		int i = 0; //line #
		vector<int> column_length;
		vector<string> column_names;
		vector<bool> is_column_primary;
		vector<Relation::entry> type;
		vector<string> values;

		string table_name;
		int number_of_columns = 0;
		int current_column_number = 0;

		while (relation_file.good()){
			getline (relation_file, current_line);
			if (i == 0){
				table_name = current_line;
			}
			if (i == 1){ //get table name
				column_length.push_back(0);
				for (int j = 0; j<current_line.length(); j++){
					if (current_line[j] == '-'){
						column_length[current_column_number]++;
					}
					else {
						column_length.push_back(0);
						current_column_number++;
					}
				}
				number_of_columns = column_length.size()-1;
			}
			if (i == 2){ //get column names
				string current_column_name;
				current_column_number = 0;
				for (int j = 0; j <number_of_columns; j++){
					for (int k = current_column_number; k<current_line.length(); k++){
						if (current_line[k] != ' ' && current_line[k] != '|'){
							current_column_name += current_line[k];
						}
						else {
							column_names.push_back(current_column_name);
							current_column_name = "";
							break;
						}
					}
					current_column_number += column_length[j]+1;
				}
			}
			if (i >= 4 && i < number_of_lines-4){ //get table values
				string current_table_value;
				current_column_number = 0;
				string all_line_values;
				for (int j = 0; j <number_of_columns; j++){
					for (int k = current_column_number; k<current_line.length(); k++){
						if (current_line[k] != ' ' && current_line[k] != '|'){
							current_table_value += current_line[k];
						}
						else {
							all_line_values += current_table_value;
							all_line_values += ", ";
							current_table_value = "";
							break;
						}
					}
					current_column_number += column_length[j]+1;
				}
				all_line_values.resize(all_line_values.size()-2);
				values.push_back(all_line_values);
			}

			if (i == number_of_lines-3){ // varchar || integer
				current_column_number = 0;
				for (int j = 0; j <number_of_columns; j++){
					if (current_line[current_column_number] == 'V'){
						type.push_back(Relation::VARCHAR);
					}
					if (current_line[current_column_number] == 'I'){
						type.push_back(Relation::INTEGER);
					}
					current_column_number += column_length[j]+1;
				}
			}

			if (i == number_of_lines-2){ //is primary or not
				current_column_number = 0;
				for (int j = 0; j <number_of_columns; j++){
					if (current_line[current_column_number] == 'Y'){
						is_column_primary.push_back(true);
					}
					if (current_line[current_column_number] == 'N'){
						is_column_primary.push_back(false);
					}
					current_column_number += column_length[j]+1;
				}
			}
			i++;
		}

		Relation r(table_name); //create table called, r, to push_back into dbase
		for (int i = 0; i<number_of_columns; i++){ //creates the necessary columns
			r.addColumn(column_names[i], type[i], column_length[i], is_column_primary[i]);
		}
		
		for (int i = 0; i<values.size(); i++){ //fills the table in memory with all the values
			r.insertInto(values[i]);
		}
		dbase.add(r,table_name); // finally adds table to vector <Relation> db
	}
	else {
		cout << "Unable to find file, "<<file_name<<endl;
	}
	relation_file.close();
}

void close(string table_name){
	if (dbase.relation_exists(table_name)){
		int location;
		location = dbase.get_relation_position(table_name);
		write(*dbase.getRelation(table_name)); //write to file before closing
		dbase.remove(location);
		cout<< "Table successfully removed from memory"<<endl;
	}
	else {
		cout<<"This relation doesn't exist"<<endl;
	}
}

void write(Relation table){
	string file_name;
	file_name += table.getTableName();
	file_name += ".db";
	cout<<"file name = "<<file_name<<endl;

	ofstream relation_file (file_name.c_str());
	if (relation_file.is_open()) {
		cout<<"Writing to file..."<<endl;
		int tablelength = 0; // stores entire table's length
		int columns = table.attributes.size(); // stores the number of columns
		int rows = table.getRows(); // stores the number of rows

		//Line 1: Table name
		relation_file << table.getTableName()<<"\n";

		//Line 2: Size
		for (int i=0; i<columns; i++){
			for (int j=0; j<table.sizes[i]; j++){
				relation_file << '-';
			}
			tablelength += table.sizes[i];
			relation_file << '+';
			tablelength ++;
		}
		relation_file << "\n";

		//Line 3: Attributes
		for (int i=0; i<columns; i++){
			relation_file << table.attributes[i];
			int wordlength = table.attributes[i].length();
			int columnlength = table.sizes[i];
			for (int j=wordlength; j<columnlength; j++){
				relation_file << ' ';
			}
			relation_file << '|';
		}
		relation_file << "\n";

		//Line 4: Skip "====="
		for (int i=0; i<tablelength; i++){
			relation_file << '=';
		}
		relation_file << "\n";

		rows = table.relation[0].size();
	
		//Line 5-X: Entries
		for (int i=0; i<rows; i++){
			for (int j=0; j<columns; j++){
				relation_file << table.relation[j][i];
				int columnlength = table.sizes[j];
				int wordlength = table.relation[j][i].length();
				for (int k=wordlength; k<columnlength; k++){
					relation_file << ' ';
				}
				relation_file << '|';
			}
			relation_file << "\n";
		}
	
		//Line X+1: Skip "====="
		for (int i=0; i<tablelength; i++){
			relation_file << '=';
		}
		relation_file << "\n";
	
		//Line X+2: Type
		for (int i=0; i<columns; i++){
			int columnlength = table.sizes[i];
			if (table.types[i] == 1) {
				relation_file << "VARCHAR";
				for (int j=7; j<columnlength; j++){
					relation_file << ' ';
				}
			}
			else {
				relation_file << "INTEGER";
				for (int j=7; j<columnlength; j++){
					relation_file << ' ';
				}
			}
			relation_file << '|';
		}
		relation_file << "\n";

		//Line X+3: Primary
		for (int i=0; i<columns; i++){
			int columnlength = table.sizes[i];
			if (table.isPrimary[i] == true){
				relation_file << 'Y';
				for (int j=1; j<columnlength; j++){
					relation_file << ' ';
				}
				relation_file << '|';
			}
			else {
				relation_file << 'N';
				for (int j=1; j<columnlength; j++){
					relation_file << ' ';
				}
				relation_file << '|';
			}
		}
		relation_file << "\n";

		//Line X+4: End
		for (int i=0; i<tablelength; i++){
			relation_file << '=';
		}
		relation_file << "\n";
		relation_file << "*end*";
	}
	else {
		cout << "Unable to open file";
	}
	relation_file.close();
	cout<<"Writing to file completed"<<endl;
}

void show (Relation table){
	int tablelength = 0; // stores entire table's length
	int columns = table.attributes.size(); // stores the number of columns
	int rows = table.getRows(); // stores the number of rows

	cout<<endl;
	cout<<"Printing out table: "<<table.getTableName()<<endl;
	
	//Line 1: Table name
	cout<<table.getTableName()<<endl;

	//Line 2: Size
	for (int i=0; i<columns; i++){
		for (int j=0; j<table.sizes[i]; j++){
			cout<<'-';
		}
		tablelength += table.sizes[i];
		cout<<'+';
		tablelength ++;
	}
	cout<<endl;

	//Line 3: Attributes
	for (int i=0; i<columns; i++){
		cout<<table.attributes[i];
		int wordlength = table.attributes[i].length();
		int columnlength = table.sizes[i];
		for (int j=wordlength; j<columnlength; j++){
			cout<<' ';
		}
		cout<<'|';
	}
	cout<<endl;

	//Line 4: Skip "====="
	for (int i=0; i<tablelength; i++){
		cout<<'=';
	}
	cout<<endl;
	rows = table.relation[0].size();
	//Line 5-X: Entries
	for (int i=0; i<rows; i++){
		for (int j=0; j<columns; j++){
			cout<<table.relation[j][i];
			int columnlength = table.sizes[j];
			int wordlength = table.relation[j][i].length();
			for (int k=wordlength; k<columnlength; k++){
				cout<<' ';
			}
			cout<<'|';
		}
		cout<<endl;
	}
	//Line X+1: Skip "====="
	for (int i=0; i<tablelength; i++){
		cout<<'=';
	}
	cout<<endl;
	
	//Line X+2: Type
	for (int i=0; i<columns; i++){
		int columnlength = table.sizes[i];
		if (table.types[i] == 1) {
			cout<<"VARCHAR";
			for (int j=7; j<columnlength; j++){
				cout<<' ';
			}
		}
		else {
			cout<<"INTEGER";
			for (int j=7; j<columnlength; j++){
				cout<<' ';
			}
		}
		cout<<'|';
	}
	cout<<endl;

	//Line X+3: Primary
	for (int i=0; i<columns; i++){
		int columnlength = table.sizes[i];
		if (table.isPrimary[i] == true){
			cout<<'Y';
			for (int j=1; j<columnlength; j++){
				cout<<' ';
			}
			cout<<'|';
		}
		else {
			cout<<'N';
			for (int j=1; j<columnlength; j++){
				cout<<' ';
			}
			cout<<'|';
		}
	}
	cout<<endl;

	//Line X+4: End
	for (int i=0; i<tablelength; i++){
		cout<<'=';
	}
	cout<<endl;
	cout<<"*end*"<<endl;
	
}

// Database Class Functions **********************************************************/
int Database::getSize() {
	return db.size();
}

void Database::add(Relation table, string relation_name) {
	bool current = false;
	for (int i=0; i<db.size(); i++) {
		if(db[i].getTableName() == relation_name) {
			current = true;
			break;
		}
	}
	if (!current) {
		db.push_back(table);
	} else {
		cerr<<"Relation currently in database"<<endl;
	}
}

void Database::remove (int vector_position){
	db.erase(db.begin()+vector_position);
}

bool Database::relation_exists(string relation_name){
	for (int i=0; i<db.size(); i++) {
		if(db[i].getTableName() == relation_name) {
			return true;
		}
	}
	return false;
}

int Database::get_relation_position(string relation_name){
	for (int i=0; i<db.size(); i++) {
		if(db[i].getTableName() == relation_name) {
			return i;
			break;
		}
	}
}

Relation Database::dProduct(string t1, string t2) {
	Relation table1(return_atomic(t1, "blah"));
	Relation table2(return_atomic(t2, "bleh"));
	if (dbase.relation_exists(table1.getTableName()) && dbase.relation_exists(table2.getTableName())) {
		string row1, row2;
		Relation prod("Null");
		
		// create columns
		for(int i=0; i<table1.numColumns(); i++) {
			prod.addColumn(table1.getColumnName(i), table1.getColumnType(i), table1.getColumnSize(i), table1.getColumnPrimary(i)); 
		}
		for(int i=0; i<table2.numColumns(); i++) {
			prod.addColumn(table2.getColumnName(i), table2.getColumnType(i), table2.getColumnSize(i), table2.getColumnPrimary(i)); 
		}
		
		// insert values
		for(int i=0; i<table1.getRows(); i++) {
			row1 += table1.getRow(i);
			for(int j=0; j<table2.getRows(); j++) {
				row2 += ", ";
				row2 += table2.getRow(j);
				prod.insertInto(row1+row2);
				row2 = "";
			}
			row1 = "";
		}
		return prod;
	} else {
		cerr<<"Relation not in database"<<endl;
	}
}

Relation Database ::dProject(string c) {
	int i = 0;
	int currentColumn = 0;
	string att, atomE;
	vector<string> attributes;
	while (c[i] != ')') {
		if (c[i] ==',') {
			attributes.push_back(att);
			att = "";
			i ++;
		} else if (isalnum(c[i]) || c[i] == '_') {
			att += c[i];
			i++;
		} else {
			i++;
		}
	}
	attributes.push_back(att);
	i = skipWS(c,i+1);
	while (i < c.length()) {
		atomE += c[i];
		i++;
	}
	cout<<"atomic expr = "<<atomE<<endl;
	Relation proj("tempName");
	Relation copy(return_atomic(atomE, "tempName"));
	cout<<"Good 1: copy.getRows() = "<<copy.getRows()<<endl;
	if (attributes.size() > copy.numColumns()) {
		cerr<<"Invalid number of attributes to select; returning entire table"<<endl;
		return copy;
	} else {
		cout<<"Good 2"<<endl;
		for(i=0; i<copy.numColumns(); i++) {
			for(int j=0; j<attributes.size(); j++) {
				if (attributes[j] == copy.getColumnName(i)) {
					proj.addColumn(copy.getColumnName(i), copy.getColumnType(i), copy.getColumnSize(i), copy.getColumnPrimary(i));
					for(int k=0; k<copy.getRows(); k++) {
						cout<<"i.k: "<<i<<"."<<k<<endl;
						proj.addValue(copy.getValue(i,k),currentColumn);
					}
					currentColumn++;
				}
			}
			cout<<"Good"<<endl;
		}
	}
	if (currentColumn == 0) {
		cout<<"Returning empty table"<<endl;
	}
	cout<<"Good 5"<<endl;
	return proj;	
	// name type size prim
}

Relation Database::justGetRelation(string relation_name) {
	for (int i=0; i<db.size(); i++) {
		if(db[i].getTableName() == relation_name) {
			return db[i];
			break;
		}
	}
}

Relation Database::selection(Relation a, string c, string rel) {

	int i = 0;
	bool isInt = false;
	string attribute, op, comparison;
	if (true) {
		Relation* r = dbase.getRelation(a.getTableName());
		for(; i<c.length() && isalnum(c[i]); i++){
			attribute += c[i];
		}
		i = skipWS(c,i);
		for(; i<c.length() && ispunct(c[i]); i++){
			op += c[i];
		}
		i = skipWS(c,i);
		for(; i<c.length() && (isalnum(c[i]) || c[i] == '"'); i++){
			comparison += c[i];
		}
		if (comparison[0] == '"') {
			comparison = comparison.substr(1,comparison.length()-2);
		} else {
			isInt = true;
		}
		string x = r->getTableName();
		int col = r->getColumnNumber(attribute);
		Relation copy(*r, "selection");
		copy.selectFrom(c);
		if (x == rel){
			dbase.remove(dbase.get_relation_position(rel));
			return copy;
		}
		else {
			return copy;
		}
	}
}

void Database::print_out_all_table_names(){
	for (int i=0; i < dbase.db.size(); i++) {
		cout<<dbase.db[i].getTableName()<<endl;
	}
}
