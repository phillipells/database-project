// so I began to rewrite the program
// the original one was leading us to a dead end (sorry for the wasted time)
// while this way is much more slow going, it is actually starting to work (completely)
// Im taking it on "depth-first" style and just tackling each case individually
// its a slow start, but it will definitely start moving alot faster soon

// so far I got it to actually do some math
// example input:
// > youcanliterallywriteanythinghere  <-   select   (3<5)
//         ^                            ^     ^        ^
//      literal                       arrow  expr   condition
// it will return abunch of "debugging" info
// but in the end... true or false (true in this case because 3<5)
// this is an example of a query with a condition
// lots more to do, just posting it to spread the news
// this weekend will be fun....

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <ctype.h>

using namespace std;

static void parse(string c);
static void query(string c);
static void expr(string c);
static void selection(string c);
static bool condition(string c);
static bool conjunction(string c);
static bool comparison(string c);
static bool comp(string c1, string c2, char op);

static int skipWS(string c, int i);

int skipWS(string c, int i) {
	while (isspace(c[i])){
		i++;
	}
	return i;
}

void parse(string c) {
		
	//check if its a query or a command
	query(c);
	//command(c);
	//fail
}

// query
void query(string c) {
	int i = 0;
	while (isalnum(c[i])) { i++; }
	i = skipWS(c,i);
	if (c.substr(i,2).compare("<-") == 0) {
		i = skipWS(c,i+2);
		expr(c.substr(i,i-c.length()));
	}
}

// expression (will not be pointless when finished)
void expr(string c) {
	selection(c);
}

// selection
void selection(string c) {
	int i = 0;
	string cond;
	if (c.substr(0,6).compare("select") == 0) {
		i = skipWS(c,6);
		if (c[i] == '(') {
			i++;
			while(c[i] != ')') { cond += c[i]; i++; }
			cout<<"cond: "<<cond<<endl;
			if(condition(cond)) {
				cout<<"true"; // when finished, this will perform the selection
			} else cout<<"false"; // this will not
		}
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
		if(c.substr(i,2).compare("||") == 0) {
			cout<<"it got in"<<endl;
			i = skipWS(c.substr(i,2),i);
			meta = condition(c.substr(i,2));
		}
	}
	cout<<"meta: "<<meta<<endl;
	cout<<"conj: "<<conj<<endl;
	return (conjunction(conj) || meta);
}

// conjunction
bool conjunction(string c) {
	int i = 0;
	string comp;
	bool meta = false;
	
	while (c[i] != '&' && i<c.length()) {
		comp += c[i];
		i++;
		if(c.substr(i,2).compare("&&") == 0) {
			i = skipWS(c.substr(i,2),i);
			meta = conjunction(c.substr(i,2));
		}
	}
	cout<<"comp: "<<comp<<endl;
	return (comparison(comp) || meta);
}

// comparison
bool comparison(string c) {
	int i = 0;
	string op1, op2;
	char op;
	
	while (isalnum(c[i])) {
		op1 += c[i];
		i++;
	}
	i = skipWS(c,i);
	cout<<i<<": "<<c[i]<<endl;
	if (ispunct(c[i])) {
		op = c[i];
		i++;
	}
	i = skipWS(c,i);
	cout<<i<<": "<<c[i]<<endl;
	while (isalnum(c[i])) {
		op2 += c[i];
		i++;
	}
	cout<<i<<": "<<c[i]<<endl;
	cout<<"op1: "<<op1<<endl;
	cout<<"op2: "<<op2<<endl;
	cout<<"op: "<<op<<endl;
	return comp(op1,op2,op);
}

// (actually preforms the comparison)
bool comp(string c1, string c2, char op) {
	bool x;
	switch (op) {
		case '<':
			x = (atoi(c1.c_str()) < atoi(c2.c_str()));
			break;
		default: 
			x = false;
	}
	cout<<"c1: "<<atoi(c1.c_str())<<endl;
	cout<<"c2: "<<atoi(c2.c_str())<<endl;
	cout<<"x: "<<x<<endl;
	return x;
}

int main() {
	char program[256];
	cout<<"> ";
	cin.getline(program,256);
	parse(program);
	cout<<"; *done*"<<endl;
}
