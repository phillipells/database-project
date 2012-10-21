//*****************************
// Database Manager Beta 1.0
// June 5th, 2012
// CSCE 315-100 Project 1:
// Team 6:
//	Phillip Ells
//	William Guerra
//	Daniel Tan
//*****************************

#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "DB_app.h"

//Global database
Database dbase;

int main() {	
	
	initialize_table(); // creates the needed tables
	print_out_ui();

	char program[256];
	string str;
	while (true) {
		while (str.substr(0,1) != "0") { // skip the UI to test the engine
			cout<<"> ";
			cin.getline(program,256);
			pick_a_number(program);
			str = program;
		}
		while (true) { // skip the engine to test the UI
			cout<<"> ";
			cin.getline(program,256);
			parse(program);
			str = program;
		}
	}
}