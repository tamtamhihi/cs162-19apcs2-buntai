#include "Function.h"


// ========= STAFF'S FUNCTIONS DEFINITION =========


// ====== STAFF - CLASS ======

// 2.1
void importStudentFromCsv() {
	cout << "Input the following info of new class:\n";
	string filepath, className; // store the path to CSV file
	cout << "\tClass: ";
	cin >> className; toUpper(className);
	cout << "\tPlease make sure the CSV file has 6 columns:\n";
	cout << "\tNo., Student ID, Last name, Full name, DOB(yyyy - mm - dd), Gender(male / female)\n";
	cout << "\tPath to CSV file: ";
	cin >> filepath;
	cout << "\n";

	// Try to open file at given filepath.
	ifstream in(filepath);
	while (!in.is_open()) {
		cout << "\tThe file path you entered is not valid.\n";
		cout << "\tPlease input another path or ""0"" to stop: ";
		cin >> filepath;
		cout << "\n";
		if (filepath == "0")
			return;
		in.open(filepath);
	}

	// Read all information from csv.
	Student* studentList = nullptr; // Student list
	string row, no, studentId, lastName, firstName, gender, dob, confirm;

	// Count the columns to make sure the csv is in the right format.
	getline(in, row);
	stringstream columnNames(row);
	int columnCount = 0;
	while (getline(columnNames, no, ','))
		columnCount++;
	if (columnCount != 6) {
		cout << "Import unsuccessful. Error: The number of columns is not compatible.\n\n";
		in.close();
		return;
	}

	// Read all students from CSV.
	Student* currentStudent = studentList;
	while (getline(in, row)) {
		stringstream thisRow(row);
		getline(thisRow, no, ',');
		getline(thisRow, studentId, ',');
		getline(thisRow, lastName, ',');
		getline(thisRow, firstName, ',');
		getline(thisRow, dob, ',');
		if (!isDateStringSuitable(dob)) {
			cout << "Import unsuccessful. Error: The date of birth column is not in format of yyyy-mm-dd.\n\n";
			deleteStudentList(studentList);
			return;
		}
		getline(thisRow, gender, ','); toLower(gender);
		if (gender != "male" && gender != "female") {
			cout << "Import unsuccessful. Error: The gender column is not female/male.\n\n";
			deleteStudentList(studentList);
			return;
		}
		if (studentList == nullptr) {
			studentList = new Student;
			currentStudent = studentList;
		}
		else {
			currentStudent->next = new Student;
			currentStudent = currentStudent->next;
		}
		currentStudent->username = getValidUsername(lastName + " " + firstName);
		currentStudent->password = toPassword(getDate(dob));
		currentStudent->status = 1;
		currentStudent->name = lastName + " " + firstName;
		currentStudent->studentId = studentId;
		currentStudent->gender = (gender == "male") ? MALE : FEMALE;
		currentStudent->dob = getDate(dob);
		currentStudent->numberOfCourse = 0;
		currentStudent->myCourse = nullptr;
		currentStudent->next = nullptr;
	}
	in.close();

	if (studentList == nullptr) {
		cout << "There's no students in this CSV. Please check and try again.\n\n";
		return;
	}

	// Print student list to confirm.
	cout << "\t\t\t\t\tSTUDENT LISTS\n\n";
	printStudentListTable(studentList);
	
	cout << "Continue importing this class? Y/N\t";
	cin >> confirm; toUpper(confirm);
	cout << "\n";
	if (confirm == "N") {
		deleteStudentList(studentList);
		cout << "Importing class cancelled.\n\n";
		return;
	}

	// Delete students with existent student ID.
	int failedStudent = 0, successfulStudent = 0;
	string failedStudentNo = "";
	currentStudent = studentList;
	Student* previousStudent = nullptr;
	int count = 0;
	while (currentStudent != nullptr) {
		++count;
		if (isStudentIdExist(currentStudent->studentId)) {
			failedStudent++;
			failedStudentNo += to_string(count) + ",";
			if (previousStudent == nullptr)
				studentList = currentStudent->next;
			else
				previousStudent->next = currentStudent->next;
			Student* temp = currentStudent;
			currentStudent = currentStudent->next;
			delete temp;
		}
		else {
			successfulStudent++;
			previousStudent = currentStudent;
			currentStudent = currentStudent->next;
		}
	}

	// Check if class already exists. If yes, add new users to User.txt
	// and register new students for existent courses.
	if (isClassExist(className)) {
		cout << "This class already exists. We're appending the new students to the existing class.\n\n";
		cout << "Do you want to continue? Y/N \t";
		string confirm;
		cin >> confirm; toUpper(confirm);
		cout << "\n";
		if (confirm == "N") {
			deleteStudentList(studentList);
			cout << "Importing students cancelled.\n\n";
			return;
		}
		cout << "\tAdding users to User.txt...\n";
		addStudentUsers(studentList);

		// Register existent courses with default class as this class.
		cout << "\tRegistering existent default courses of class...\n";
		AcademicYear* academicYears = nullptr;
		readAcademicYearsFromFile(academicYears);
		AcademicYear* currentYear = academicYears;
		while (currentYear != nullptr) {
			string sem;
			stringstream semesterNames(currentYear->semester);
			while (getline(semesterNames, sem, ',')) {
				CourseInfo* courseList = nullptr;
				string ay = to_string(currentYear->academicYear) + "-"
					+ to_string(currentYear->academicYear + 1);
				readCourseListFromFile(courseList, ay, sem);
				CourseInfo* currentCourse = courseList;
				while (currentCourse != nullptr) {
					if (currentCourse->defaultClass == className)
						registerCourseForStudentList(studentList, currentCourse);
					currentCourse = currentCourse->next;
				}
				currentYear = currentYear->next;
				deleteCourseInfo(courseList);
			}
		}
		deleteAcademicYears(academicYears);

		// Read existent students of class.
		Student* existentClass = nullptr;
		readClassFromFile(className, existentClass);
		Student* currentStudent = existentClass;
		while (currentStudent->next != nullptr)
			currentStudent = currentStudent->next;
		currentStudent->next = studentList;
		studentList = existentClass;
	}
	else {
		cout << "\tAdding users to User.txt...\n";
		addStudentUsers(studentList);
		addClass(className);
	}
	writeClassToFile(studentList, className);
	deleteStudentList(studentList);
	if (!failedStudentNo.empty())
		failedStudentNo.pop_back(); // Pop the last comma.

	cout << "\n";
	cout << "Import successful " << successfulStudent << " student";
	if (successfulStudent) cout << "s.\n";
	else cout << ".\n";
	cout << "Import failed " << failedStudent << " student";
	if (failedStudent) cout << "s. No.: " << failedStudentNo << "\n\tError: Student ID already exists.\n";
	else cout << ".\n";
	if (successfulStudent)
		cout << "\nYou can find the database at folder Database/Class.\n\n";
}

