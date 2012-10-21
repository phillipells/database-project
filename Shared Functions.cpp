#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "Shared Functions.h"
#include "DB_Functions.h"

extern Database dbase;

// expression (will not be pointless when finished)
void expr(string c, string r) {
	if (selection(c, r)) {
		// run selection
		//cout<<"r = "<<r<<endl;
		dbase.add(return_expr(c,r),r);
		// rel.setTableName(r);
	} else if (projection(c,r)) {
		int i = 0;
		while (c[i] != '(') {
			i++;
		}
		i++;
		Relation proj(dbase.dProject(c.substr(i,c.length()-i)));
		proj.setName(r);
		cout<<"Adding "<<proj.getTableName()<<" to database..."<<endl;
		dbase.add(proj, r);
		cout<<"Added"<<endl;
	} else if (rename_(c,r)) {
		// rename
	} else if (product(c)) {
		int i = 0;
		string t1, t2;
		while (isalpha(c[i]) || c[i] == '_') {
			t1 += c[i];
			i++;
		}
		i = skipWS(c,i);
		i++;
		i = skipWS(c,i);
		while (isalpha(c[i]) || c[i] == '_') {
			t2 += c[i];
			i++;
		}
		Relation prod(dbase.dProduct(t1,t2));
		prod.setName(r);
		dbase.add(prod);
		cout<<r<<" added to database"<<endl;
	} else {
		 cerr<<"Invalid expression"<<endl;
	}
}

bool atomic(string c, string r) {
	if (isalpha(c[0])) {
		return relationName(c);
	} else if (c[0] == '(') {
		return pExpr(c,r);
	} else {
		cout<<"Fail"<<endl;
		return false;
		// fail
	}
}

bool relationName(string c) {
	int i = 1; // first char is already parsed
	string rel;
	rel += c[0];
	while (isalnum(c[i]) || c[i] == '_') {
		rel += c[i];
		i++;
	}
	i = skipWS(c,i);
	if(isgraph(c[i])) {
		cerr<<"Invalid relation name"<<endl;
		return false;
	} else {
		if (dbase.relation_exists(rel)) {
			return true;			
		} else {
			cerr<<"Relation: "<<rel<<" does not exist in database"<<endl;
			return false;
		}
	}
}

//****Daniel*********************************************//
//Checks to see if a space and a relation_name follows; updates pos
bool relation_name_check(string c, int pos){
	//End of input string - FAIL
	if (end_of_input_reached(c,pos)){
		return false;
	}
	else {
		//If not followed by a space - FAIL
		pos++;
		if (c[pos] != ' ') {
			return false;
		}
		else {
			//If ONLY followed by a space - FAIL
			if (end_of_input_reached(c,pos)){
				return false;
			}
			else {
				pos++;
				//If then followed by an non-alphanum char - FAIL
				if (!isalnum(c[pos])) {
					return false;
				}
				else {
					string relation_name;
					for (int j=pos; j<c.length(); ++j) {
						if (isalnum(c[j])) {
							relation_name.push_back(c[j]);
							pos1 = j;
						}
						else {
							if (!end_of_input_reached(c,pos1) &&
								c[pos1+1] == '_' ){
									relation_name.push_back(c[pos1+1]);
									if (end_of_input_reached(c,pos1+1)) {
										return false;
									}
							}
							else {
								break;
							}
						}
					}
					return true;
				}
			}
		}
	}
}
//*******************************************************//

bool pExpr(string c, string r) { //expr surrounded by ( ) 
	//cout<<"r, pExpr = "<<r<<endl;
	int i = 1, paren = 1;
	string expression;
	while(paren != 0 && !end_of_input_reached(c,i)) {
		if (c[i] == '(') {
			expression += c[i];
			paren++;
		} else if (c[i] == ')') {
			if (paren > 1) {
				expression += c[i];
			}
			paren--;
		} else {
			expression += c[i];
		}
		i++;
	}
	if (end_of_input_reached(c,i+1) && !isspace(c[i])) {
		cerr<<"Invalid: expr (junk after expr))"<<endl;
		return false;
		// 
	} else {
		expr(expression,r);
		return true;
	}
}

// selection
bool selection(string c, string r) {
	int i = 0;
	string cond, atomicExpr;
	if (c.substr(0,7).compare("select ") == 0) {
		i = skipWS(c,7);
		if (c[i] == '(') {
			i++;
			while(c[i] != ')' && i<c.length()) { 
				cond += c[i]; 
				i++; 
			}
			i = skipWS(c,i+1);
			while(i<c.length()) { 
				atomicExpr += c[i]; 
				i++; 
			}
			if(condition(cond) && atomic(atomicExpr,r)) {
				return true;
			} else {
				cout<<"Invalid: Selection"<<endl;
			}
		} else {
			cerr<<"invalid syntax: no ( for condition"<<endl;
		}
	} else { 
		return false; 
	}
	return true;
}

