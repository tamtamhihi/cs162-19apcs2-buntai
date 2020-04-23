#include "Function.h"


// ========= STAFF'S FUNCTIONS DEFINITION =========


void importStudentFromCsv() {
	string filepath, className; // store the path to CSV file
	cout << "Class: ";
	cin >> className; // store class name
	toUpper(className);
	cout << "Enter the path to CSV file: ";
	cin >> filepath;
	cout << "\n";

	// Try to open file at given filepath.
	ifstream in;
	in.open(filepath);
	while (!in.is_open()) {
		cout << "The file path you entered is not valid.\n";
		cout << "Please input another path or ""0"" to stop: ";
		cin >> filepath;
		cout << "\n";
		if (filepath == "0")
			return;
		in.open(filepath);
	}

	// Read all information from csv.
	Student* studentList = nullptr; // Student list
	string row, no, studentId, lastName, firstName, gender, dob;

	// Count the columns to make sure the csv is in the right format.
	getline(in, row);
	stringstream columnNames(row);
	int columnCount = 0;
	while (getline(columnNames, no, ','))
		columnCount++;
	if (columnCount != 6) {
		cout << "Import unsuccesful. Error: The number of columns is not compatible.\n\n";
		in.close();
		return;
	}
	Student* currentStudent = studentList;
	while (getline(in, row)) {
		stringstream thisRow(row);
		getline(thisRow, no, ',');
		getline(thisRow, studentId, ',');
		getline(thisRow, lastName, ',');
		getline(thisRow, firstName, ',');
		getline(thisRow, dob, ',');
		getline(thisRow, gender, ',');
		toLower(gender);
		if (studentList == nullptr) {
			studentList = new Student;
			currentStudent = studentList;
		}
		else {
			currentStudent->next = new Student;
			currentStudent = currentStudent->next;
		}
		currentStudent->username = getValidUsername(lastName + " " + firstName);
		currentStudent->password = toPassword(getDob(dob));
		currentStudent->status = 1;
		currentStudent->name = lastName + " " + firstName;
		currentStudent->studentId = studentId;
		currentStudent->gender = (gender == "male") ? MALE : FEMALE;
		currentStudent->dob = getDob(dob);
		currentStudent->numberOfCourse = 0;
		currentStudent->myCourse = nullptr;
		currentStudent->next = nullptr;
	}
	in.close();
	
	// Write to database.
	writeClassToFile(studentList, className);
	addClass(className);
	addStudentUsers(studentList);
	deleteStudentList(studentList);
	cout << "Import succesful. You can find the database at folder Database/Class.\n\n";
}

void manuallyAddStudent() {
	// Ask for information in one line.
	cout << "Please input the following information with the same format:\n";
	cout << "<studentId>,<full-name>,<class-name>,<birthday yyyy-mm-dd>,<female/male>\n\t";
	string row, studentId, name, className, dob, gender;
	getline(cin, row);
	cout << "\n";

	// Retrieve each information.
	stringstream info(row);
	getline(info, studentId, ',');
	getline(info, name, ',');
	name = toFormalCase(name);
	getline(info, className, ',');
	toUpper(className);
	getline(info, dob, ',');
	Date Dob = getDob(dob);
	getline(info, gender, ',');
	toLower(gender);
	string username = getValidUsername(name), password = toPassword(Dob);
	int genderNum = (gender == "male") ? MALE : FEMALE;

	// Ask again.
	cout << "Student info:\n";
	cout << "\tStudent ID: " << studentId << "\n";
	cout << "\tName: " << name << "\n";
	cout << "\tClass: " << className << "\n";
	cout << "\tUsername: " << username << "\n";
	cout << "\tPassword: " << password << "\n";
	cout << "\tDate of birth: " << Dob.day << "-" << Dob.month << "-" << Dob.year << "\n";
	cout << "\tGender: " << gender << "\n";
	cout << "Do you want to add this student? Y/N\n";
	cin >> row;
	cout << "\n";
	toUpper(row);
	if (row == "N")
		return;

	// Save to database.
	ofstream out("Database/Class/" + className + ".txt", ios::app);
	out << username << "\n" << password << "\n" << 1 << "\n";
	out << name << "\n" << studentId << "\n" << genderNum << "\n";
	out << Dob.day << " " << Dob.month << " " << Dob.year << "\n";
	out << 0 << "\n\n";
	out.close();

	// Edit other related files.
	addClass(className);
	addUser(username, password, STUDENT);

	cout << "Student added successfully.\n\n";
}