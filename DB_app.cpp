#include "DB_app.h"
#include "DB_Functions.h"
#include "Relation.h"
#include <sstream>

int userid = 0;
int inventory_num = 0;

extern Database dbase;

void initialize_table() {
    parse("open customers;");
    parse("open dvds;");
    parse("open rental_log;");
}

void print_out_ui(){
	cout<<"WELCOME to Team 6's DVD rental system!"<<endl;
	cout<<"Please, read the following & type in the desired #"<<endl;
	cout<<" 1. Add new customer"<<endl;
	cout<<" 2. Add new DVD"<<endl;
	cout<<" 3. Remove customer"<<endl;
	cout<<" 4. Remove DVD"<<endl;
	cout<<" 5. Update customer"<<endl;
	cout<<" 6. Update DVD"<<endl;
	cout<<" 7. List customer"<<endl;
	cout<<" 8. Search customer (by name, phone)"<<endl;
	cout<<" 9. List DVD"<<endl;
	cout<<" 10. Search DVD (by ID, title)"<<endl;
	cout<<" 11. Search inventory"<<endl;
	cout<<" 12. Check out DVD"<<endl;
	cout<<" 13. Check in DVD"<<endl;
	cout<<" 14. Show rental list by customer"<<endl;
	cout<<" 15. Show customer list by DVD-ID"<<endl;
}

void pick_a_number(string c) {
	string input;
	int input_int;
	//check to see input is a number 1 to 15
	for (int i=0; i<c.length(); i++) {
		if (isdigit(c[i])) {
			// input is a number
			input += c[i];
		}
		else {
			cout<<"Sorry, that is a invalid input"<<endl;
			break;
		}
	}
	input_int = atoi(input.c_str());
	if (input_int < 16 && input_int > 0) {
		//cout<<input_int<<endl;
		if (input_int == 1) {			
                        // run 1. Add new customer
                        string fname, lname, phone_number;
			cout<<"ADD NEW CUSTOMER: "<<endl;
                        cout<<"Enter first name: ";
                        cin>>fname;
                        cout<<endl;
                        cout<<"Enter last name: ";
                        cin>>lname;
                        cout<<endl;
                        cout<<"Enter phone number: ";
                        cin>>phone_number;
                        add_new_customer(fname, lname, phone_number);
                        cout<<"New customer added."<<endl;
		} else if (input_int == 2) {
			// run 2. Add new DVD
                        string dvdid, dvdtitle;
                        cout<<"ADD NEW DVD: "<<endl;
                        cout<<"Enter DVD ID: ";
                        cin>>dvdid;
                        cout<<endl;
                        cout<<"Enter DVD title: ";
                        cin>>dvdtitle;
			add_new_DVD(dvdid, dvdtitle);
                        cout<<"New DVD added."<<endl;
		} else if (input_int == 3) {
			//run 3. Remove customer
			int custid;
                        string fname, lname;
                        cout<<"REMOVE A CUSTOMER: "<<endl;
                        cout<<"Enter the customer ID: ";
                        cin>>custid;
                        remove_customer(custid);
                        cout<<"Customer "<<custid<<" removed."<<endl;
		} else if (input_int == 4) {
			//run 4. Remove DVD
                        int invnum;
                        cout<<"REMOVE A DVD: "<<endl;
                        cout<<"Enter the DVD inventory number: ";
                        cin>>invnum;
			remove_DVD(invnum);
                        cout<<"DVD "<<invnum<<" removed."<<endl;
		} else if (input_int == 5) {
			//run 5. Update customer
                        string cust_attr, cust_op, cust_val, cust_cond, cond_op, cond_val;
                        cout<<"UPDATE THE CUSTOMER(S): "<<endl;
                        cout<<"Format: UPDATE customers SET cust_attr(first name, last name, etc.) cust_op(=,<, etc.) cust_val(new value) WHERE cust_cond(first name, last name, etc.) cond_op(=,<,etc.) cond_val(comparison value);"<<endl;
                        cout<<"Enter an attribute(first name, ID, etc.): ";
                        cin>>cust_attr;
                        cout<<endl;
                        cout<<"Enter an op to perform on attribute(==,<=,etc.): ";
                        cin>>cust_op;
                        cout<<endl;
                        cout<<"Enter a value that is being compared to attribute(john, 0, etc.): ";
                        cin>>cust_val;
                        cout<<endl;
                        cout<<"Enter the first part of the WHERE condition(first name, ID, etc.): ";
                        cin>>cust_cond;
                        cout<<endl;
                        cout<<"Enter the operator that will be used(<=,=,etc.): ";
                        cin>>cond_op;
                        cout<<endl;
                        cout<<"Enter the value that is being compared(john, 0, etc.): ";
                        cin>>cond_val;

			update_customer(cust_attr, cust_op, cust_val, cust_cond, cond_op, cond_val);
                        cout<<"Customer(s) successfully updated."<<endl;
		} else if (input_int == 6) {
			//run 6. Update DVD
			update_DVD();
		} else if (input_int == 7) {
			//run 7. List customer
			list_customer();
		} else if (input_int == 8) {
			//run 8. Search customer (by name, phone)
			search_customer();
		} else if (input_int == 9) {
			//run 9. List DVD
			list_DVD();
		} else if (input_int == 10) {
			//run 10. Search DVD (by ID, title)
			search_DVD();
		} else if (input_int == 11) {
			//run 11. Search inventory
			search_inventory();
		} else if (input_int == 12) {
			//run 12. Check out DVD
			check_out_DVD();
		} else if (input_int == 13) {
			//run 13. Check in DVD
			check_in_DVD();
		} else if (input_int == 14) {
			//run 14. Show rental list by customer
			show_rental_list();
		} else if (input_int == 15) {
			//run 15. Show customer list by DVD-ID
			show_customer_list();
		}
	}
	else {
		cout<<"Sorry, that is a invalid input"<<endl;
	}
}

