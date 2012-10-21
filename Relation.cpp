#include "Relation.h"
#include <stdlib.h>

void Relation::addColumn(string name, entry type, int size, bool primary) {
	attributes.push_back (name);	//update vector of column names (attribute names)
	types.push_back(type);			//update vector of types (entry is either VARCHAR or INTEGER)
	sizes.push_back(size);			//update vector of sizes (column max length)
	isPrimary.push_back(primary);	//update vector of isPrimary (contains whether or not the column is a PRIMARY KEY)
	vector<string> col;				//create column vector
	relation.push_back(col);		//insert column into main relation vector
}

void Relation::addValue(string value, int column_number){
	relation[column_number].push_back(value);
}

void Relation::setPrimary(string name) {
	for(int i=0; i<numColumns(); i++) {
		if (name == attributes[i]) {
			isPrimary[i] = true;
			break;
		}
	} 
}

void Relation::setName(string n) {
	name = n;
}

string Relation::getTableName() {
	return name;
}
string Relation::getColumnName(int i) {
	return attributes[i];
}

Relation::entry Relation::getColumnType(int i) {
	return types[i];
}

int Relation::getColumnSize(int i) {
	return sizes[i];
}

bool Relation::getColumnPrimary(int i) {
	return isPrimary[i];
}

string Relation::getValue(int column, int row) {
	string value;
	value = relation[column][row];
	return value;
}

int Relation::numColumns() {
	return relation.size();
}

int Relation::getRows() {
	return relation[0].size();
}

void Relation::addRow() {
	rows++;
}

int Relation::getColumnNumber(string n) {
	for(int i=0; i<numColumns(); i++) {
		if (n == attributes[i]) {
			return (i);
		}
	}
	return 0;
}

void Relation::insertInto(string attributes) {
	int i = 0;
	int j;
	int col = 0; //current column
	string att;
	string att_col_counter;
	int col_counter = 0; // finds the number of columns within a relation

	for(j = 0; j < attributes.length(); ++j) {
		while(isalnum(attributes[j]) || attributes[j] == '_' || attributes[j] == '"') {
			att_col_counter += attributes[j];
			j++;
		}
		if(attributes[j] == ',' || !attributes[j]) {
			col_counter++;
		}
	}
	if(col_counter == relation.size()) {
		while(i < attributes.length()) {
			while(isalnum(attributes[i]) || attributes[i] == '_' || attributes[i] == '"') {
				att += attributes[i];
				i++;
			}
			if(attributes[i] == ',' || !attributes[i]) {
				if((isalnum(att[1]) && types[col] == Relation::VARCHAR) || 
					(isdigit(att[0]) && types[col] == Relation::INTEGER)) {
					this -> relation[col].push_back(att);
					att = "";
					col++;
					addRow();
				}
				else {
					cout<<"ENTRY TYPES INVALID"<<endl;
				}
			}
			att = "";
			i++;
		}
	}
	else { cerr<<"The number of entries does not match the number of columns"<<endl; }
}

void Relation::deleteFrom(string cond) {
    int i = 0;
    int columns;
    string attr, op, comp;
    
    for(; i < cond.length() && isalnum(cond[i]); ++i) {
        attr += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i <cond.length() && ispunct(cond[i]); ++i) {
        op += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i<cond.length() && (isalnum(cond[i]) || cond[i] == '"'); ++i) {
        comp += cond[i];
    }
    
    for(int j = 0; j < relation.size(); ++j) {
        if(attr == attributes[j]) {
            if(op == "==") {
                for(int k = 0; k < relation[j].size(); ++k) {
                    if(comp == relation[j][k]) {
                        for(int m = 0; m < relation.size(); ++m) {
                            relation[m].erase(relation[m].begin()+k);
                        }
                    }                
                }
            }
            else if(op == "!=") {
                for(int k = 0; k < relation[j].size(); ++k) {
                    if(comp != relation[j][k]) {
                        for(int m = 0; m < relation.size(); ++m) {
                            relation[m].erase(relation[m].begin()+k);
                        }
                    }
					k--;
                }
            }
            else if(op == ">=") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) >= atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == ">") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) > atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == "<=") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) <= atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == "<") {
                for(int n = 0; n < attr.length(); ++n) {
                        if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) { // a number
                        for(int k = 0; k < relation[j].size(); ++k) {	//for all rows
                            if(atoi(relation[j][k].c_str()) < atoi(comp.c_str())) { 
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
        }
    }    
}

void Relation::selectFrom(string cond) {
    int i = 0;
    int columns;
    string attr, op, comp;
    
    for(; i < cond.length() && isalnum(cond[i]); ++i) {
        attr += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i <cond.length() && ispunct(cond[i]); ++i) {
        op += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i<cond.length() && (isalnum(cond[i]) || cond[i] == '"'); ++i) {
        comp += cond[i];
    }

	//replace the operator with it's complement thus making a deleteFrom a selectFrom
	//**************************
	if(op == "==") { op = "!="; }
	else if(op == "!=") { op = "=="; }
	else if(op == ">=") { op = "<"; }
	else if(op == ">") { op = "<="; }
	else if(op == "<=") { op = ">"; }
	else if(op == "<") { op = ">="; }

	//**************************
    
    for(int j = 0; j < relation.size(); ++j) { // j = column
        if(attr == attributes[j]) { // found matching column
            if(op == "==") {
                for(int k = 0; k < relation[j].size(); ++k) { //go through column vector
                    if(comp == relation[j][k]) {
                        for(int m = 0; m < relation.size(); ++m) {
                            relation[m].erase(relation[m].begin()+k);
                        }
                    }                
                }
            }
            else if(op == "!=") {
                for(int k = 0; k < relation[j].size(); ++k) {
					cout <<"comp ="<< comp <<"; relation[j][k] ="<<relation[j][k]<<";"<<endl;
                    if(comp != relation[j][k]) {
                        for(int m = 0; m < relation.size(); ++m) {
                            relation[m].erase(relation[m].begin()+k);
                        }
						k--;
                    }
                }
            }
            else if(op == ">=") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) >= atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == ">") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) > atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == "<=") {
                for(int n = 0; n < attr.length(); ++n) {
                    if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) <= atoi(comp.c_str())) {
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
            else if(op == "<") {
                for(int n = 0; n < attr.length(); ++n) {
                        if(!isalpha(comp[n]) && !ispunct(comp[n]) && !isspace(comp[n])) { // a number
                        for(int k = 0; k < relation[j].size(); ++k) {	//for all rows
                            if(atoi(relation[j][k].c_str()) < atoi(comp.c_str())) { 
                                for(int m = 0; m < relation.size(); ++m) {
                                    relation[m].erase(relation[m].begin()+k);
                                }
                            }
                        }
                    }
                }
            }
        }
    }    
}