//****Phillip****//
bool projection(string c, string r) {
	int i = 0;
	int paren = 0;
    string attList;
	string expression;
	if(c.substr(0,8).compare("project ") == 0) {
        i = skipWS(c,8);
		if(c[i] == '(') {
			i++;
			while(c[i] != ')') { 
                attList += c[i]; 
                i++;
            }
			i = skipWS(c, i+1);
			if (i != c.length() && atomic(c.substr(i,c.length()-i),r) && attribute(attList)) {
				cout<<"Valid: Projection"<<endl;
			} else {
				cerr<<"Invalid: Projection (No atomic expression)"<<endl;
			}
        } else { 
			cerr<<"Invalid: Projection (No attribute list)"<<endl;
		}
	} else {
		return false;
	}
	return true;
}
//****Phillip****//

bool rename_(string c, string r) {
	int i = 0;
	int paren = 0;
	string attList;
	string expression;
	if (c.substr(0,7) == "rename ") {
        i = skipWS(c,7);
		if(c[i] == '(') {
			i++;
			while(c[i] != ')') { 
                attList += c[i]; 
                i++;
            }
			i = skipWS(c, i+1);
			if (i != c.length() && atomic(c.substr(i,c.length()-i),r) && attribute(attList)) {
				cerr<<"Valid: Rename"<<endl;			
			} else {
				cerr<<"Invalid Rename: No atomic expression"<<endl;
			}
        } else { 
			cerr<<"Invalid Rename: No attribute list"<<endl; 
		}
	} else {
		return false;
	}
	return true;
}

bool product(string c) {
	int i = 0;
	string atom1, atom2;
	char op;
	if (isalpha(c[i])) {
		atom1 += c[i];
		i++;
		while(isalnum(c[i]) || c[i] == '_') {
			atom1 += c[i];
			i++;
		}
		i = skipWS(c,i);
		if (ispunct(c[i])) {
			op = c[i];
			i++;
			i = skipWS(c,i);
			if (isalpha(c[i])) {
				atom2 += c[i];
				i++;
				while(isalnum(c[i]) || c[i] == '_') {
					atom2 += c[i];
					i++;
				}
				switch (op) {
					case '*':
						break;
					case '+':
						cout<<"Valid: Union"<<endl;
						return false;
						break;
					case '-':
						cout<<"Valid: Difference"<<endl;
						return false;
						break;
					default:
						return false;
				}
			} else { 
				cerr<<"Invalid Product: Bad  2nd operator"<<endl;
			}
		} else {
			cerr<<"Bad operator"<<endl;
		}
		return true;
	} else {
		cerr<<"Invalid query"<<endl; 
		return false;
	}
}

// condition
bool condition(string c) {
	int i = 0;
	string conj;
	bool meta = false;
	
	while (c[i] != '|' && i<c.length()) {
		conj += c[i];
		i++;
		if(c.substr(i,2) == "||") {
			i = skipWS(c,i+2);
			cout<<"\""<<c.substr(i,c.length())<<"\""<<" got in"<<endl;
			meta = condition(c.substr(i,c.length()));
		}
	}
	return (conjunction(conj) || meta);
}

// conjunction
bool conjunction(string c) {
	int i = 0;
	string comp;
	bool meta = true;
	
	while (c[i] != '&' && i<c.length()) {
		comp += c[i];
		i++;
		if(c.substr(i,2) == "&&") {
			i = skipWS(c,i+2);
			cout<<"\""<<c.substr(i,c.length())<<"\""<<" got_in"<<endl;
			meta = condition(c.substr(i,c.length()));
		}
	}
	return (comparison(comp) && meta);
}

// comparison
 bool comparison(string c) {
	int i = 0;
	string op1, op2, op;
	i = skipWS(c,i);
	while (isalnum(c[i]) || c[i] == '_') {
		op1 += c[i];
		i++;
	}
	i = skipWS(c,i);
	while (ispunct(c[i]) && c[i] != '"') {
		op += c[i];
		i++;
	}
	i = skipWS(c,i);
	if(c[i] != '"') {		
	    if(isdigit(c[i])) {
	        while(isdigit(c[i])) {
                    op2 += c[i];
                    i++;
                }
	        return intComp(op1,op2,op);
            }
            else { cerr<<"bad syntax: no quotes"<<endl; }
        }
	else {
		i++;
	while (isalnum(c[i]) || c[i] == '_') {
		op2 += c[i];
		i++;
	}
	return intComp(op1,op2,op);
        }
        return false;
}