// 2.2
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
	getline(info, name, ','); name = toFormalCase(name);
	getline(info, className, ','); toUpper(className);
	getline(info, dob, ',');
	while (!isDateStringSuitable(dob)) {
		cout << "The date of birth is not in format of yyyy-mm-dd. Please input again or ""0"" to stop: ";
		getline(cin, dob);
		if (dob == "0") {
			cout << "\nAdding student cancelled.\n\n";
			return;
		}
	}
	Date Dob = getDate(dob);
	getline(info, gender, ','); toLower(gender);
	while (gender != "male" && gender != "female") {
		cout << "The gender is not male/female. Please input again or ""0"" to stop: ";
		getline(cin, gender); toLower(gender);
		if (gender == "0") {
			cout << "\nAdding student cancelled.\n\n";
			return;
		}
	}
	string username = getValidUsername(name), password = toPassword(Dob);
	int genderNum = (gender == "male") ? MALE : FEMALE;
	cout << "\n";

	// Ask again.
	cout << "Student info:\n";
	cout << "\tStudent ID: " << studentId << "\n";
	cout << "\tName: " << name << "\n";
	cout << "\tClass: " << className << "\n";
	cout << "\tUsername: " << username << "\n";
	cout << "\tPassword: " << password << "\n";
	cout << "\tDate of birth: " << Dob.day << "-" << Dob.month << "-" << Dob.year << "\n";
	cout << "\tGender: " << gender << "\n";
	cout << "Do you want to add this student? Y/N\t";
	cin >> row;	toUpper(row);
	cout << "\n";
	if (row == "N")
		return;

	// Check if studentId exists.
	if (isStudentIdExist(studentId)) {
		cout << "Adding student failed. Error: Student ID already exists.\n\n";
		return;
	}
	if (isClassExist(className)) {
		cout << "This class already exists. We're appending the new student to the existing class.\n\n";
		cout << "Do you want to continue? Y/N \t";
		string confirm;
		cin >> confirm; toUpper(confirm);
		cout << "\n";
		if (confirm == "N") {
			cout << "Adding student cancelled.\n\n";
			return;
		}
		cout << "\tAdding user to User.txt...\n";
		addUser(username, password, STUDENT);

		Student* students = new Student;
		students->studentId = studentId;
		students->username = username;
		students->name = name;
		students->dob = Dob;
		students->status = 1;
		students->gender = genderNum;
		students->numberOfCourse = 0;
		students->myCourse = nullptr;
		students->next = nullptr;

		// Register existent courses with default class as this class.
		cout << "\tRegistering existent default courses of class...\n";
		AcademicYear* academicYears = nullptr;
		readAcademicYearsFromFile(academicYears);
		AcademicYear* currentYear = academicYears;
		while (currentYear != nullptr) {
			string sem;
			stringstream semesterNames(currentYear->semester);
			while (getline(semesterNames, sem, ',')) {
				CourseInfo* courseList = nullptr;
				string ay = to_string(currentYear->academicYear) + "-"
					+ to_string(currentYear->academicYear + 1);
				readCourseListFromFile(courseList, ay, sem);
				CourseInfo* currentCourse = courseList;
				while (currentCourse != nullptr) {
					if (currentCourse->defaultClass == className)
						registerCourseForStudentList(students, currentCourse);
					currentCourse = currentCourse->next;
				}
				currentYear = currentYear->next;
				deleteCourseInfo(courseList);
			}
		}
		deleteAcademicYears(academicYears);

		// Read existent students of class.
		cout << "\tReading existent class file...\n";
		Student* existentClass = nullptr;
		readClassFromFile(className, existentClass);
		Student* currentStudent = existentClass;
		while (currentStudent->next != nullptr)
			currentStudent = currentStudent->next;
		currentStudent->next = students;
		students = existentClass;
		cout << "\tWriting to class file...\n";
		writeClassToFile(students, className);
		deleteStudentList(existentClass);
	}
	else {
		cout << "\tAdding user to User.txt...\n";
		addUser(username, password, STUDENT);
		cout << "\tCreating new class file...\n";
		addClass(className);
		// Save to new file.
		cout << "\tSaving to class file...\n";
		ofstream out("Database/Class/" + className + ".txt");
		out << username << "\n" << password << "\n" << 1 << "\n";
		out << name << "\n" << studentId << "\n" << genderNum << "\n";
		out << Dob.day << " " << Dob.month << " " << Dob.year << "\n";
		out << 0 << "\n\n";
		out.close();
	}
	cout << "\nStudent added successfully.\n\n";
}

// 2.3
void editExistingStudent() {
	// Ask for class and student ID.
	cout << "Please input student class with the same format:\n";
	cout << "<class-name>,<studentId>\n\t";
	string row, className, id, password;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, className, ',');
	toUpper(className);
	getline(info, id);

	// Check if class exists.
	if (!isClassExist(className)) {
		cout << "Edit failed. Error: Can't find class.\n\n";
		return;
	}

	// Read all students' information in class.
	Student* studentList = nullptr;
	readClassFromFile(className, studentList);
	Student* current = studentList, * editedStudent = nullptr;
	while (current != nullptr) {
		if (current->studentId == id) {
			editedStudent = current;
			break;
		}
		current = current->next;
	}
	if (editedStudent == nullptr || editedStudent->status == 0) {
		cout << "Edit failed. Error: Can't find student in class.\n\n";
		return;
	}

	// Find password
	editedStudent->password = findPasswordFromUsername(editedStudent->username);

	// Confirm student info.
	printStudentInfo(editedStudent);

	// Ask for field to edit and prompt editing.
	cout << "What field do you want to edit?\n";
	cout << "\t1-Name\n\t2-Gender\n\t3-Date of birth\n";
	cout << "Input in increasing order with a space between.\n\t";
	getline(cin, row);
	cout << "\n";
	info = stringstream(row);
	int choice = 0;
	while (info >> choice) {
		if (choice == 1) {
			string name;
			cout << "New name: ";
			getline(cin, name);
			name = toFormalCase(name);
			cout << "Do you want to change name from "
				<< editedStudent->name << " to " << name << "? Y/N\n\t";
			cin >> row;
			cout << "\n";
			toUpper(row);
			if (row == "Y")
				editedStudent->name = name;
		}
		else if (choice == 2) {
			cout << "Do you want to change gender of this student? Y/N\n\t";
			cin >> row;
			cout << "\n";
			toUpper(row);
			if (row == "Y") {
				if (editedStudent->gender == FEMALE)
					editedStudent->gender = MALE;
				else
					editedStudent->gender = FEMALE;
			}
		}
		else {
			cout << "New date of birth yyyy-mm-dd: ";
			cin >> row;
			Date Dob = getDate(row);
			cout << "Do you want to change date of birth from "
				<< editedStudent->dob.day << "-"
				<< editedStudent->dob.month << "-"
				<< editedStudent->dob.year << " to "
				<< Dob.day << "-" << Dob.month << "-" << Dob.year << "? Y/N\n\t";
			cin >> row;
			cout << "\n";
			toUpper(row);
			if (row == "Y")
				editedStudent->dob = Dob;
		}
	}

	// Edit in course files that this student enrolls.
	CourseInfo* currentCourse = editedStudent->myCourse;
	while (currentCourse != nullptr) {
		Course* course = new Course;
		readCourseFromFile(currentCourse, course);
		Student* currentStudent = course->students;
		while (currentStudent != nullptr) {
			if (currentStudent->studentId == editedStudent->studentId)
				break;
			currentStudent = currentStudent->next;
		}
		// Edit 3 fields that might be edited.
		currentStudent->name = editedStudent->name;
		currentStudent->gender = editedStudent->gender;
		currentStudent->dob = editedStudent->dob;
		currentCourse = currentCourse->next;
		writeCourseToFile(course);
		deleteCourse(course);
	}

	// Save students to class file.
	writeClassToFile(studentList, className);

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "Edit student successfully.\n\n";
}

// 2.4
void removeStudent() {
	// Ask for class and student ID.
	cout << "Please input student class and student ID with the same format:\n";
	cout << "<class-name>,<studentId>\n\t";
	string row, className, id;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, className, ',');
	toUpper(className);
	getline(info, id);

	// Check if class exists.
	if (!isClassExist(className)) {
		cout << "Edit failed. Error: Can't find class.\n\n";
		return;
	}

	// Read all students' information in class.
	Student* studentList = nullptr;
	readClassFromFile(className, studentList);
	Student* current = studentList, * removeStudent = nullptr;
	while (current != nullptr) {
		if (current->studentId == id) {
			removeStudent = current;
			break;
		}
		current = current->next;
	}
	if (removeStudent == nullptr || removeStudent->status == 0) {
		cout << "Edit failed. Error: Can't find student in class.\n\n";
		return;
	}

	// Change status of student
	removeStudent->status = 0;

	// Save students to class file.
	writeClassToFile(studentList, className);

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "Remove student successfully.\n\n";
}

