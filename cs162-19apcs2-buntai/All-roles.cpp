#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========

// 1.1

void login(string& userName, int& userRole) {
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
		return;
	}
	while (getline(in, checkName)) {
		getline(in, checkPassword);
		in >> type;
		in.ignore();
		if (userName == checkName && userPassword == checkPassword) {
			userRole = type;
			in.close();
			cout << "\tWelcome " << userName << ".\n\n";
			return;
		}
		if (userName == checkName && userPassword != checkPassword) {
			in.close();
			cout << "Login failed. Error: Wrong password for " << userName << ".\n\n";
			return;
		}
	}
	in.close();
	cout << "Login failed. Error: Username not found.\n\n";
}

// 1.2

void showMenu(int& userRole) {
	int chon;
	if (userRole == 0) {
		showStaffMenu();
	}
	else if (userRole == 1) {
		showLecturerMenu();
	}
	else if (userRole == 2) {
		showStudentMenu();
	}
}


//1.3

void viewProfileInfo(string& userName, int& userRole) {
	if (userRole == 0) {
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
				{
					//print out info 
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newTurn.username << endl;
					cout << "\tFullname: " << newTurn.name << endl;
					if (newTurn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					return;
				}
			}
			in.close();
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
				//read data of each user in file 
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
				if (userName == newTurn.username)
				{
					//print out info
					cout << "User profile: " << endl;
					cout << "\tUsername: " << newTurn.username << endl;
					cout << "\tFullname: " << newTurn.name << endl;
					cout << "\tTitle: " << newTurn.title << endl;
					if (newTurn.gender == 0) cout << "\tGender: Female" << endl;
					else cout << "\tGender: Male" << endl;
					cout << "\tCourses: " << newTurn.totalCourse << endl;
					if (newTurn.totalCourse != 0) {
						CourseInfo* courseInfo = newTurn.myCourse;
						while (courseInfo!=nullptr) {
							cout << "\t " << courseInfo->academicYear - 1 << "-"
								<< courseInfo->academicYear << ", "
								<< courseInfo->semester << " semester, "
								<< courseInfo->courseName << endl;
							courseInfo = courseInfo->next;
						}
						deleteCourseInfo(courseInfo);
					}
					cout << endl;
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
		int count = 0;
		in.open("Database/Class/Classes.txt");
		count++;
		if (!in) cout << "Cannot open class file, please try it later" << endl;
		else (in >> newTurn.myClass);
		in.close();
		while (findStudentInfoFromFile(newTurn, userName) == false) {
			ifstream in;
			in.open("Database/Class/Classes.txt");
			while (in) {
				for (int i = 0; i < count; i++) in >> newTurn.myClass;
			}
			in.close();
		}
		//print out info
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
		cout << "\tDate of birth: " << newTurn.dob.day << " "
			<< newTurn.dob.month << " " << newTurn.dob.year << "\n";
		cout << "\tClass: " << newTurn.myClass << "\n";
		cout << "\tCourses: " << newTurn.numberOfCourse << "\n";
		if (newTurn.numberOfCourse != 0) {
			CourseInfo* courseInfo = newTurn.myCourse;
			while (courseInfo != nullptr) {
				cout << "\t " << courseInfo->academicYear - 1 << "-"
					<< courseInfo->academicYear << ", "
					<< courseInfo->semester << " semester, "
					<< courseInfo->courseName << endl;
				courseInfo = courseInfo->next;
			}
			deleteCourseInfo(courseInfo);
		}
		cout << endl;
		in.close();
		return;
	}
}

//1.4
void changePassword(string& userName) {
	string userPassword = "", checkName, checkPassword, newPassword, confirmedPassword;
	string* userlist[1000];
	cout << "\tPlease enter your password: ";
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

	//check password and find position of password in file User.txt
	ifstream in;
	in.open("Database/User.txt");
	int count = 0,n=0;
	if (!in) {
		cout << "Login failed. Error: User file is missing, please try again later.\n\n";
		return;
	}

	while (getline(in, checkName)) {
		count++; n++;
		getline(in, checkPassword); count++; n++;
		in.ignore(); count++; n++;
		in.ignore();
		if (userName == checkName && userPassword == checkPassword) {
			//continue to count the number of lines of file text
			while (in)
			{
				in.ignore(); n++;
				in.ignore();  n++;
				in.ignore();  n++;
				in.ignore();  
			}
			in.close();

			cout << "\tPlease enter your new password:" ;
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
			cout << "\n\n";

			cout << "\tEnter again your new password:" ;
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
				// in file text, replace old password by new password

				//create a array to store each line of file text, replace the line store old password by new password
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

				//rewrite User.txt
				ofstream out;
				out.open("Database/User.txt");
				for (int i = 0; i < n; i++) {
					out << userFile[i] << endl;
				}
				out.close();
				cout << "Your password has been changed successfully" << endl;
				delete[]userFile;
			}
			else cout << "The new password entered again is incorrect" << endl;
			return;
		}
		else if (userName == checkName && userPassword != checkPassword) {
			in.close();
			cout << " Wrong password for " << userName << ".\n\n";
			return;
		}
	}
}

//1.5 
void logout(string& userName, int& userRole) {
	int choice;
	cout << "Do you want to return to login menu or exist the program ?" << endl;
	cout << "\t1.Login menu" << endl;
	cout << "\t2.Exist program" << endl;
	cout << "\tYour choice" << endl;
	cin >> choice;
	if (choice == 1) {
		system("cls");
		login(userName, userRole);
	}
}