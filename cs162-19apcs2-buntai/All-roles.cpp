#include "Function.h"
#include<iostream>

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========

// 1.0
void showAllRoleMenu(string &userName, int &userRole) {
	cout << "\t1. Show options\n"
		 << "\t2. View profile\n"
		 << "\t3. Change password\n"
		 << "\t4. Log out\n\n";
	string choice;
	cout << "Please enter your choice: ";
	cin >> choice;
	clearScreenWithoutPress();
	if (choice == "1") {
		showMenu(userName,userRole);
		clearScreenWithoutPress();	
	}
	else if (choice == "2") {
		viewProfileInfo(userName, userRole);
		clearScreen();
	}
	else if (choice == "3") {
		changePassword(userName);
		clearScreen();
	}
	else if (choice == "4") {
		logout(userName, userRole);
		if (userName != "\0") {
			clearScreenWithoutPress();
			showAllRoleMenu(userName, userRole);
		}
	}
	else {
		cout << "Wrong option number.\n\n";
		cin.ignore();
		clearScreen();
	}
	if (choice != "4")
		showAllRoleMenu(userName, userRole);
}

// 1.1
bool login(string& userName, int& userRole) {
	string userPassword = "", checkName, checkPassword;
	int type;

	// Get login info from user.
	cout << "Please add login information:" << endl;
	cout << "\tUsername: ";
	cin >> userName;
	cout << "\tPassword: ";
	char c = _getch();
	while (c != 13) { // c != '\n'
		if (c == 8) { // c == backspace
			if (!userPassword.empty()) {
				userPassword.pop_back();
				cout << "\b \b";
			}
			c = _getch();
			continue;
		}
		_putch('*');
		userPassword += c;
		c = _getch();
	}
	cout << "\n\n";

	// Check the login information.
	ifstream in;
	in.open("Database/User.txt");
	if (!in) {
		cout << "Login failed. Error: User file is missing, please try again later.\n\n";
		return false;
	}
	while (getline(in, checkName)) {
		getline(in, checkPassword);
		in >> type;
		in.ignore();
		if (userName == checkName && userPassword == checkPassword) {
			userRole = type;
			in.close();
			switch (userRole)
			{
			case 0: {
				Staff newTurn;
				ifstream in;
				in.open("Database/Staff.txt");
				if (!in) cout << "Cannot open staff file, please try it later" << endl;
				else
				{
					while (in) {
						//read data of each user in file 
						getline(in, newTurn.username);
						getline(in, newTurn.name);
						in >> newTurn.gender;
						in.ignore();
						in.ignore();
						if (userName == newTurn.username)
							break;
					}
					in.close();
					if (newTurn.gender == 0)
						cout << "Welcome Ms. " << newTurn.name << "!\n";
					if (newTurn.gender == 1)
						cout << "Welcome Mr. " << newTurn.name << "!\n";
				}
			}
			case 1: {
				Lecturer lecturer;
				findLecturerFromUsername(userName, lecturer);
				if (lecturer.gender == 0)
					cout << "Welcome Ms. " << lecturer.name << "!\n";
				if (lecturer.gender == 1)
					cout << "Welcome Mr. " << lecturer.name << "!\n";
				break;
			}
			case 2: {
				Student newTurn;
				ifstream in;
				int count = 0;
				in.open("Database/Class/Classes.txt");
				count++;
				if (!in) cout << "Cannot open class file, please try it later" << endl;
				else {
					while (in >> newTurn.myClass)
						if (findStudentInfoFromFile(newTurn, userName))
							break;
					in.close();
					cout << "Hello " << newTurn.name << "!\n";
				}
				break;
			}
			default:
				break;
			}
			return true;
		}
		else if (userName == checkName && userPassword != checkPassword) {
			in.close();
			cout << "Login failed. Error: Wrong password for " << userName << ".\n\n";
			return false;
		}
	}
	in.close();
	cout << "Login failed. Error: Username not found.\n\n";
	return false;
}

// 1.2
void showMenu(string &username,int& userRole) {
	int chon;
	clearScreenWithoutPress();
	if (userRole == 0) {
		showStaffMenu(username);
	}
	else if (userRole == 1) {
		showLecturerMenu(username);
	}
	else if (userRole == 2) {
		showStudentMenu(username);
	}
}

