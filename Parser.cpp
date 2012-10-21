#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "Parser.h"

extern Database dbase;

void parse(string c) {
	if (query(c)) { // check to see if input is a query
	} else if (command(c)) { // check to see if input is a command
	} else {
		cout<<"INVALID!"<<endl; // parser fails
	}
}

bool query(string c) {
	int i = 0;
	string rel, ex;
	if (c[c.length()-1] == ';') {
		if (isalpha(c[i])) {
			rel += c[i];
			i++;
			while (isalnum(c[i]) || c[i] == '_') { 
				rel += c[i];
				i++; 
			}
			i = skipWS(c,i);
			if (c.substr(i,2) == "<-") {
				i = skipWS(c,i+2);
				while (i <c.length()-1) {
					ex += c[i];
					i++;
				}
				//cout<<"Rel: "<<rel<<endl;
				expr(ex, rel);
				return true;
			} else {
				return false;
				cerr<<"No '<-' sign after relation name"<<endl;
			}
		} else {
			return false;
			cerr<<"No relation name"<<endl;
		}			
	} else {
		return false;
		cerr<<"No semicolon"<<endl;
	}
}

bool command(string c) {
	pos1 = 0;
	//Convert all characters into lowercase and push them into the string, cmd
	for (int j=0; j<c.length(); ++j) {
		c[j]=tolower(c[j]);
	}
	
	string cmd; //check the first substring of the input
	for (int j=0; j<c.length(); ++j) {
		if (isalnum(c[j])) {
			cmd.push_back(c[j]);
			pos1 = j;
		}
		else{
			break;
		}
	}

	//***************************************
	//Check to see if cmd is OPEN/CLOSE/WRITE
	if (cmd == "open" || cmd == "close" || cmd == "write" ){
		//check to see if OPEN/CLOSE/WRITE are followed by a relation-name and a ';'
		if (relation_name_check(c, pos1) &&
			!end_of_input_reached(c,position1()) &&
			c[position1()+1]==';'){
				cout<<"This is a VALID command"<<endl;

				//create string to hold the name of the relation for execution function calls
				string newRelation = c.substr(pos1+2,position1()-pos1-1);

				//execute OPEN
				if (cmd == "open"){
					open(newRelation);
				}

				//execute CLOSE
				if (cmd == "close"){
					close(newRelation);
				}

				//execute WRITE
				if (cmd == "write"){
					if (dbase.relation_exists(newRelation)){
						write(*dbase.getRelation(newRelation));
					}
					else {
						cout<<"This relation doesn't exist"<<endl;
					}
				}
				return true;
		}
		else {
			return false;
		}
	}
	//***************************************
	//Check to see if cmd is EXIT
	else if (cmd == "exit"){
		//check to see if EXIT is followed by a ';'
		if (!end_of_input_reached(c,pos1) &&
			c[pos1+1]==';'){
				cout<<"This is a VALID command"<<endl;
				exit (1);
				return true;
		}
		else {
			return false;
		}
	}
	//***************************************
	//Check to see if cmd is CREATE TABLE
	else if (cmd == "create"){
		if (c.length()>=13 &&
			c.substr (0,13) == "create table " && //checks for "create table "
			relation_name_check(c, pos1+6) && //checks to see if followed by a relation name
			//check to see if followed by " ("
			!end_of_input_reached(c,position1()) && 
			c[position1()+1] == ' ' &&
			!end_of_input_reached(c,position1()+1) &&
			c[position1()+2] == '(' &&
			!end_of_input_reached(c,position1()+2)){
				// store the name of the relation
				string newRelation = c.substr(pos1+8,position1()-pos1-7);
				pos1 = position1()+3;
				int i = pos1;
				string typedattr;
				if (!end_of_input_reached(c,i)){
					for (int j=i; j<c.length(); ++j) {
						if (c[j] == ')' &&
							!end_of_input_reached(c,j) &&
							c[j+1] == ' '){
								break;
						}
						else{
							typedattr.push_back(c[j]);
							i++;
						}
					}
					i--; //i was incremented 1 too many by for loop
				}
				else {
					return false;
				}

				if (typedAttribute(typedattr)) {
					//check for ") PRIMARY KEY "
					pos1 = i;
					if (!end_of_input_reached(c,pos1) &&
						c.substr (pos1+1,15) == ") primary key ("){
							// put whatever is in the last () into string, attr
							i = pos1+1+14;
							string attr;
							if (!end_of_input_reached(c,i)){
								i++;
								for (int j=i; j<c.length(); ++j) {
									if (c[j] != ')') {
										attr.push_back(c[j]);
										i++;
									}
									else{
										break;
									}
								}
								i--; //i was incremented 1 too many by for loop
							}
							if (attribute(attr)){
								pos1 = i;
								//cout<<"creating..."<<endl;
								//cout<<"pos1 = "<<pos1<<endl;
								if (!end_of_input_reached(c,pos1) &&
									c[pos1+1] == ')' &&
									!end_of_input_reached(c,pos1+1) &&
									c[pos1+2] == ';'){
										//cout<<"This is a VALID command"<<endl;
										//Creates a table with name = newRelation
										Relation r(newRelation);
										vector<vector<string> > a;
										vector<string> vietnam;
										int x = 0;
										string name, type, size;
										while (x < typedattr.length()) {
											while (isalnum(typedattr[x]) || typedattr[x] == '_') {
												name += typedattr[x];
												x++;
											} 
											x = skipWS(typedattr,x);
											if (typedattr.substr(x,8) == "varchar(") {
												x+=8;
												// add varchar to list...
												while (isdigit(typedattr[x])) {
													size += typedattr[x];
													x++;
												}
												x+=2; // close paren -> comma -> space
												r.addColumn(name, Relation::VARCHAR, (atoi(size.c_str())), false);
											} else if (typedattr.substr(x,7) == "integer") {
												// add integer and default integer size
												r.addColumn(name, Relation::INTEGER, 8, false);
												x+=8;
											}
											name = "";
											size = "";
											x++;
											
										}
										
										for(int y=0; y < attr.length(); y++) {
											if (isalnum(attr[y]) || attr[y] == '_') {
												name += attr[y];
												if (y == attr.length()-1){
													r.setPrimary(name);
												}
											} else if (attr[y] == ',') {
												r.setPrimary(name);
												name = "";
											}
										}
										
										dbase.add(r,newRelation);
										cout<<"Table Successfully Created"<<endl;
										return true;
								}
								else {
									return false;
								}
							}
							else {
								return false;
							}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
		}
		else {
			return false;
		}
	}

	//Check to see if cmd is INSERT INTO
	else if (cmd == "insert"){
		pos1 = 0;
		//*Check both cases of INSERT INTO; VALUES FROM & VALUES FROM RELATION*
		//Check to see if INSERT INTO is followed by a relation-name and then by VALUES FROM
		if (c.length()>=11 &&
			c.substr (0,11) == "insert into" &&
			relation_name_check(c, pos1+10) &&
			!end_of_input_reached(c,position1()) &&
			c.substr(position1()+1,14) == " values from (" &&
			!end_of_input_reached(c,position1()+1+13)){
				string newRelation;
				newRelation = c.substr(pos1+12,position1()-pos1-11);
				pos1 = position1()+1+13;
				//check to see if VALUES FROM is followed by an '('
				if (!end_of_input_reached(c,pos1)){
					//check to see if enclosed is a VALID literal (or list of literals) followed by a ')'
					if (is_literal(c,pos1+1)){
						string attributes;
						attributes = c.substr(pos1+1,position1()-pos1);
						if (!end_of_input_reached(c,position1()) &&
							c[position1()+1] == ')' &&
							!end_of_input_reached(c,position1()+1) &&
							c[position1()+2]==';'){
								// dbase[dbase.get_relation_position(/*name*/)]
								cout<<"This is a VALID command"<<endl;
								if (dbase.relation_exists(newRelation)){
									cout<<"Inserting..."<<endl;
									dbase.getRelation(newRelation)->insertInto(attributes);
								}
								else {
									cout<<"This relation doesn't exist"<<endl;
								}
								return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
		}
		
		//Check to see if INSERT INTO is followed by a relation-name and then by VALUES FROM RELATION
		else {
			pos1=0;
			if (c.length()>=11 &&
				c.substr (0,11) == "insert into" &&
				relation_name_check(c, pos1+10) &&
				!end_of_input_reached(c,position1()) &&
				c.substr(position1()+1,22) == " values from relation " &&
				!end_of_input_reached(c,position1()+1+21) &&
				c[c.length()-1] == ';'){
					pos1 = position1()+1+22;
					cout<<"follow this by an expr"<<endl;
					string exp;
					exp = c.substr (pos1,c.length()-pos1-1);
					cout<<exp<<endl;
					expr(exp, "Well a relation name is supposed to go here..."); // remove comments when expr works
					// dbase[dbase.get_relation_position(/*name*/)]
					//followed by ';'
			}
			else {
				return false;
			}
		}
	}

	//Check to see if command is DELETE FROM
	else if (cmd == "delete"){
		pos1 = 0;
		if (c.length()>=11 &&
			c.substr (0,11) == "delete from" &&
			relation_name_check(c, pos1+10) &&
			!end_of_input_reached(c,position1()) &&
			c.substr(position1()+1,7) == " where " &&
			!end_of_input_reached(c,position1()+1+6) &&
			c[c.length()-1] == ';'){
				string newRelation;
				newRelation = c.substr(pos1+12,position1()-pos1-11);
				pos1 = position1()+1+7;
				string cond;
				cond = c.substr (pos1,c.length()-pos1-1);
				if (condition(cond)){
					cout<<"This is a VALID command"<<endl;
					if (dbase.relation_exists(newRelation)){
						cout<<"Deleting..."<<endl;
						dbase.getRelation(newRelation)->deleteFrom(cond);
					}
					else {
						cout<<"This relation doesn't exist"<<endl;
					}
					return true;
				}
				else {
					return false;
				}
		}
		else if (c.length()>=12 &&
				c.substr (0,12) == "delete table" &&
				relation_name_check(c, pos1+11) &&
				!end_of_input_reached(c,position1()) &&
				c[position1()+1] == ';' &&
				end_of_input_reached(c,position1()+1)) {
					cout<<"This is a VALID command"<<endl;
					
					// implementation isn't necessary for the purpose of this project; but TA wanted to check this command

					return true;
		}
		else {
			return false;
		}
	}
	
	//Check to see if command is UPDATE
	else if (cmd == "update"){
		pos1 = 5;
		if (relation_name_check(c, pos1) &&
			!end_of_input_reached(c,position1()) ){
			// function finds whatever is between SET and WHERE
				string newRelation;
				newRelation = c.substr(pos1+2,position1()-pos1-1);
				string afterrelation;
				afterrelation = c.substr(position1()+1,c.find_last_of(c));
				int pos2 = position1()+6;
				if (is_update_list(afterrelation, 0)){
					if (afterrelation.substr(position1(),7) == " where " &&
						!end_of_input_reached(c,position1()+6) &&
						c[c.length()-1] == ';'){
							int pos3 = position1();
							pos1 = position1()+7;
							string cond;
							cond = afterrelation.substr (pos1,afterrelation.length()-pos1-1);
							cout<<afterrelation.length()<<endl;
							afterrelation = c.substr(pos2,afterrelation.length()-5-cond.length()-8);
							if (condition(cond)){
								cout<<"This is a VALID command"<<endl;
								if (dbase.relation_exists(newRelation)){
									cout<<"Updating..."<<endl;
									dbase.getRelation(newRelation)->update(afterrelation, cond);
								}
								else {
									cout<<"This relation doesn't exist"<<endl;
								}
								return true;
							}
							else {
								return false;
							}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
		}
		else {
			return false;
		}
	}
	
	//Check to see if command is SHOW
	else if (cmd == "show"){
		pos1 = 3;
		if (!end_of_input_reached(c,pos1) &&
			c[pos1+1] == ' ' &&
			!end_of_input_reached(c,pos1+1)) {
				
				//**This block needs to be replaced by atomic-expr for future**
				//*************************************************************
						
				//INCOMPLETE
				//atomic-expr needs to be implemented here
				//atomic(atomicexpr);
				
				//followed by ';'
				//**************

				if (relation_name_check(c, pos1) &&
					!end_of_input_reached(c,position1()) &&
					c[position1()+1]==';'){
						cout<<"This is a VALID command"<<endl;
						
						//execute SHOW
						string newRelation = c.substr(pos1+2,position1()-pos1-1);
						if (dbase.relation_exists(newRelation)){
							show(*dbase.getRelation(newRelation));
						}
						else if (newRelation == "tables"){
							cout<<"Here are all the tables: "<<endl;
							cout<<"********************"<<endl;
							dbase.print_out_all_table_names();
							cout<<"********************"<<endl;
						}
						else {
							cout<<"This relation doesn't exist"<<endl;
						}
						return true;
				}
				//*************************************************************
				else {
					return false;
				}
		}
		else {
			return false;
		}
	}

	//Anything else typed is NOT a valid command
	else {
		return false;
	}
}
// END of Command Function ***********************************************************/