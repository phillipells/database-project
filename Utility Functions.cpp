#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "Utility Functions.h"

// Skips whitespace
int skipWS(string c, int i) {
	while (isspace(c[i])){
		i++;
	}
	return i;
}

//****Daniel*********************************************//
//Checks to see if the end of the input string has been reached
bool end_of_input_reached(string c, int pos) {
	if (pos != c.find_last_of(c)){
		return false;
	}
	else {
		return true;
	}
}