// 2.5 
void changeStudentClass() {
	// Ask for the input.
	cout << "Please input student's ID, old class and new class of that student with the same format:\n";
	cout << "<studentID>,<oldClass>,<newClass>\n\t";
	string row, id, oldClass, newClass;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, id, ',');
	getline(info, oldClass, ',');
	toUpper(oldClass);
	getline(info, newClass);
	toUpper(newClass);

	// Check if old class exists.
	if (!isClassExist(oldClass)) {
		cout << "Edit failed. Error: Can't find old class.\n\n";
		return;
	}

	// Check if new class exists.
	if (!isClassExist(newClass)) {
		cout << "Edit failed. Error: Can't find new class.\n\n";
		return;
	}

	// Find student in old class.
	Student* studentList = nullptr;
	readClassFromFile(oldClass, studentList);
	Student* current = studentList, * changeStudent = nullptr;
	while (current != nullptr) {
		if (current->studentId == id) {
			changeStudent = current;
			break;
		}
		current = current->next;
	}
	if (changeStudent == nullptr || changeStudent->status == 0) {
		cout << "Edit failed. Error: Can't find student in old class.\n\n";
		return;
	}

	// Change status of the student in old class.
	changeStudent->status = 0;

	// Save old class file.
	writeClassToFile(studentList, oldClass);
	
	// Write info of student to new class.
	ofstream out;
	out.open("Database/Class/" + newClass + ".txt", ios::app);
	out << changeStudent->username <<"\n";
	out << 1 <<"\n";
	out << changeStudent->name << "\n";
	out << changeStudent->studentId << "\n";
	out << changeStudent->gender << "\n";
	out << changeStudent->dob.day << " "
		<< changeStudent->dob.month << " "
		<< changeStudent->dob.year << "\n";
	out << changeStudent->numberOfCourse << "\n";
	CourseInfo* changeCourseInfo = changeStudent->myCourse;
	while (changeCourseInfo != nullptr) {
		out << changeCourseInfo->academicYear << " "
			<< changeCourseInfo->academicYear + 1 << " "
			<< changeCourseInfo->semester << " "
			<< changeCourseInfo->courseName << "\n"
			<< changeCourseInfo->defaultClass << "\n";
		changeCourseInfo = changeCourseInfo->next;
	}
	out << "\n";
	out.close();

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "Change class of student successfully." << endl;
}

// 2.6
void viewListOfClasses() {
	ifstream in;
	in.open("Database/Class/Classes.txt");

	if (in.is_open()) {
		cout << "List of classes:\n";
		string className;
		while (in >> className)
			cout << "\t" << className << endl;
		cout << endl;

		in.close();
	}
}

// 2.7 
void viewListOfStudentInAClass() {
	// Ask for class name.
	cout << "Please enter class name: ";
	string className;
	getline(cin, className);

	// Check if class exists.
	if (!isClassExist(className)) {
		cout << "Edit failed. Error: Can't find class.\n\n";
		return;
	}

	// Read all students' information in class.
	Student* studentList = nullptr;
	readClassFromFile(className, studentList);

	// Print all students' name in class.
	Student* studentCur = studentList;
	cout << "The list of students in class " << className << " :\n";
	while (studentCur != nullptr) {
		if(studentCur->status == 1)
		    cout << "\t" << studentCur->name << endl;
		studentCur = studentCur->next;
	}
	cout << "The list has been loaded successfully.\n\n";

	// Delete linked lists.
	deleteStudentList(studentList);
}


// ====== STAFF - COURSE ======

// 3.1
void manipulateAcademicYearsAndSemester() {
	int academicYear;
	string semester, action;
	cout << "Please input the academic year:\n";
	cout << "\t(input 2018 for AY 2018-2019)\n\t";
	cin >> academicYear;
	cout << "\n";
	cout << "Input the semester name:\n";
	cout << "\t(Summer/Fall/Spring or None to view AY only)\n\t";
	cin >> semester; 
	semester = toFormalCase(semester);
	if (semester != "Summer" && semester != "Fall" && semester != "Spring" && semester != "None") {
		cout << "Error: Semester input is not valid. Please try again.\n\n";
		cout << semester;
		return;
	}
	cout << "\nInput action:\n";
	cout << "\tC | Create\n";
	cout << "\tD | Delete\n";
	cout << "\tV | View\n";
	cout << "Your choice: ";
	cin >> action; toUpper(action);
	cout << "\n";
	
	string confirm;
	if (action == "C") {
		if (semester == "None") {
			cout << "Do you want to create directory for academic year " 
				<< academicYear << "-" << academicYear + 1 << "?\n\t";
			cout << "Y/N? ";
			cin >> confirm; toUpper(confirm);
			cout << "\n";
			if (confirm == "N") {
				cout << "Adding academic year cancelled.\n\n";
				return;
			}
			addAcademicYear(academicYear);
		}
		else {
			cout << "Do you want to create directory for semester " << semester << " of "
				<< academicYear << "-" << academicYear + 1 << "?\n\t";
			cout << "Y/N? ";
			cin >> confirm; toUpper(confirm);
			cout << "\n";
			if (confirm == "N") {
				cout << "Adding semester cancelled.\n\n";
				return;
			}
			addSemester(academicYear, semester);
		}
	}
	else if (action == "D") {
		if (semester == "None") {
			cout << "Do you want to delete directory for academic year "
				<< academicYear << "-" << academicYear + 1 << "?\n\t"
				<< "By deleting, you are losing all courses and semesters information\n"
				<< "\tof this academic year.\n\t"
				<< "All enrolled students and lecturers of any course in this academic year\n"
				<< "\twill be unenrolled and lose all information of attendance and scores.\n";
			cout << "Y/N? ";
			cin >> confirm; toUpper(confirm);
			cout << "\n";
			if (confirm == "N") {
				cout << "Deleting academic year cancelled.\n\n";
				return;
			}
			deleteAcademicYear(academicYear);
		}
		else {
			cout << "Do you want to delete directory for semester " << semester << " of academic year "
				<< academicYear << "-" << academicYear + 1 << "?\n\t"
				<< "By deleting, you are losing all courses information\n"
				<< "\tof this semester.\n\t"
				<< "All enrolled students and lecturers of any course in this semester\n"
				<< "\twill be unenrolled and lose all information of attendance and scores.\n";
			cout << "Y/N? ";
			cin >> confirm; toUpper(confirm);
			cout << "\n";
			if (confirm == "N") {
				cout << "Deleting semester cancelled.\n\n";
				return;
			}
			deleteSemester(academicYear, semester);
		}
	}
	else {
		if (semester == "None") {
			if (!isAcademicYearExist(academicYear)) {
				cout << "View academic year failed. Error: Academic year does not exist.\n\n";
				return;
			}
			AcademicYear* academicYears = nullptr;
			readAcademicYearsFromFile(academicYears);
			AcademicYear* currentYear = academicYears;
			while (currentYear != nullptr) {
				if (currentYear->academicYear == academicYear)
					break;
				currentYear = currentYear->next;
			}
			cout << "Academic Year " << academicYear << "-" << academicYear + 1 << ":\n";
			cout << setw(25) << "Number of semester | " << currentYear->numberOfSemester << "\n";
			if (currentYear->numberOfSemester) {
				stringstream sem(currentYear->semester);
				string semesterName;
				getline(sem, semesterName, ',');
				cout << setw(25) << "Semester names | " << semesterName << "\n";
				while (getline(sem, semesterName, ','))
					cout << setw(25) << "| " << semesterName << "\n";
			}
			cout << "To view the courses of each semester, please view the specific semester.\n\n";
			deleteAcademicYears(academicYears);
		}
		else {
			if (!isSemesterExist(academicYear, semester)) {
				cout << "View semester failed. Error: Semester does not exist.\n\n";
				return;
			}
			CourseInfo* courseList = nullptr;
			readCourseListFromFile(courseList, to_string(academicYear) + "-" + to_string(academicYear + 1), semester);
			cout << "Semester " << semester << " AY " << academicYear << "-" << academicYear + 1 << ":\n";
			cout << "\t" << setw(15) << "Course ID |" << " Default class\n";
			cout << "\t" << setfill('-') << setw(15) << "+" << setw(15) << " " << "\n";
			CourseInfo* currentCourse = courseList;
			while (currentCourse != nullptr) {
				cout << "\t" << setfill(' ') << setw(14) << currentCourse->courseName << "| " << currentCourse->defaultClass << "\n";
				currentCourse = currentCourse->next;
			}
			cout << "\n";
			deleteCourseInfo(courseList);
		}
	}
}