// 1.3
void viewProfileInfo(string& username, int& userRole) {
	if (userRole == 0) {
		Staff newTurn;
		ifstream in;
		in.open("Database/Staff.txt");
		if (!in) cout << "Cannot open staff file, please try it later" << endl;
		else
		{
			while (in) {
				// Read data of each user in file.
				getline(in, newTurn.username);
				getline(in, newTurn.name);
				in >> newTurn.gender;
				in.ignore();
				in.ignore();
				if (username == newTurn.username)
				{
					// Print out info.
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newTurn.username << endl;
					cout << "\tFullname: " << newTurn.name << endl;
					if (newTurn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					in.close();
					return;
				}
				in.close();
			}
		}
	}
	else if (userRole == 1)
	{
		Lecturer newTurn;
		ifstream in;
		in.open("Database/Lecturer.txt");
		if (!in) cout << "Cannot open lecturer file, please try it later" << endl;
		else
		{
			while (in) {
				// Read data of each user in file.
				getline(in, newTurn.username);
				getline(in, newTurn.name);
				getline(in, newTurn.title);
				in >> newTurn.gender;
				in >> newTurn.totalCourse;
				if (newTurn.totalCourse != 0)
				{
					newTurn.myCourse = nullptr;
					CourseInfo* currentCourse = 0;
					for (int i = 0; i < newTurn.totalCourse; ++i) {
						if (newTurn.myCourse == nullptr) {
							newTurn.myCourse = new CourseInfo;
							in >> newTurn.myCourse->academicYear;
							in >> newTurn.myCourse->academicYear;
							in >> newTurn.myCourse->semester >> newTurn.myCourse->courseName >> newTurn.myCourse->defaultClass;
							newTurn.myCourse->next = nullptr;
							currentCourse = newTurn.myCourse;
						}
						else {
							currentCourse->next = new CourseInfo;
							currentCourse = currentCourse->next;
							in >> currentCourse->academicYear;
							in >> currentCourse->academicYear;
							in >> currentCourse->semester >> currentCourse->courseName >> currentCourse->defaultClass;
							currentCourse->next = nullptr;
						}
					}
				}
				in.ignore();
				in.ignore();
				if (username == newTurn.username)
				{
					// Print out info.
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newTurn.username << endl;
					cout << "\tFullname: " << newTurn.name << endl;
					cout << "\tTitle: " << newTurn.title << endl;
					if (newTurn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					cout << "\tCourses: " << newTurn.totalCourse << endl;
					if (newTurn.totalCourse != 0) {
						cout << "\tYour courses list:" << endl;
						CourseInfo* courseInfo = newTurn.myCourse;
						printCourseListTable(courseInfo);
						deleteCourseInfo(courseInfo);
					}
					in.close();
					return;
				}
			}
			in.close();
		}
	}
	else if (userRole == 2)
	{
		Student newTurn;
		ifstream in;
		in.open("Database/Class/Classes.txt");
		if (!in) {
			cout << "Cannot open class file, please try it later" << endl;
			return;
		}
		else {
			while (in >> newTurn.myClass)
				if (findStudentInfoFromFile(newTurn, username))
					break;
			in.close();
		}
		cout << "Student info:\n";
		cout << "\tUsername: " << newTurn.username << "\n";
		cout << "\tStatus: ";
		if (newTurn.status == 1) cout << "Available" << "\n";
		else cout << "Dropped\n";
		cout << "\tName: " << newTurn.name << "\n";
		cout << "\tStudent ID: " << newTurn.studentId << "\n";
		cout << "\tGender: ";
		if (newTurn.gender == 0) cout << "female\n";
		else cout << "male\n";
		cout << "\tDate of birth: " << dateToString(newTurn.dob) << "\n";
		cout << "\tClass: " << newTurn.myClass << "\n";
		cout << "\tCourses: " << newTurn.numberOfCourse << "\n";
				if (newTurn.numberOfCourse != 0) {
			cout << "\tYour courses list:" << endl;
			CourseInfo* courseInfo = newTurn.myCourse;
			printCourseListTable(courseInfo);
			deleteCourseInfo(courseInfo);
		}
		in.close();
		return;
	}
	return;
}

// 1.4
void changePassword(string& username) {
	string userPassword = "", checkName, checkPassword, newPassword, confirmedPassword;
	string* userlist[1000];
	cout << "To change password, please enter:\n";
	cout << "\tCurrent password: ";
	char c = _getch();
	while (c != 13) { // c != '\n'
		if (c == 8) { // c == backspace
			if (!userPassword.empty()) {
				userPassword.pop_back();
				cout << "\b \b";
			}
			c = _getch();
			continue;
		}
		_putch('*');
		userPassword += c;
		c = _getch();
	}
	cout << "\n";

	// Check password and find position of password in file User.txt.
	ifstream in;
	in.open("Database/User.txt");
	int count = 0, n = 0;
	if (!in) {
		cout << "Change password failed. Error: User file is missing, please try again later.\n\n";
		return;
	}

	while (getline(in, checkName)) {
		count++; n++;
		getline(in, checkPassword); count++; n++;
		in.ignore(); count++; n++;
		in.ignore();
		if (username == checkName && userPassword == checkPassword) {
			// Continue to count the number of lines of file text.
			while (in)
			{
				in.ignore(); n++;
				in.ignore();  n++;
				in.ignore();  n++;
				in.ignore();  
			}
			in.close();

			cout << "\tNew password: " ;
			char d = _getch();
			while (d!= 13) { // c != '\n'
				if (d == 8) { // c == backspace
					if (!newPassword.empty()) {
						newPassword.pop_back();
						cout << "\b \b";
					}
					d = _getch();
					continue;
				}
				_putch('*');
				newPassword += d;
				d = _getch();
			}
			cout << "\n";
			cout << "\tEnter new password again: " ;
			char e = _getch();
			while (e!= 13) { // c != '\n'
				if (e == 8) { // c == backspace
					if (!confirmedPassword.empty()) {
						confirmedPassword.pop_back();
						cout << "\b \b";
					}
					e = _getch();
					continue;
				}
				_putch('*');
				confirmedPassword += e;
				e = _getch();
			}
			cout << "\n\n";

			if (newPassword == confirmedPassword) {
				// In file text, replace old password by new password.
				// Create a array to store each line of file text, replace the line store old password by new password.
				string* userFile;
				userFile = new string[n];
				ifstream in;
				in.open("Database/User.txt");
				for (int i = 0; i < n; i++) {
					if (i + 1 == count) {
						userFile[i - 1] = newPassword;
						in.ignore();
						in >> userFile[i];
					}
					else in >> userFile[i];
				}
				in.close();

				// Rewrite User.txt.
				ofstream out;
				out.open("Database/User.txt");
				for (int i = 0; i < n; i++) {
					out << userFile[i] << endl;
				}
				out.close();
				cout << "Your password has been changed successfully.\n\n";
				delete[]userFile;
			}
			else cout << "Change password failed. Error: 2 entered passwords do not match.\n\n";
			return;
		}
		else if (username == checkName && userPassword != checkPassword) {
			in.close();
			cout << "\nChang password failed. Error: Wrong password for " << username << ".\n\n";
			return;
		}
	}
}

// 1.5 
void logout(string& userName, int& userRole) {
	int choice;
	cout << "Are you sure to log out? Y/N\t";
	string confirm;
	cin >> confirm; toUpper(confirm);
	if (confirm == "N")
		return;
	userName = "\0";
	clearScreenWithoutPress();
	cout << "Do you want to login into another account or exit the program?" << endl;
	cout << "\t1. Login again" << endl;
	cout << "\t2. Exit program" << endl;
	cout << "\tYour choice: ";
	cin >> choice;
	if (choice == 1) {
		clearScreenWithoutPress();
		while (!login(userName, userRole)) {
			clearScreen();
		}
		clearScreen();
		showAllRoleMenu(userName, userRole);
	}
	else if (choice == 2) {
		clearScreenWithoutPress();
		cout << "\t\t\t\tThank you for using our program.\n\n";
		system("exit");
	}
}
