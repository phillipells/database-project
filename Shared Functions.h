#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "Relation.h"
#include "Utility Functions.h"

//shared functions
void expr(string c, string r);
//expr can be the following: atomic-expr | selection | projection | renaming | union | difference | product
	bool atomic(string c, string r);
		//atomic-expr ::= relation-name | ( expr )
		static bool relationName(string c);
		bool relation_name_check(string c, int pos);
		bool is_update_list(string c, int pos);
		static bool pExpr(string c, string r);

	static bool selection(string c, string r);
	static bool projection(string c, string r);
	static bool rename_(string c, string r);
	//static void union
	//static void difference
	static bool product(string c);
	
bool condition(string c);
	bool conjunction(string c);
	bool comparison(string c);
	bool intComp(string c1, string c2, string op);

bool attribute(string c);
bool typedAttribute(string c);

bool is_literal(string c, int pos);
	bool string_with_quotes(string c, int pos);
	bool is_digit_string(string c, int pos);

// functions for executing expr; all eventually return Relation (or the query sub-table created)
Relation return_expr(string c, string r);
Relation return_atomic(string c, string r);
Relation return_relationName(string c);
Relation return_pExpr(string c, string r);
Relation return_selection(string c, string r);
Relation return_projection(string c, string r);
Relation return_product(string c, string r);

//global variable for command checks
static int pos1; //keeps track of string position
int position1(); //returns the value of pos1

//global variable for returning a false query
static bool validExpr = false;