// 3.2
void importCourseFromCsv() {
	string filepath; // store the path to CSV file
	int academicYear;
	string semester;
	cout << "Academic year: ";
	cin >> academicYear; // store academic year
	cout << "Semester: ";
	cin >> semester; // store semester
	semester = toFormalCase(semester);
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
	Course* currentCourse = nullptr;
	string row, no, courseId, courseName, defautClass, lecturerName, 
		   startDate, endDate, sessionsPerWeek, dayOfWeek, startHour, endHour, room;

	// Count the columns to make sure the csv is in the right format.
	getline(in, row);
	stringstream columnNames(row);
	int columnCount = 0;
	while (getline(columnNames, no, ','))
		columnCount++;
	if (columnCount != 12) {
		cout << "Import unsuccesful. Error: The number of columns is not compatible.\n\n";
		in.close();
		return;
	}
	while (getline(in, row)) {
		stringstream thisRow(row);

		// Read from courseId to sessionsPerWeek information.
		getline(thisRow, no, ',');
		getline(thisRow, courseId, ',');
		getline(thisRow, courseName, ',');
		getline(thisRow, defautClass, ',');
		getline(thisRow, lecturerName, ',');
		getline(thisRow, startDate, ',');
		getline(thisRow, endDate, ',');
		getline(thisRow, sessionsPerWeek, ',');
		
		// Check if course exists.
		CourseInfo* courseInfo = new CourseInfo;
		courseInfo->academicYear = academicYear;
		courseInfo->semester = semester;
		courseInfo->courseName = courseId;
		courseInfo->defaultClass = defautClass;
		courseInfo->next = nullptr;
		if (isCourseExist(courseInfo)) {
			cout << "Import failed. Error: Course already exists.\n\n";
			deleteCourseInfo(courseInfo);
			return;
		}

		// Check if class exists.
		if (!isClassExist(defautClass)) {
			cout << "Import failed. Error: Can't find default class.\n\n";
			deleteCourseInfo(courseInfo);
			return;
		} 

		// Check if academic year exists.
		if (!isAcademicYearExist(academicYear)) {
			cout << "Import failed. Error: Academic year does not exist. Please create the corrensponding year first.\n\n";
			deleteCourseInfo(courseInfo);
			return;
		}

		// Check if semester exists.
		if (!isSemesterExist(academicYear, semester)) {
			cout << "Import failed. Error: Semester does not exist. Please create the corresponding semester first.\n\n";
			deleteCourseInfo(courseInfo);
			return;
		}

		currentCourse = new Course;
		currentCourse->academicYear = academicYear;
		currentCourse->semester = semester;
		currentCourse->courseId = courseId;
		currentCourse->courseName = courseName;
		currentCourse->defaultClass = defautClass;
		string lecturerUsername = toUsername(lecturerName);
		// If lecturer hasn't existed, make sure the new lecturer account doesn't overlap any other roles.
		if (!isLecturerExist(lecturerUsername))
			lecturerUsername = getValidUsername(lecturerUsername);
		currentCourse->lecturer.username = lecturerUsername;

		// Update Lecturer.txt.
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		Lecturer* currentLecturer = lecturers,* previousLecturer = nullptr;
		while (currentLecturer != nullptr) {
			if (currentLecturer->username == lecturerUsername) {
				CourseInfo* myCourse = currentLecturer->myCourse;
				if (currentLecturer->myCourse == nullptr) {
					currentLecturer->myCourse = new CourseInfo;
					myCourse = currentLecturer->myCourse;
				}
				else {
					while (myCourse->next != nullptr)
						myCourse = myCourse->next;
					myCourse->next = new CourseInfo;
					myCourse = myCourse->next;
				}
				myCourse->academicYear = courseInfo->academicYear;
				myCourse->semester = courseInfo->semester;
				myCourse->courseName = courseInfo->courseName;
				myCourse->defaultClass = courseInfo->defaultClass;
				myCourse->next = nullptr;
				currentLecturer->totalCourse++;
				break;
			}
			previousLecturer = currentLecturer;
			currentLecturer = currentLecturer->next;
		}
		if (currentLecturer == nullptr) {
			string title, gender, password;
			cout << "The lecturer does not exist. Please input his/her additional information with the same format:\n";
			cout << "<title>,<male/female>\n\t";
			string temp;
			cin.ignore();
			getline(cin, temp);
			stringstream TEMP(temp);
			getline(TEMP, title, ',');
			getline(TEMP, gender, ',');
			toLower(gender);
			password = toPasswordGeneral(lecturerName);
			addUser(lecturerUsername, password, 1);
			addLecturerFromNewCourse(currentCourse->lecturer, courseInfo);
			previousLecturer->next = new Lecturer;
			currentLecturer = previousLecturer->next;
			currentLecturer->username = lecturerUsername;
			currentLecturer->name = lecturerName;
			currentLecturer->title = title;
			currentLecturer->gender = (gender == "male") ? 1 : 0;
			currentLecturer->totalCourse = 1;
			currentLecturer->myCourse = new CourseInfo;
			currentLecturer->myCourse->academicYear = courseInfo->academicYear;
			currentLecturer->myCourse->semester = courseInfo->semester;
			currentLecturer->myCourse->courseName = courseInfo->courseName;
			currentLecturer->myCourse->defaultClass = courseInfo->defaultClass;
			currentLecturer->myCourse->next = nullptr;
			currentLecturer->next = nullptr;
		} // Check if lecturer exists, if not create a new account.
		writeLecturersToFile(lecturers);
		deleteLecturers(lecturers);

		currentCourse->startDate = getDate(startDate);
		currentCourse->endDate = getDate(endDate);
		currentCourse->sessionsPerWeek = stoi(sessionsPerWeek);
		
		// Read sessionInfo information.
		currentCourse->sessionInfo = nullptr;
		SessionInfo* currentSession = nullptr;
		thisRow.get();
		int i = currentCourse->sessionsPerWeek;
		for (int i = 0; i < currentCourse->sessionsPerWeek; ++i) {
			getline(thisRow, dayOfWeek, ',');
			if (currentCourse->sessionInfo == nullptr) {
				currentCourse->sessionInfo = new SessionInfo;
				currentSession = currentCourse->sessionInfo;
			}
			else {
				currentSession->next = new SessionInfo;
				currentSession = currentSession->next;
			}
			currentSession->day = dayToNumber(dayOfWeek);
			currentSession->next = nullptr;
		}
		thisRow.get();
		thisRow.get();
		thisRow.get();
		currentSession = currentCourse->sessionInfo;
		while (currentSession != nullptr) {
			getline(thisRow, startHour, ',');
			currentSession->startTime = getTime(startHour);
			currentSession = currentSession->next;
		}
		thisRow.get();
		thisRow.get();
		thisRow.get();
		currentSession = currentCourse->sessionInfo;
		while (currentSession != nullptr) {
			getline(thisRow, endHour, ',');
			currentSession->endTime = getTime(endHour);
			currentSession = currentSession->next;
		}
		thisRow.get();
		thisRow.get();

		// Read room information.
		getline(thisRow, room, ',');
		currentCourse->room = room;

		// Calculate total sessions.
		currentCourse->totalSessions = calculateTotalSessions(currentCourse);

		// Read enrolled students information.
		currentCourse->students = nullptr;
		readClassFromFile(defautClass, currentCourse->students);
		currentCourse->studentCourseInfo = nullptr;
		Student* currentStudent = currentCourse->students;
		StudentCourseInfo* currentStudentInfo = nullptr;
		
		while (currentStudent != nullptr) {
			// Update enrolled students information.
			currentStudent->numberOfCourse++;
			CourseInfo* currentStudentCourse = currentStudent->myCourse;
			if (currentStudent->myCourse == nullptr) {
				currentStudent->myCourse = new CourseInfo;
				currentStudentCourse = currentStudent->myCourse;
			}
			else {
				while (currentStudentCourse->next != nullptr)
					currentStudentCourse = currentStudentCourse->next;
				currentStudentCourse->next = new CourseInfo;
				currentStudentCourse = currentStudentCourse->next;
			}
			currentStudentCourse->academicYear = courseInfo->academicYear;
			currentStudentCourse->semester = courseInfo->semester;
			currentStudentCourse->courseName = courseInfo->courseName;
			currentStudentCourse->defaultClass = courseInfo->defaultClass;
			currentStudentCourse->next = nullptr;

			// Read enrolled students course information.
			if (currentCourse->studentCourseInfo == nullptr) {
				currentCourse->studentCourseInfo = new StudentCourseInfo;
				currentStudentInfo = currentCourse->studentCourseInfo;
			}
			else {
				currentStudentInfo->next = new StudentCourseInfo;
				currentStudentInfo = currentStudentInfo->next;
			}
			currentStudentInfo->midterm = 0;
			currentStudentInfo->final = 0;
			currentStudentInfo->lab = 0;
			currentStudentInfo->bonus = 0;
			currentStudentInfo->status = 1;
			currentStudentInfo->next = nullptr;
			currentStudentInfo->attendance = nullptr;
			Attendance* currentAttendance = nullptr;
			Date nextSession = currentCourse->startDate;
			int daysToNext;
			SessionInfo* currentSession = currentCourse->sessionInfo;
			// Link session info list circularly.
			while (currentSession != nullptr && currentSession->next != nullptr)
				currentSession = currentSession->next;
			currentSession->next = currentCourse->sessionInfo;
			currentSession = currentSession->next;
			for (int i = 0; i < currentCourse->totalSessions; ++i) {
				if (currentStudentInfo->attendance == nullptr) {
					currentStudentInfo->attendance = new Attendance;
					currentAttendance = currentStudentInfo->attendance;
				}
				else {
					currentAttendance->next = new Attendance;
					currentAttendance = currentAttendance->next;
				}
				currentAttendance->date = nextSession;
				currentAttendance->startTime = currentSession->startTime;
				currentAttendance->endTime = currentSession->endTime;
				currentAttendance->time = Time{ 0,0 };
				currentAttendance->next = nullptr;
				daysToNext = currentSession->next->day - currentSession->day;
				daysToNext += (daysToNext > 0) ? 0 : 7;
				nextSession = dateAfterDays(nextSession, daysToNext);
				currentSession = currentSession->next;
			}
			// Unlink circularly.
			currentSession = currentCourse->sessionInfo->next;
			while (currentSession->next != currentCourse->sessionInfo)
				currentSession = currentSession->next;
			currentSession->next = nullptr;
			currentStudent = currentStudent->next;
		}

		// Edit related files.
		// Update "Courses.txt" file.
		filepath = "Database/"
			+ to_string(academicYear) + "-"
			+ to_string(academicYear + 1) + "/"
			+ semester + "/Courses.txt";
		ofstream out;
		out.open(filepath, ios::app);
		out << courseId << " " << defautClass << "\n";
		out.close();
		// Update students information in default class.
		writeClassToFile(currentCourse->students, defautClass);

		// Write to database.
		writeCourseToFile(currentCourse);

		// Because the default delete function for course linked list 
		// does not delete course info of students.
		currentStudent = currentCourse->students;
		while (currentStudent != nullptr) {
			deleteCourseInfo(currentStudent->myCourse);
			currentStudent = currentStudent->next;
		}
		deleteCourse(currentCourse);
		deleteCourseInfo(courseInfo);
	}
	in.close();

	cout << "Import successful. You can find the database at folder Database/" 
		 << academicYear << "-" << academicYear + 1 << "/" << semester << ".\n\n";
}

