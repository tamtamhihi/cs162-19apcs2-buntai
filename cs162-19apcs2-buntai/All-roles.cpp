#include "Function.h"

// ========= ALL-ROLES' FUNCTIONS DEFINITION =========

// 1.1

void login(string& username, int& userrole) {
	string userpassword = "", checkname, checkpassword;
	int type;

	// Get login info from user.
	cout << "Please add login information:" << endl;
	cout << "\tUsername:";
	getline(cin, username);
	cout << "\tPassword:";
	char c = _getch();
	while (c != '\n') {
		_putch('*');
		userpassword += c;
		c = _getch();
	}
	cout << endl;

	// Check the login information.
	ifstream in;
	in.open("Database/User.txt");
	if (!in) {
		cout << "\tLogin failed. Error: User file is missing, please try again later.\n\n";
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
			cout << "\tLogin failed. Error: Wrong password for " << username << ".\n\n";
			return;
		}
	}
	in.close();
	cout << "\tLogin failed. Error: Username not found.\n\n";
}

// 1.2

void showMenu(int& userrole) {
	int chon;
	if (userrole == 0) {
		staffmenu();
	}
	else if (userrole == 1) {
		lecturermenu();
	}
	else if (userrole == 2) {
		studentmenu();
	}
}
void staffMenu() {
	int choice;
	int chon;
	cout << "\t1.Class" << endl;
	cout << "\t2.Course" << endl;
	cout << "\t3.Scoreboard" << endl;
	cout << "\tAttendance list" << endl;
	cout << "Which object do you want to work with ?(please enter a number)" << endl;
	cin >> choice;
	if (choice == 1) {
		cout << "1. Import students of a class from a csv file" << endl;
		cout << "2. Manually add a new student to a class." << endl;
		cout << "3. Edit an existing student." << endl;
		cout << "4. Remove a student." << endl;
		cout << "5. Change students from class A to class B" << endl;
		cout << "6. View list of classes." << endl;
		cout << "7. View list of students in a class." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		default:
			break;
		}
	}
	else if (choice == 2) {
		cout << "1. Create / update / delete / view academic years and semesters" << endl;
		cout << "2. From a semester, import courses from a csv file." << endl;
		cout << "3. Manually add a new course." << endl;
		cout << "4. Edit an existing course." << endl;
		cout << "5. Remove a course." << endl;
		cout << "6. Remove a specific student from a course." << endl;
		cout << "7. Add a specific student to a course." << endl;
		cout << "8. View list of courses in the current semester." << endl;
		cout << "9. View list of students of a course." << endl;
		cout << "10. View attendance list of a course." << endl;
		cout << "11. Create / update / delete / view all lecturers." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		default:
			break;
		}
	}
	else if (choice == 3) {
		cout << "1. Search and view the scoreboard of a course." << endl;
		cout << "2. Export a scoreboard to a csv file." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		default:
			break;
		}
	}
	else if (choice == 4) {
		cout << "1.Search and view attendance list of a course." << endl;
		cout << "2. Export a attendance list to a csv file." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		default:
			break;
		}
	}
	else cout << "Your input is not valid" << endl;
}
void lecturerMenu() {
	int choice;
	cout << "1. View list of courses in the current semester." << endl;
	cout << "2. View list of students of a course." << endl;
	cout << "3. View attendance list of a course." << endl;
	cout << "4. Edit an attendance." << endl;
	cout << "5. Import scoreboard of a course (midterm, final, lab, bonus) from a csv file." << endl;
	cout << "6. Edit grade of a student." << endl;
	cout << "7. VView a scoreboard." << endl;
	cout << "Which function do you want to perform ? (please enter a number)" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	default:
		break;
	}
}
void studentMenu() {
	int choice;
	cout << "1. Check-in." << endl;
	cout << "2. View check-in result." << endl;
	cout << "3. View schedules." << endl;
	cout << "4. View my scores of a course." << endl;
	cout << "Which function do you want to perform ? (please enter a number)" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	default:
		break;
	}
}