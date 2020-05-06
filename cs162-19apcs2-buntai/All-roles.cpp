#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========

// 1.1

void login(string& username, int& userrole) {
	string userpassword = "", checkname, checkpassword;
	int type;

	// Get login info from user.
	cout << "Please add login information:" << endl;
	cout << "\tUsername: ";
	getline(cin, username);
	cout << "\tPassword: ";
	char c = _getch();
	while (c != 13) { // c != '\n'
		if (c == 8) { // c == backspace
			if (!userpassword.empty()) {
				userpassword.pop_back();
				cout << "\b \b";
			}
			c = _getch();
			continue;
		}
		_putch('*');
		userpassword += c;
		c = _getch();
	}
	cout << "\n\n";

	// Check the login information.
	ifstream in;
	in.open("Database/User.txt");
	if (!in) {
		cout << "Login failed. Error: User file is missing, please try again later.\n\n";
		return;
	}
	while (getline(in, checkname)) {
		getline(in, checkpassword);
		in >> type;
		in.ignore();
		if (username == checkname && userpassword == checkpassword) {
			userrole = type;
			in.close();
			cout << "\tWelcome " << username << ".\n\n";
			return;
		}
		if (username == checkname && userpassword != checkpassword) {
			in.close();
			cout << "Login failed. Error: Wrong password for " << username << ".\n\n";
			return;
		}
	}
	in.close();
	cout << "Login failed. Error: Username not found.\n\n";
}

// 1.2

void showMenu(int& userrole) {
	int chon;
	if (userrole == 0) {
		showStaffMenu();
	}
	else if (userrole == 1) {
		showLecturerMenu();
	}
	else if (userrole == 2) {
		showStudentMenu();
	}
}


//1.3

void viewProfileInfo(string& username, int& userrole) {
	if (userrole == 0) {
		Staff newturn;
		ifstream in;
		in.open("Database/Staff.txt");
		if (!in) cout << "Cannot open staff file, please try it later" << endl;
		else
		{
			while (in) {
				getline(in, newturn.username);
				getline(in, newturn.name);
				in >> newturn.gender;
				in.ignore();
				in.ignore();
				if (username == newturn.username)
				{
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newturn.username << endl;
					cout << "\tFullname: " << newturn.name << endl;
					if (newturn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					return;
				}
			}
			in.close();
		}
	}
	else if (userrole == 1)
	{
		Lecturer newturn;
		ifstream in;
		in.open("Database/Lecturer.txt");
		if (!in) cout << "Cannot open lecturer file, please try it later" << endl;
		else
		{
			while (in) {
				getline(in, newturn.username);
				getline(in, newturn.name);
				getline(in, newturn.title);
				in >> newturn.gender;
				in.ignore();
				in.ignore();
				if (username == newturn.username)
				{
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newturn.username << endl;
					cout << "\tFullname: " << newturn.name << endl;
					cout << "\tTitle: " << newturn.title << endl;
					if (newturn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					return;
				}

			}
			in.close();
		}
	}
	else if (userrole == 2)
	{
		Student newturn;
		ifstream in;
		int count = 0;
		in.open("Database/Class/Classes.txt");
		count++;
		if (!in) cout << "Cannot open class file, please try it later" << endl;
		else (in >> newturn.myClass);
		in.close();
		while (findStudentInfoFromFile(newturn, username) == false) {
			ifstream in;
			in.open("Database/Class/Classes.txt");
			while (in) {
				for (int i = 0; i < count; i++) in >> newturn.myClass;
			}
			in.close();
		}

		cout << "Student info:\n";
		cout << "\tUsername: " << newturn.username << "\n";
		cout << "\tStatus: ";
		if (newturn.status == 1) cout << "Available" << "\n";
		else cout << "Dropped\n";
		cout << "\tName: " << newturn.name << "\n";
		cout << "\tStudent ID: " << newturn.studentId << "\n";
		cout << "\tGender: ";
		if (newturn.gender == 0) cout << "female\n";
		else cout << "male\n";
		cout << "\tDate of birth: " << newturn.dob.day << " "
			<< newturn.dob.month << " " << newturn.dob.year << "\n";
		cout << "\tClass: " << newturn.myClass << "\n";
		cout << "\tCourses: " << newturn.numberOfCourse << "\n";
		if (newturn.myCourse != nullptr) {
			CourseInfo* courseInfo = newturn.myCourse;
			while (courseInfo != nullptr) {
				cout << "\t " << courseInfo->academicYear - 1 << "-"
					<< courseInfo->academicYear << ", "
					<< courseInfo->semester << " semester, "
					<< courseInfo->courseName << "\n";
				courseInfo = courseInfo->next;
			}
			cout << "\n";
		}
		CourseInfo* cur = newturn.myCourse;
		deleteCourseInfo(cur);
	}
}