// 3.3
void manuallyAddCourse() {
	// Ask for information.
	string row, academicYear, semester, courseId, courseName, defaultClass, lecturerName,
		startDate, endDate, sessionsPerWeek, dayOfWeek, startHour, endHour, room;
	stringstream info;
	cout << "Please input the following information with the same format:\n\n";
	cout << "<academic-year>,<semester>,<course-id>,<course-name>,<default-class>\n\t";
	getline(cin, row);
	cout << "\n";
	info << row;
	getline(info, academicYear, ',');
	getline(info, semester, ','); semester = toFormalCase(semester);
	getline(info, courseId, ','); toUpper(courseId);
	getline(info, courseName, ',');
	getline(info, defaultClass, ','); toUpper(defaultClass);
	info.clear();

	if (!isAcademicYearExist(stoi(academicYear))) {
		cout << "Adding course failed. Error: Academic year does not exist. Please create the corresponding academic year first.\n\n";
		return;
	}
	if (!isSemesterExist(stoi(academicYear), semester)) {
		cout << "Adding course failed. Error: Semester does not exist. Please create the corresponding semester first.\n\n";
		return;
	}

	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = stoi(academicYear);
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	if (isCourseExist(courseInfo)) {
		cout << "Adding course failed. Error: Course already exists.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}
	if (!isClassExist(defaultClass)) {
		cout << "Adding course failed. Error: Default class doest not exist.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}

	cout << "<lecturer-name>,<start-date yyyy-mm-dd>,<end-date yyyy-mm-dd>,<sessions-per-week>,<room>\n\t";
	getline(cin, row);
	cout << "\n";
	info << row;
	getline(info, lecturerName, ',');
	string lecturerUsername = toUsername(lecturerName);
	// If lecturer hasn't existed, make sure the new lecturer account does not overlap any other roles.
	if (!isLecturerExist(lecturerUsername))
		lecturerUsername = getValidUsername(lecturerUsername);
	getline(info, startDate, ',');
	getline(info, endDate, ',');
	getline(info, sessionsPerWeek, ',');
	getline(info, room, ',');
	info.clear();

	cout << "<day-of-week-1>,<start-hour-1>,<end-hour-1>,(...)\n\t";
	getline(cin, row);
	info << row;
	cout << "\n";

	// Continue retrieving.
	Course* courseTmp = new Course;
	courseTmp->academicYear = courseInfo->academicYear;
	courseTmp->semester = courseInfo->semester;
	courseTmp->courseId = courseInfo->courseName;
	courseTmp->courseName = courseName;
	courseTmp->defaultClass = defaultClass;
	courseTmp->startDate = getDate(startDate);
	courseTmp->endDate = getDate(endDate);
	courseTmp->lecturer.username = lecturerUsername;
	courseTmp->room = room;
	courseTmp->students = nullptr;
	courseTmp->studentCourseInfo = nullptr;
	courseTmp->sessionsPerWeek = stoi(sessionsPerWeek);
	courseTmp->sessionInfo = nullptr;
	SessionInfo* currentSession = nullptr;
	for (int i = 0; i < courseTmp->sessionsPerWeek; ++i) {
		getline(info, dayOfWeek, ',');
		getline(info, startHour, ',');
		getline(info, endHour, ',');
		if (courseTmp->sessionInfo == nullptr) {
			courseTmp->sessionInfo = new SessionInfo;
			currentSession = courseTmp->sessionInfo;
		}
		else {
			currentSession->next = new SessionInfo;
			currentSession = currentSession->next;
		}
		currentSession->day = dayToNumber(dayOfWeek);
		currentSession->startTime = getTime(startHour);
		currentSession->endTime = getTime(endHour);
		currentSession->next = nullptr;
	}

	// Ask again for confirmation.
	cout << "Course info:\n";
	cout << "\tAcademic year: " << academicYear << "\n";
	cout << "\tSemester: " << semester << "\n";
	cout << "\tCourse ID: " << courseId << "\n";
	cout << "\tCourse name: " << courseName << "\n";
	cout << "\tDefault class: " << defaultClass << "\n";
	cout << "\tLecturer Account: " << lecturerUsername << "\n";
	Date start = getDate(startDate), end = getDate(endDate);
	cout << "\tStart date: " << start.day << "-" << start.month << "-" << start.year << "\n";
	cout << "\tEnd date: " << end.day << "-" << end.month << "-" << end.year << "\n";
	cout << "\tSessions per week: " << sessionsPerWeek << "\n";
	cout << "\tSessions info:\n";
	cout << "\t" << setw(20) << "Day of week |" << setw(20) << "Start time |" << " End time\n";
	cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+" << setw(20) << " " << "\n";
	currentSession = courseTmp->sessionInfo;
	for (int i = 0; i < courseTmp->sessionsPerWeek; ++i) {
		string sessionStart = to_string(currentSession->startTime.hour) + ":"
			+ to_string(currentSession->startTime.minute);
		string sessionEnd = to_string(currentSession->endTime.hour) + ":"
			+ to_string(currentSession->endTime.minute);
		cout << "\t" << setfill(' ') << setw(19) << numToDay(currentSession->day) << "|"
			<< setw(19) << sessionStart << "| " << sessionEnd << "\n";
		currentSession = currentSession->next;
	}
	cout << "\tRoom: " << room << "\n";
	cout << "Do you want to add this course? Y/N\n";
	cin >> row;
	cout << "\n";
	toUpper(row);
	if (row == "N") {
		deleteCourseInfo(courseInfo);
		deleteCourse(courseTmp);
		return;
	}

	// Update Lecturer.txt.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers, * previousLecturer = nullptr;
	while (currentLecturer != nullptr) {
		if (currentLecturer->username == lecturerUsername) {
			CourseInfo* myCourse = currentLecturer->myCourse;
			if (currentLecturer->myCourse == nullptr) {
				currentLecturer->myCourse = new CourseInfo;
				myCourse = currentLecturer->myCourse;
			}
			else {
				while (myCourse->next != nullptr)
					myCourse = myCourse->next;
				myCourse->next = new CourseInfo;
				myCourse = myCourse->next;
			}
			myCourse->academicYear = courseInfo->academicYear;
			myCourse->semester = courseInfo->semester;
			myCourse->courseName = courseInfo->courseName;
			myCourse->defaultClass = courseInfo->defaultClass;
			myCourse->next = nullptr;
			currentLecturer->totalCourse++;
			break;
		}
		previousLecturer = currentLecturer;
		currentLecturer = currentLecturer->next;
	}
	if (currentLecturer == nullptr) {
		string title, gender, password;
		cout << "The lecturer does not exist. Please input his/her additional information with the same format:\n";
		cout << "<title>,<male/female>\n\t";
		string temp;
		cin.ignore();
		getline(cin, temp);
		stringstream TEMP(temp);
		getline(TEMP, title, ',');
		getline(TEMP, gender, ',');
		toLower(gender);
		password = toPasswordGeneral(lecturerName);
		addUser(lecturerUsername, password, 1);
		addLecturerFromNewCourse(courseTmp->lecturer, courseInfo);
		previousLecturer->next = new Lecturer;
		currentLecturer = previousLecturer->next;
		currentLecturer->username = lecturerUsername;
		currentLecturer->name = lecturerName;
		currentLecturer->title = title;
		currentLecturer->gender = (gender == "male") ? 1 : 0;
		currentLecturer->totalCourse = 1;
		currentLecturer->myCourse = new CourseInfo;
		currentLecturer->myCourse->academicYear = courseInfo->academicYear;
		currentLecturer->myCourse->semester = courseInfo->semester;
		currentLecturer->myCourse->courseName = courseInfo->courseName;
		currentLecturer->myCourse->defaultClass = courseInfo->defaultClass;
		currentLecturer->myCourse->next = nullptr;
		currentLecturer->next = nullptr;
	} // Check if lecturer exists, if not create a new account.
	writeLecturersToFile(lecturers);
	deleteLecturers(lecturers);

	// Calculate total sessions.
	courseTmp->totalSessions = calculateTotalSessions(courseTmp);

	// Read enrolled students information.
	readClassFromFile(defaultClass, courseTmp->students);
	courseTmp->studentCourseInfo = nullptr;
	Student* currentStudent = courseTmp->students;
	StudentCourseInfo* currentStudentInfo = nullptr;

	while (currentStudent != nullptr) {
		// Update enrolled students information.
		currentStudent->numberOfCourse++;
		CourseInfo* currentStudentCourse = currentStudent->myCourse;
		if (currentStudent->myCourse == nullptr) {
			currentStudent->myCourse = new CourseInfo;
			currentStudentCourse = currentStudent->myCourse;
		}
		else {
			while (currentStudentCourse->next != nullptr)
				currentStudentCourse = currentStudentCourse->next;
			currentStudentCourse->next = new CourseInfo;
			currentStudentCourse = currentStudentCourse->next;
		}
		currentStudentCourse->academicYear = courseInfo->academicYear;
		currentStudentCourse->semester = courseInfo->semester;
		currentStudentCourse->courseName = courseInfo->courseName;
		currentStudentCourse->defaultClass = courseInfo->defaultClass;
		currentStudentCourse->next = nullptr;

		// Read enrolled students course information.
		if (courseTmp->studentCourseInfo == nullptr) {
			courseTmp->studentCourseInfo = new StudentCourseInfo;
			currentStudentInfo = courseTmp->studentCourseInfo;
		}
		else {
			currentStudentInfo->next = new StudentCourseInfo;
			currentStudentInfo = currentStudentInfo->next;
		}
		currentStudentInfo->midterm = 0;
		currentStudentInfo->final = 0;
		currentStudentInfo->lab = 0;
		currentStudentInfo->bonus = 0;
		currentStudentInfo->status = 1;
		currentStudentInfo->next = nullptr;
		currentStudentInfo->attendance = nullptr;
		Attendance* currentAttendance = nullptr;
		Date nextSession = courseTmp->startDate;
		int daysToNext;
		SessionInfo* currentSession = courseTmp->sessionInfo;
		// Link session info list circularly.
		while (currentSession != nullptr && currentSession->next != nullptr)
			currentSession = currentSession->next;
		currentSession->next = courseTmp->sessionInfo;
		currentSession = currentSession->next;
		for (int i = 0; i < courseTmp->totalSessions; ++i) {
			if (currentStudentInfo->attendance == nullptr) {
				currentStudentInfo->attendance = new Attendance;
				currentAttendance = currentStudentInfo->attendance;
			}
			else {
				currentAttendance->next = new Attendance;
				currentAttendance = currentAttendance->next;
			}
			currentAttendance->date = nextSession;
			currentAttendance->startTime = currentSession->startTime;
			currentAttendance->endTime = currentSession->endTime;
			currentAttendance->time = Time{ 0,0 };
			currentAttendance->next = nullptr;
			daysToNext = currentSession->next->day - currentSession->day;
			daysToNext += (daysToNext > 0) ? 0 : 7;
			nextSession = dateAfterDays(nextSession, daysToNext);
			currentSession = currentSession->next;
		}
		// Unlink circularly.
		currentSession = courseTmp->sessionInfo->next;
		while (currentSession->next != courseTmp->sessionInfo)
			currentSession = currentSession->next;
		currentSession->next = nullptr;
		currentStudent = currentStudent->next;
	}
	writeClassToFile(courseTmp->students, defaultClass);
	writeCourseToFile(courseTmp);

	// Edit other related files.
	// Update "Courses.txt" file.
	string filepath = "Database/"
		+ academicYear + "-"
		+ to_string(stoi(academicYear) + 1) + "/"
		+ semester + "/Courses.txt";
	ofstream out;
	out.open(filepath, ios::app);
	out << courseId << " " << defaultClass << "\n";
	out.close();

	// Delete linked list.
	currentStudent = courseTmp->students;
	while (currentStudent != nullptr) {
		deleteCourseInfo(currentStudent->myCourse);
		currentStudent = currentStudent->next;
	}
	deleteCourse(courseTmp);
	deleteCourseInfo(courseInfo);

	cout << "Course added successfully.\n\n";
}

// 3.4
void editAnExistingCourse() {

}

// 3.5
void removeCourse() {
	// Ask for name of academic year, semester, course name.
	cout << "Please enter academic year, semester and name of the course like this example: \n";
	cout << "2019-2020,Summer,CS162\n\t";
	string row, academicYear, semester, courseID;
	getline(cin, row);
	cout << "\n";
	stringstream input(row);
	getline(input, academicYear, ',');
	getline(input, semester, ',');
	toFormalCase(semester);
	getline(input, courseID);
	toUpper(courseID);

	// Read courses.txt to find all default class of remove course.
	CourseInfo* courseList = nullptr;
	readCourseListFromFile(courseList, academicYear, semester);

	// Find remove course.
	if (courseList == nullptr) {
		cout << "Error: Cannot find the course.\n\n";
		return;
	}
	CourseInfo* currentCourseList = courseList;
	CourseInfo* previous = currentCourseList;
	while (currentCourseList->courseName != courseID) {
		previous = currentCourseList;
		currentCourseList = currentCourseList->next;
		if (currentCourseList == nullptr) {
			cout << "Error: Cannot find the course.\n\n";
			deleteCourseInfo(courseList);
			return;
		}
	}

	// Remove file and remove course in student course
	CourseInfo* tempCurrent = currentCourseList;
	while (currentCourseList->courseName == courseID) {

		// Remove all file <course-default>.txt
		string filePath= "Database/"
			+ academicYear + "/"
			+ semester + "/"
			+ courseID + "-" + currentCourseList->defaultClass
			+ ".txt";
		const char* filepath = filePath.c_str();
		remove(filepath);

		// Remove course in student course
		Student* studentList = nullptr;
		readClassFromFile(currentCourseList->defaultClass, studentList);
		Student* currentStudent = studentList;
		while (currentStudent != nullptr) {
			if (currentStudent->myCourse->courseName == courseID) {
				CourseInfo* temp = currentStudent->myCourse;
				currentStudent->myCourse = currentStudent->myCourse->next;
				delete temp;
			}
			else {
				CourseInfo* currentCourse = currentStudent->myCourse;
				while (currentCourse->next != nullptr) {
					if (currentCourse->next->courseName == courseID) {
						CourseInfo* temp;
						temp = currentCourse;
						currentCourse->next = currentCourse->next->next;
						delete temp;
					}
					currentCourse = currentCourse->next;
					if (currentCourse == nullptr) break;
				}
			}
			currentStudent = currentStudent->next;
		}
		writeClassToFile(studentList, currentCourseList->defaultClass);
		deleteStudentList(studentList);

		currentCourseList = currentCourseList->next;
		if (currentCourseList == nullptr) break;
	}

	// Remove given course in file Courses.txt
	currentCourseList = tempCurrent;
	while (currentCourseList->courseName == courseID) {
		if (currentCourseList == courseList) {
			courseList = courseList->next;
			delete currentCourseList;
			currentCourseList = courseList;
		}
		else {
			previous->next = currentCourseList->next;
			delete currentCourseList;
			currentCourseList = previous->next;
		}
		if (currentCourseList == nullptr) break;
	}
	writeCourseListToFile(courseList, academicYear, semester);

	// Delete courseList.
	deleteCourseInfo(courseList);

	// Annoucement
	cout << "Remove course successfully!\n";
}

// 3.8
void viewListOfCourses() {
	int academicYear;
	string semester;
	cout << "Academic year: ";
	cin >> academicYear;
	cout << "Semester: ";
	cin >> semester;

	string filepath = "Database/"
		+ to_string(academicYear) + "-"
		+ to_string(academicYear + 1) + "/"
		+ semester + "/";
	ifstream in(filepath + "Courses.txt");
	string row, courseId, defaultClass, courseName;
	cout << "List of courses in " << semester << " semester of academic year "
		 << academicYear << "-" << academicYear + 1 << ":\n";
	while (getline(in, row)) {
		stringstream info(row);
		info >> courseId >> defaultClass;
		ifstream fin(filepath + courseId + "-" + defaultClass + ".txt");
		getline(fin, courseId);
		getline(fin, courseName);
		cout << "\tCourse ID: " << courseId << "\n"
			 << "\tCourse name: " << courseName << "\n"
			 << "\tDefault class: " << defaultClass << "\n";
		cout << "\n";
		fin.close();
	}
	cout << "\n";

	in.close();
}

// 3.9
void viewListOfStudentsOfCourse() {
	// Get course information.
	int academicYear;
	string semester, courseId, defaultClass;
	cout << "Academic year: ";
	cin >> academicYear;
	cout << "Semester: ";
	cin >> semester;
	cout << "Course ID: ";
	cin >> courseId;
	cout << "Default class: ";
	cin >> defaultClass;

	// Check if course exists.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = academicYear;
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	if (!isCourseExist(courseInfo)) {
		cout << "View students list failed. Error: Course not found.\n\n";
		return;
	}
	
	// Read course information into a course.
	string filepath = "Database/"
		+ to_string(academicYear) + "-"
		+ to_string(academicYear + 1) + "/"
		+ semester + "/"
		+ courseId + "-"
		+ defaultClass + ".txt";
	ifstream in(filepath);
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);

	// Print students list.
	cout << "List of students enrolled in course " << courseId
		 << " of " << semester << " semester, academic year "
		 << academicYear << "-" << academicYear + 1 << ":\n";
	Student* currentStudent = course->students;
	int studentNum = 1;
	while (currentStudent != nullptr) {
		cout << "\t";
		cout << studentNum << ". ";
		cout << currentStudent->name << "\n";
		studentNum++;
		currentStudent = currentStudent->next;
	}
	cout << "\n";

	// Delete linked list.
	deleteCourseInfo(courseInfo);
	deleteCourse(course);

	in.close();
}

// 3.10
void viewAttendanceListOfCourse() {
	// Get course information.
	int academicYear;
	string semester, courseId, defaultClass;
	cout << "Academic year: ";
	cin >> academicYear;
	cout << "Semester: ";
	cin >> semester;
	cout << "Course ID: ";
	cin >> courseId;
	cout << "Default class: ";
	cin >> defaultClass;

	// Check if course exists.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = academicYear;
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	if (!isCourseExist(courseInfo)) {
		cout << "View students list failed. Error: Course not found.\n\n";
		return;
	}

	// Read course information into a course.
	string filepath = "Database/"
		+ to_string(academicYear) + "-"
		+ to_string(academicYear + 1) + "/"
		+ semester + "/"
		+ courseId + "-"
		+ defaultClass + ".txt";
	ifstream in(filepath);
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);

	// Print attendance list.
	cout << "Attendance list in course " << courseId
		<< " of " << semester << " semester, academic year "
		<< academicYear << "-" << academicYear + 1 << ":\n";
	Student* currentStudent = course->students;
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	int studentNum = 1;
	while (currentStudent != nullptr) {
		cout << "\t";
		cout << studentNum << ". ";
		cout << currentStudent->name << ":\n";
		Attendance* currentAttendance = currentStudentInfo->attendance;
		while (currentAttendance != nullptr) {
			cout << "\t\tDATE: " << currentAttendance->date.day
				<< "-" << currentAttendance->date.month
				<< "-" << currentAttendance->date.year << "\n";
			cout << "\t\tSession time:\n"
				<< "\t\t\tStart time: " << currentAttendance->startTime.hour << ":" << currentAttendance->startTime.minute << "\n"
				<< "\t\t\tEnd time: " << currentAttendance->endTime.hour << ":" << currentAttendance->endTime.minute << "\n";
			cout << "\t\tCheck-in time: " << currentAttendance->time.hour << ":" << currentAttendance->time.minute << "\n";
			cout << "\n";
			currentAttendance = currentAttendance->next;
		}
		cout << "\n";
		studentNum++;
		currentStudent = currentStudent->next;
	}
	cout << "n";

	// Delete linked list.
	deleteCourseInfo(courseInfo);
	deleteCourse(course);

	in.close();
}

