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

	// Try to open file at given filepath
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

	// Read all information from csv
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
		currentStudent->gender = (gender == "male") ? 1 : 0;
		currentStudent->dob = getDob(dob);
		currentStudent->numberOfCourse = 0;
		currentStudent->myCourse = nullptr;
		currentStudent->next = nullptr;
	}
	in.close();
	// Write to database
	writeClassToFile(studentList, className);
	addClass(className);
	addStudentUsers(studentList);
	deleteStudentList(studentList);
	cout << "Import succesful. You can find the database at folder Database/Class.\n\n";
}