vector<int> Relation::found_in_these_rows(string cond) {
	int i = 0;
	int columns;
    string attr, op, comp;
	vector <int> these_rows;
    
    for(; i < cond.length() && isalnum(cond[i]); ++i) {
		attr += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i <cond.length() && ispunct(cond[i]); ++i) {
        op += cond[i];
    }
    while(isspace(cond[i])) {
        i++;
    }
    for(; i<cond.length() && (isalnum(cond[i]) || cond[i] == '"'); ++i) {
        comp += cond[i];
    }
    for(int j = 0; j < relation.size(); ++j) {
		if(attr == attributes[j]) {
			if(op == "==") {
				for(int k = 0; k < relation[j].size(); ++k) {
                    if(comp == relation[j][k]) {
						these_rows.push_back(k);
                    }                
                }
				return these_rows;
            }
            else if(op == "!=") {
                for(int k = 0; k < relation[j].size(); ++k) {
                    if(comp != relation[j][k]) {
                        these_rows.push_back(k);
                    }
                }
				return these_rows;
            }
            else if(op == ">=") {
                    if(!isalpha(comp[0]) && !ispunct(comp[0]) && !isspace(comp[0])) {
                        for(int k = 0; k < relation[j].size(); ++k) {
                            if(atoi(relation[j][k].c_str()) >= atoi(comp.c_str())) {
								these_rows.push_back(k);
                            }
                        }
                    }
				return these_rows;
            }
            else if(op == ">") {
				if(!isalpha(comp[0]) && !ispunct(comp[0]) && !isspace(comp[0])) {
					for(int k = 0; k < relation[j].size(); ++k) {
						if(atoi(relation[j][k].c_str()) > atoi(comp.c_str())) {
							these_rows.push_back(k);
						}
					}
				}
				return these_rows;
            }
            else if(op == "<=") {
				if(!isalpha(comp[0]) && !ispunct(comp[0]) && !isspace(comp[0])) {
					for(int k = 0; k < relation[j].size(); ++k) {
						if(atoi(relation[j][k].c_str()) <= atoi(comp.c_str())) {
							these_rows.push_back(k);
						}
					}
				}
				return these_rows;
            }
            else if(op == "<") {
				if(!isalpha(comp[0]) && !ispunct(comp[0]) && !isspace(comp[0])) {
					for(int k = 0; k < relation[j].size(); ++k) {	//for all rows
						if(atoi(relation[j][k].c_str()) < atoi(comp.c_str())) { 
							these_rows.push_back(k);
						}
					}
				}
				return these_rows;
            }
        }
    }
}

void Relation::update(string set, string cond) {
	vector <string> lines; // stores the list of (attribute-list strings) to be called in insertInto(attributes)
	vector <string> words; // stores the words found in each column per entry
	vector <int> these_rows;

	//break set into attribute-name and literal1
	string attribute_list;
	string attribute_name;
	string literal_name;

	for (int i = 0; i<set.length(); i++){
		if (set[i] != '=' && set[i] != ' '){ // before = sign
			attribute_name += set[i];
		}
		else { // after
			literal_name = set.substr(i+3,set.length()-i-3);
			break;
		}
	}

	//get all attribute lists from all that fulfill condition cond
	these_rows = found_in_these_rows(cond);

	//get row contents and put them in words vector
	for (int i = 0; i<these_rows.size(); i++){
		for (int j=0; j<relation.size(); j++){
			if (attributes[j] == attribute_name){
				words.push_back(literal_name);
			}
			else{
				words.push_back(relation[j][these_rows[i]]);
			}
		}
		for (int j=0; j<words.size(); j++){
			if (j != words.size()-1){
				attribute_list += words[j];
				attribute_list += ", ";
			}
			else{
				attribute_list += words[j];
			}
		}
		lines.push_back(attribute_list);
		attribute_list = "";
		words.clear();
	}

	//delete all that fulfill condition, cond
	deleteFrom(cond);

	for (int i = 0; i<lines.size(); i++){
		insertInto(lines[i]);
	}
}

string Relation::getRow(int row) {
	string result;
	for(int i=0; i<numColumns(); i++) {
		result += getValue(i, row);
		result += ", ";
	}
	return result.substr(0,result.length()-2);
}