// 3.11
void manipulateAllLecturers() {
	string action, confirm, row;
	cout << "\nInput action:\n";
	cout << "\tC  | Create a new lecturer account\n";
	cout << "\tU  | Update information of an existing lecturer\n";
	cout << "\tD  | Delete an existing lecturer\n";
	cout << "\tV  | View a lecturer's info\n";
	cout << "\tVA | View all lecturers\n";
	cout << "Your choice: ";
	cin >> action; toUpper(action);

	while (action != "C" && action != "U" && action != "D" && action != "V" && action != "VA") {
		cout << "Action is not available. Please input again: ";
		cin >> action; toUpper(action);
	}
	cout << "\n";
	string lecturerName, title, gender, lecturerUsername, password;

	// Create lecturer.
	if (action == "C") {
		// Ask for lecturer's information.
		cout << "Please input the following information of lecturer in the same format:\n";
		cout << "<full-name>,<title>,<male-female>\n\t";
		cin.ignore();
		getline(cin, row);
		cout << "\n";
		stringstream info(row);
		getline(info, lecturerName, ','); lecturerName = toFormalCase(lecturerName);
		getline(info, title, ',');
		getline(info, gender, ','); toLower(gender);
		lecturerUsername = toUsername(lecturerName);

		// Exit if lecturer already exists.
		if (isLecturerExist(lecturerUsername)) {
			cout << "Creating new lecturer failed. Error: Lecturer already exists.\n\n";
			return;
		}
		lecturerUsername = getValidUsername(lecturerUsername);

		Lecturer* lecturer = new Lecturer;
		lecturer->username = lecturerUsername;
		lecturer->name = lecturerName;
		lecturer->title = title;
		lecturer->password = toPasswordGeneral(lecturerName);
		lecturer->gender = (gender == "male") ? MALE : FEMALE;
		lecturer->totalCourse = 0;
		lecturer->myCourse = nullptr;
		lecturer->next = nullptr;

		// Confirmation.
		printLecturerInfo(lecturer);
		cout << "Do you want to add this lecturer? Y/N\t";
		cin >> confirm; toUpper(confirm);
		cout << "\n";
		if (confirm == "N") {
			cout << "Adding lecturer cancelled.\n\n";
			deleteLecturers(lecturer);
			return;
		}

		// Add lecturer.
		addLecturer(lecturer);
		addUser(lecturer->username, lecturer->password, LECTURER);
		deleteLecturers(lecturer);
		cout << "Adding lecturer successful.\n\n";
	}
	// Updating a lecturer.
	else if (action == "U") {
		cout << "Input the lecturer's username:\n\t";
		cin >> lecturerUsername;
		cout << "\n";
		if (!isLecturerExist(lecturerUsername)) {
			cout << "Updating lecturer failed. Error: Lecturer does not exist.\n\n";
			return;
		}
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		Lecturer* currentLecturer = lecturers;
		while (currentLecturer->username != lecturerUsername)
			currentLecturer = currentLecturer->next;
		currentLecturer->password = findPasswordFromUsername(lecturerUsername);
		printLecturerInfo(currentLecturer);

		// Ask for field to edit and prompt editing.
		cout << "What field do you want to edit?\n";
		cout << "\t1-Name\n\t2-Gender\n\t3-Title\n";
		cout << "Input in increasing order with a space between.\n\t";
		cin.ignore();
		getline(cin, row);
		cout << "\n";
		stringstream info(row);
		int choice = 0;
		while (info >> choice) {
			if (choice == 1) {
				cout << "New name: ";
				getline(cin, lecturerName);
				lecturerName = toFormalCase(lecturerName);
				cout << "Do you want to change name from "
					<< currentLecturer->name << " to " << lecturerName << "? Y/N\n\t";
				cin >> row; toUpper(row);
				cout << "\n";
				if (row == "Y")
					currentLecturer->name = lecturerName;
			}
			else if (choice == 2) {
				cout << "Do you want to change gender of this lecturer? Y/N\n\t";
				cin >> row; toUpper(row);
				cout << "\n";
				if (row == "Y") {
					if (currentLecturer->gender == FEMALE)
						currentLecturer->gender = MALE;
					else
						currentLecturer->gender = FEMALE;
				}
			}
			else {
				string title;
				cout << "New title: ";
				getline(cin, title);
				cout << "Do you want to change title from "
					<< currentLecturer->title << " to " << title << "? Y/N\n\t";
				cin >> row; toUpper(row);
				cout << "\n";
				if (row == "Y")
					currentLecturer->title = title;
			}
		}
		writeLecturersToFile(lecturers);
		cout << "Updating " << lecturerUsername << "'s information successful.\n\n";
		deleteLecturers(lecturers);
	}
	// Delete a lecturer.
	else if (action == "D") {
		cout << "Input the lecturer's username:\n\t";
		cin >> lecturerUsername;
		cout << "\n";
		if (!isLecturerExist(lecturerUsername)) {
			cout << "Deleting lecturer failed. Error: Lecturer does not exist.\n\n";
			return;
		}
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		Lecturer* currentLecturer = lecturers;
		while (currentLecturer->username != lecturerUsername)
			currentLecturer = currentLecturer->next;
		printLecturerInfo(currentLecturer);
		cout << "Are you sure to delete this lecturer?\n\t";
		cout << "All information about their coursesand student's scores, attendance will be lost and can't be undone.\n";
		cout << "Y/N?\t";
		cin >> confirm; toUpper(confirm);
		cout << "\n";
		if (confirm == "N") {
			cout << "Deleting lecturer cancelled.\n\n";
			deleteLecturers(lecturers);
			return;
		}
		// TODO: REMOVE LECTURER.
		deleteLecturers(lecturers);
		cout << "Deleting lecturer successful.\n\n";
	}
	// View a lecturer.
	else if (action == "V") {
		cout << "Input the lecturer's username:\n\t";
		cin >> lecturerUsername;
		cout << "\n";
		if (!isLecturerExist(lecturerUsername)) {
			cout << "Viewing lecturer failed. Error: Lecturer does not exist.\n\n";
			return;
		}
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		Lecturer* currentLecturer = lecturers;
		while (currentLecturer->username != lecturerUsername)
			currentLecturer = currentLecturer->next;
		printLecturerInfo(currentLecturer);
		cout << "\n";
	}
	// View all lecturers.
	else {
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		cout << "ALL LECTURES' INFORMATION:\n";
		cout << "\t" << setw(15) << "Username |" << setw(30) 
			<< "Full name |" << setw(15) << "Title |" << setw(15) 
			<< "Gender |" <<  "Total courses\n";
		cout << "\t" << setfill('-') << setw(15) << "+" << setw(30) << "+" << setw(15) << "+" << setw(15) << "+" <<  setw(15) << " " << "\n";
		Lecturer* currentLecturer = lecturers;
		while (currentLecturer != nullptr) {
			string gender = (currentLecturer->gender == MALE) ? "male" : "female";
			cout << "\t" << setfill(' ') << setw(14) << currentLecturer->username << "|" << setw(29)
				<< currentLecturer->name << "|" << setw(14) << currentLecturer->title << "|" << setw(14)
				<< gender << "| " << to_string(currentLecturer->totalCourse) << "\n";
			currentLecturer = currentLecturer->next;
		}
		deleteLecturers(lecturers);
		cout << "\n";
	}
}