void add_new_customer(string fname, string lname, string phone_number) {
	/* //USER-ID takes the next unavailable number
	First name: > 
	Last name: >
	Phone Number: >
	*/
        string uid;
        stringstream ssout;
        ssout<<userid;
        uid = ssout.str();
    
        string insert_query = "insert into customers values from (" + uid + ", \"" + fname + "\", \"" + lname + "\", \"" + phone_number + "\");";
        cout<<insert_query<<endl;
        parse(insert_query);
        userid++;

	// create string for engine to run
}
void add_new_DVD(string dvd_id, string dvd_title) {
	/* //Inventory number takes the next unavailable number
	//DVD-ID: 
	Title: >
	*/
        string invnum;
        stringstream ssout;
        ssout<<inventory_num;
        invnum = ssout.str();

        string insert_query = "insert into dvds values from (" + invnum + ", \"" + dvd_id + "\", \"" + dvd_title + "\");";
        cout<<insert_query<<endl;
        parse(insert_query);
        inventory_num++;
        

	// create string for engine to run
}
void remove_customer(int custid) {
	/*
	First name: >
	Last name: >
	*/
        string cid;
        stringstream ssout;
        ssout<<custid;
        cid = ssout.str();

        string remove_query = "delete from customers where userID == " + cid + ";";
        cout<<remove_query<<endl;
        parse(remove_query);
	// create string for engine to run
}
void remove_DVD(int inv_num) {
	/*
	INVNUM: >
	*/
        string invnum;
        stringstream ssout;
        ssout<<inv_num;
        invnum = ssout.str();

        string remove_query = "delete from dvds where inventoryNumber == " + invnum + ";";
        cout<<remove_query<<endl;
        parse(remove_query);

	// create string for engine to run
}
void update_customer(string custattr, string custop, string custval, string custcond, string condop, string condval) {
	/*
	First name: >
	Last name: >
	*/
        string update_query = "update customers set " + custattr + " " + custop + " " + custval + " where " + custcond + " " + condop + " " + condval + ";";
        cout<<update_query<<endl;
        parse(update_query);

	// create string for engine to run
}
void update_DVD(){
	/*
	Title: >
	*/

	// create string for engine to run
}
void list_customer(){
	/*
	//prints out table of customers
	//columns: USER-ID, First name, Last name, Phone number
	*/
	parse("show customers;");
	// create string for engine to run
}
void search_customer(){
	/*
	Search by 1) name or 2) phone #
		1)
			Type in first name: >
			Type in last name: >
		2)
			Type in phone #: >

	*/

	// create string for engine to run
}
void list_DVD(){
	/*
	//prints out table of DVDs
	//columns: DVD-ID, Title, Inventory number
	*/
	parse("show dvds;");

	// create string for engine to run
}
void search_DVD(){
	/*
	Search by 1) Title or 2) DVD-ID
	1)
		Type in DVD title: >
	2)
		Type in DVD-ID: >
	*/

	// create string for engine to run
}
void search_inventory(){
	/*
	*/

	// create string for engine to run
}
void check_out_DVD(){

	char check_date[BUFSIZ] = {'\0'};
	char due_date[BUFSIZ] = {'\0'};
	time_t now = time(&now);
	struct tm* local_time = new tm() ;
	local_time = localtime(&now) ;
	sprintf(check_date,"%02d/%02d/%04d", local_time->tm_mon+1, local_time->tm_mday, local_time->tm_year+1900); 
	sprintf(due_date,"%02d/%02d/%04d", local_time->tm_mon+1, local_time->tm_mday+7, local_time->tm_year+1900); 


	string uid, out_choice;
	cout<<"Enter the user ID to check out a DVD: ";
	cin>>uid;
	cout<<endl;
	string invnum;
	cout<<"Enter the DVD inventory number: ";
	cin>>invnum;
	cout<<endl;
	string parse_invnum = "insert into rental_log values from (" + uid + ", " + invnum + ", \"" + check_date + "\", \"" + due_date + "\");";
	parse(parse_invnum);
	cout<<"The DVD is due back by: "<<due_date<<endl;

}
void check_in_DVD(){
	/*
	Check in by 1) Title or by 2) DVD-ID
		1)
			What DVD are you returning today? Title: >
		2)
			What DVD are you returning today? DVD-ID: >
	//Thanks for renting from us!
	*/

	// create string for engine to run
}
void show_rental_list(){
	int custRow = -1;
	string user_id;
	
	// Create Table
	Relation rent_list("rental_list");
	rent_list.addColumn("inv_num", Relation::VARCHAR, 20, true);
	rent_list.addColumn("checked_out", Relation::VARCHAR, 20, false);
	rent_list.addColumn("due_date", Relation::VARCHAR, 20, false);
	rent_list.addColumn("first_Name", Relation::VARCHAR, 20, false);
	rent_list.addColumn("last_Num", Relation::VARCHAR, 20, false);
	
	// scan through rental log
	for (int i=0; i < (dbase.justGetRelation("rental_log").getRows()); i++) {
		
		// get user id
		user_id = dbase.justGetRelation("rental_log").getValue(0,i);
		
		// find row in customers
		for (int j=0; j < (dbase.justGetRelation("customers").getRows()); j++) {
			if (user_id == dbase.justGetRelation("customers").getValue(0,j)) {
				custRow = j;
				
				// add the row to the table
				rent_list.addValue((dbase.justGetRelation("rental_log").getValue(1,i)), 0);
				rent_list.addValue((dbase.justGetRelation("rental_log").getValue(2,i)), 1);
				rent_list.addValue((dbase.justGetRelation("rental_log").getValue(3,i)), 2);
				rent_list.addValue((dbase.justGetRelation("customers").getValue(1,custRow)), 3);
				rent_list.addValue((dbase.justGetRelation("customers").getValue(2,custRow)), 4);
				
				break;
			}
		}
	}
	
	// remove if already exists
	if (dbase.relation_exists("rental_list")) {
		dbase.remove(dbase.get_relation_position("rental_list"));
	}
		
	dbase.add(rent_list);
	cout<<"Rental List updated"<<endl;
	parse("show rental_list;");
	// success
}
void show_customer_list(){
	
	int rentRow = -1;
	int custRow = -1;
	
	bool rented;
	
	// get DVD-ID
	string dvd_id;
	string inv;
	string user_id;
	stringstream ssout;
	ssout<<userid;
	dvd_id = ssout.str();
	
	// Create Table
	Relation cust_list("customers_by_dvd");
	cust_list.addColumn("DVD_ID", Relation::INTEGER, 8, true);
	cust_list.addColumn("User_ID", Relation::INTEGER, 8, false);
	cust_list.addColumn("First_Name", Relation::VARCHAR, 20, false);
	cust_list.addColumn("Last_Name", Relation::VARCHAR, 20, false);
	cust_list.addColumn("Phone_Num", Relation::VARCHAR, 20, false);
	
	// scan through dvd table
	for (int i=0; i < (dbase.justGetRelation("dvds").getRows()); i++) {
		
		// get inventory #
		inv = dbase.justGetRelation("dvds").getValue(0,i);
		
		// find row in rental log
		for (int j=0; j < (dbase.justGetRelation("rental_log").getRows()); j++) {
			if (inv == dbase.justGetRelation("rental_log").getValue(1,j)) {
				rentRow = j;
				user_id = dbase.justGetRelation("rental_log").getValue(0,j);
				break;
			}
		}
		
		if (rentRow == -1) {
			// dvd not in rental log
			rented = false;
		} else {
			rented = true;
			
			// find row in customers
			for (int j=0; j < (dbase.justGetRelation("customers").getRows()); j++) {
				if (user_id == dbase.justGetRelation("customers").getValue(0,j)) {
					custRow = j;
					break;
				}
			}
		}
		
		if (!rented) {
			// dont add anything
		} else {			
			cust_list.addValue((dbase.justGetRelation("dvds").getValue(1,i)), 0);
			cust_list.addValue(dbase.justGetRelation("customers").getValue(0,custRow),1);
			cust_list.addValue(dbase.justGetRelation("customers").getValue(1,custRow),2);
			cust_list.addValue(dbase.justGetRelation("customers").getValue(2,custRow),3);
			cust_list.addValue(dbase.justGetRelation("customers").getValue(3,custRow),4);
			// now add each thing from this row int columns [1-4]
			// then add dvd-id to col 0	
		}
	}
	
	// remove if already exists
	if (dbase.relation_exists("customers_by_dvd")) {
		dbase.remove(dbase.get_relation_position("customers_by_dvd"));
	}
	
	dbase.add(cust_list);
	cout<<"Customer Rental List updated"<<endl;
	parse("show customers_by_dvd;");
	// success
}
