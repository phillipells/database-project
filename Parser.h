#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include "Relation.h"
#include "Shared Functions.h"
#include "Utility Functions.h"
#include "DB_Functions.h"

void parse(string c);

//query check function
static bool query(string c);

//command check function
static bool command(string c);