// (actually preforms the comparison) only works for ints now
bool intComp(string c1, string c2, string op) {
	bool x;
        if(op.length() <= 2 && op.length() > 0) {
            switch (op[0]) {
		    case '<':
			    if (op[1] == '=') {
				    x = (atoi(c1.c_str()) <= atoi(c2.c_str()));
			    } else {
				    x = (atoi(c1.c_str()) < atoi(c2.c_str()));
			    }	
			    break;
		    case '>':
			    if (op[1] == '=') {
				    x = (atoi(c1.c_str()) >= atoi(c2.c_str()));
			    } else {
				    x = (atoi(c1.c_str()) > atoi(c2.c_str()));
			    };
			    break;
		    case '=':
			    if (op[1] == '=') {
				    x = (atoi(c1.c_str()) == atoi(c2.c_str()));
                            }
			    break;
		    case '!':
			    if (op[1] == '=') {
				    x = (atoi(c1.c_str()) != atoi(c2.c_str()));
			    }
			    break;
		    default: 
			// some kind of incorrect comparison
			    x = false;
			}
			//cout<<"Valid conditon"<<endl;
			return true;
        }
        else { 
			cerr<<"bad condition syntax (op)"<<endl; 
			return false;
		}
	
}

bool attribute(string c) {
	int i = 0;
	bool validAttributeList = true;
	string att;
	vector<string> attributes;
	for (; i<c.length(); i++) {
		if (c[i] == ',') {
			if (att.length() > 0) {
				attributes.push_back(att);
				att = "";
			} else {
				cout<<"Blank attribute"<<endl;
				validAttributeList = false;
				break;
			}
		} else if (isalpha(c[i]) || c[i] == '_') {
			att += c[i];
			i++;
			for (int j=i; j<c.length(); ++j) {
				if (isalnum(c[i])) {
					att.push_back(c[j]);
					i++;
				}
				else{
					break;
				}
			}
			i--;
		} else if (isspace(c[i])) {
			// do nothing...
		} else {
			cerr<<"Invalid attribute"<<endl;
			validAttributeList = false;
		}
	}
	if (validAttributeList) {
		if (att.length() > 0) {
			attributes.push_back(att); // get that last one
		} else {
			cout<<"Blank attribute"<<endl;
			validAttributeList = false;
		}
	}
	return validAttributeList;
}


bool typedAttribute(string c) {
	int i = 0;
	string att, type, size;
	bool validAttributeList = true;
	for (; i<c.length(); i++) {
		if (c[i] == ',') {
			if (att.length() > 0) {
				att = ""; // reset
			} else {
				cerr<<"Blank attribute"<<endl;
				validAttributeList = false;
				break;
			}
		} else if (isalpha(c[i])) {
			att += c[i];
			i++;
			while (isalnum(c[i]) || c[i] == '_') {
				att += c[i];
				i++;
			}
			if (isspace(c[i])) {
				//we are good
				i = skipWS(c,i);
			} else {
				cerr<<"Garbage: "<<c[i]<<endl;
				validAttributeList = false;
				break;
			}
			if (c.substr(i,8) == "varchar(") {
				i += 8;
				size = "";
				while (isdigit(c[i])){
					size += c[i];
					i++;
				}
				if (c[i] == ')') {
					// Valid: VARCHAR Attribute
					// Size is also stored and ready for whenever we need it
				} else {
					cerr<<"Invalid size"<<endl;
					validAttributeList = false;
					break;
				}
			} else if (c.substr(i,7) == "integer") {
				i += 6; 
				// Valid: INTEGER ATTRIBUTE
			} else {
				cerr<<"Invalid type: "<<c.substr(i,8)<<endl;
				validAttributeList = false;
				break;
			}		
		} else if (isspace(c[i])) {
			// do nothing...
		} else {
			cerr<<"Invalid attribute: "<<c[i]<<endl;
			validAttributeList = false;
			break;
		}
	}
	if (validAttributeList) {
		if (att.length() > 0) {
			// checking if last attribute is empty
		} else {
			cout<<"Blank attribute"<<endl;
			validAttributeList = false;
		}
	}
	return validAttributeList;
}
		

