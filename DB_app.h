#include "Parser.h"

void initialize_table();
void print_out_ui();
void pick_a_number(string c);

// operations
void add_new_customer(string fname, string lname, string phone_number);
void add_new_DVD(string dvd_id, string dvd_title);
void remove_customer(int custid);
void remove_DVD(int inv_num);
void update_customer(string custattr, string custop, string custval, string custcond, string condop, string condval);
void update_DVD();
void list_customer();
void search_customer();
void list_DVD();
void search_DVD();
void search_inventory();
void check_out_DVD();
void check_in_DVD();
void show_rental_list();
void show_customer_list();
