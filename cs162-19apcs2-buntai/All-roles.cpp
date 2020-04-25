#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========
#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========
void login() {
	allRole newturn, check;
	//get login info from user
	cout << "\tHello, Welcome to the program" << endl;
	cout << "\tFor login, please enter your username and password" << endl;
	cout << "\tUsername:";
	getline(std::cin, newturn.username);
	cout << endl;
	cout << "\tPassword:";
	getline(std::cin, newturn.password);
	cout << endl;
	//check the login information
	ifstream in;
	in.open("user.txt");
	if (!in) cout << "\tCannot open user file, please try it later" << endl;
	else
	{
		while (in) {
			getline(in, check.username);
			getline(in, check.password);
			if (newturn.username == check.username && newturn.password == check.password)
			{
				in >> newturn.role;
				in.close();
				cout << "\tHello " << newturn.username << endl;
				return;
			}
		}
	}

	in.close();
	cout << "\tLogin failed" << endl;
}