//****Daniel*********************************************//
//Checks to see if a literal is found;
bool is_literal(string c, int pos){
	literalcheck:
	//checks for a string bound by quotes, digit strings, and dates
	if (string_with_quotes(c, pos) ||
		is_digit_string(c, pos)){
		if (!end_of_input_reached(c,pos1) &&
			c[pos1+1] == ',') {
				if (!end_of_input_reached(c,pos1+1) &&
					c[pos1+2] == ' ' &&
					!end_of_input_reached(c,pos1+2)) {
						pos=pos1+3;
						goto literalcheck;
				}
				else {
					return false;
				}
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}
//*******************************************************//

//****Daniel*********************************************//
//Checks to see if a literal is a string bound by quotes
bool string_with_quotes(string c, int pos){
	if (!end_of_input_reached(c,pos) &&
		c[pos] == '"') {
			pos = pos+1;
			string attr;
			for (int j=pos; j<c.length(); ++j) {
				if (c[j] != '"') {
					attr.push_back(c[j]);
					pos1 = j;
				}
				else{
					break;
				}
			}
			if (!end_of_input_reached(c,pos1) &&
				c[pos1+1] == '"') {
					//this attribute can be pushed onto a stack of literals
					pos1=pos1+1;
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
//*******************************************************//

//****Daniel*********************************************//
//Checks to see if a literal is a digit/string of digits; digits are still treated as strings
bool is_digit_string(string c, int pos){
	if (!end_of_input_reached(c,pos)){
		//check to see if it's a negative number
		if ((c[pos]) == '-'){
			pos = pos+1;
		}
		if (!end_of_input_reached(c,pos) &&
			isdigit(c[pos])){
				string temp;
				for (int j=pos; j<c.length(); ++j) {
					if (isdigit(c[j])) {
						temp.push_back(c[j]);
						pos1 = j;
					}
					else{
						break;
					}
				}
				if (!end_of_input_reached(c,pos1)) {
					//this int can be pushed onto a stack of literals
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
//*******************************************************//

bool is_update_list(string c, int pos){
	if (c.substr(0,4) == " set") {
		pos1 = 3;
		updatelistcheck:
		if (!end_of_input_reached(c, pos1) &&
			relation_name_check(c, pos1) &&
			c.substr(position1()+1,3) == " = " &&
			!end_of_input_reached(c, position1()+1+2)) {
				pos1 = position1()+1+3;
				if (string_with_quotes(c,pos1) || 
					is_digit_string(c,pos1)){
						if (!end_of_input_reached(c, pos1) &&
							c[pos1+1] == ' ') {
								pos1++;
								return true;
						}
						else if (!end_of_input_reached(c, pos1) &&
							c[pos1+1] == ',' ) {
								pos1 = pos1+1;
								goto updatelistcheck;
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
//*******************************************************//

//****Daniel*********************************************//
int position1(){
	return pos1;
}
//*******************************************************//

//****Daniel*********************************************//
//Checks to see if a literal is a date
//another function
//*******************************************************//


//*******************************************************//
// functions for executing expr; all eventually return Relation (or the query sub-table created)
Relation return_expr(string c, string r){
	if (selection(c, r)) {
		//cout<<"Valid selection, good job"<<endl;
		return return_selection(c, r);
	} else if (projection(c,r)) {
		return dbase.dProject(c);
	} else if (rename_(c,r)) {	// run rename

	} else if (product(c)) {
		//return dbase.dProduct(c);
	}
}
Relation return_atomic(string c, string r){
	if (isalpha(c[0])) {
		return *dbase.getRelation(c);
	} else if (c[0] == '(') {
		return return_pExpr(c, r);
	} else {
		cout<<"Fail - not a proper atomic-expr"<<endl;
	}
}
Relation return_pExpr(string c, string r){
	int i = 1, paren = 1;
	string expression;
	while(paren != 0 && !end_of_input_reached(c,i)) {
		if (c[i] == '(') {
			expression += c[i];
			paren++;
		} else if (c[i] == ')') {
			if (paren > 1) {
				expression += c[i];
			}
			paren--;
		} else {
			expression += c[i];
		}
		i++;
	}
	if (end_of_input_reached(c,i+1) && !isspace(c[i])) {
		cerr<<"Invalid: expr (junk after expr))"<<endl;
	} else {
		cout<<"pExpression = "<<expression<<endl;
		return return_expr(expression, r);
	}
}
Relation return_selection(string c, string r){
	int i = 0;
	string cond, atomicExpr;
	if (c.substr(0,7).compare("select ") == 0) {
		i = skipWS(c,7);
		if (c[i] == '(') {
			i++;
			while(c[i] != ')' && i<c.length()) { 
				cond += c[i]; 
				i++; 
			}
			i = skipWS(c,i+1);
			while(i<c.length()) { 
				atomicExpr += c[i]; 
				i++; 
			}
			Relation s(dbase.selection(return_atomic(atomicExpr, r),cond, r));
			s.setName(r);
			return s;
		} else {
			cerr<<"invalid syntax: no ( for condition"<<endl;
		}
	}
}
Relation return_projection(string c, string r){
	return *dbase.getRelation("howdy");
}
Relation return_product(string c, string r){
	return *dbase.getRelation("howdy");
}
//*******************************************************//
