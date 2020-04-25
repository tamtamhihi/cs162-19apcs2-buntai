#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========

//1.1

void login(string& username,int& userrole) {
	string userpassword, checkname, checkpassword;
	//get login info from user
	cout << "Hello, Welcome to the program" << endl;
	cout << "For login, please enter your username and password" << endl;
	cout << "\tUsername:";
	getline(std::cin, username);
	cout << endl;
	cout << "\tPassword:";
	getline(std::cin, userpassword);
	cout << endl;
	//check the login information
	ifstream in;
	in.open("User.txt");
	if (!in) cout << "\tCannot open user file, please try it later" << endl;
	else
	{
		while (in) {
			getline(in, checkname);
			getline(in, checkpassword);
			if (username == checkname && userpassword == checkpassword)
			{
				in >> userrole;
				in.close();
				cout << "\tHello " << username << endl;
				return ;
			}
		}
	}

	in.close();
	cout << "\tLogin failed" << endl;
	cout << endl;
}
