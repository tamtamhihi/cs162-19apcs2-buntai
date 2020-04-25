#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========
void login() {
	string newturnname, newturnpassword, checkname, checkpassword;
	int newturnrole;
	//get login info from user
	cout << "\tHello, Welcome to the program" << endl;
	cout << "\tFor login, please enter your username and password" << endl;
	cout << "\tUsername:";
	getline(std::cin, newturnname);
	cout << endl;
	cout << "\tPassword:";
	getline(std::cin, newturnpassword);
	cout << endl;
	//check the login information
	ifstream in;
	in.open("user.txt");
	if (!in) cout << "\tCannot open user file, please try it later" << endl;
	else
	{
		while (in) {
			getline(in, checkname);
			getline(in, checkpassword);
			if (newturnname == checkname && newturnpassword == checkpassword)
			{
				in >> newturnrole;
				in.close();
				cout << "\tHello " << newturnname << endl;
				return ;
			}
		}
	}

	in.close();
	cout << "\tLogin failed" << endl;
}
