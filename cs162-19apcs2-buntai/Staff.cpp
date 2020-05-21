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
				readCourseListFromFile(courseList, currentYear->academicYear, sem);
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
				readCourseListFromFile(courseList, currentYear->academicYear, sem);
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
			cin.ignore();
			getline(cin, row);
			while (!isDateStringSuitable(row)) {
				cout << "Date string is not in yyyy-mm-dd format. Please input again: ";
				getline(cin, row);
			}
			Date Dob = getDate(row);
			cout << "Do you want to change date of birth from " 
				<< dateToString(editedStudent->dob) << " to " 
				<< dateToString(Dob) << "? Y/N\n\t";
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

	removeStudent->password = findPasswordFromUsername(removeStudent->username);
	// Confirm student info.
	printStudentInfo(removeStudent);
	cout << "Are you sure to remove this student? This will remove him from all enrolled courses and can't be undone.\n";
	cout << "Y/N?\t";
	string confirm;
	cin >> confirm; toUpper(confirm);
	cout << "\n";
	if (confirm == "N") {
		deleteStudentList(studentList);
		cout << "Removing student cancelled.\n\n";
		return;
	}

	// Change status of student
	cout << "\tRemoving student...\n";
	removeStudent->status = 0;

	// Save students to class file.
	cout << "\tWriting class to file...\n";
	writeClassToFile(studentList, className);

	// Change status of removed student in enrolled courses to be 0.
	cout << "\tRemoving students from enrolled courses...\n";
	CourseInfo* myCourse = removeStudent->myCourse,* currentCourse = myCourse;
	while (currentCourse != nullptr) {
		unregisterCourseForStudent(removeStudent, currentCourse);
		currentCourse = currentCourse->next;
	}

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "\nRemove student successfully.\n\n";
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
	getline(info, oldClass, ','); toUpper(oldClass);
	getline(info, newClass); toUpper(newClass);

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

	changeStudent->password = findPasswordFromUsername(changeStudent->username);
	// Confirm student info.
	printStudentInfo(changeStudent);
	cout << "Are you sure to move this student from class " 
		<< oldClass << " to " << newClass << "?\n";
	cout << "It will remove him from enrolled courses and enroll in default courses of new class.\n";
	cout << "Y/N?\t";
	string confirm;
	cin >> confirm; toUpper(confirm);
	cout << "\n";
	if (confirm == "N") {
		deleteStudentList(studentList);
		cout << "Removing student cancelled.\n\n";
		return;
	}

	// Change status of student
	cout << "\tRemoving student from old class...\n";
	changeStudent->status = 0;

	// Save students to class file.
	cout << "\tWriting old class to file...\n";
	writeClassToFile(studentList, oldClass);

	// Change status of removed student in enrolled courses to be 0.
	cout << "\tRemoving students from enrolled courses...\n";
	CourseInfo* myCourse = changeStudent->myCourse, * currentCourse = myCourse;
	while (currentCourse != nullptr) {
		unregisterCourseForStudent(changeStudent, currentCourse);
		currentCourse = currentCourse->next;
	}

	Student* temp = changeStudent;
	changeStudent = new Student;
	changeStudent->username = temp->username;
	changeStudent->name = temp->name;
	changeStudent->studentId = temp->studentId;
	changeStudent->dob = temp->dob;
	changeStudent->gender = temp->gender;
	changeStudent->status = 1;
	changeStudent->numberOfCourse = 0;
	changeStudent->myCourse = nullptr;
	changeStudent->next = nullptr;

	// Delete old class linked list.
	deleteStudentList(studentList);

	// Register default courses of new class for change student.
	cout << "\tRegistering default courses of new class...\n";
	AcademicYear* academicYears = nullptr;
	readAcademicYearsFromFile(academicYears);
	AcademicYear* currentYear = academicYears;
	while (currentYear != nullptr) {
		string sem;
		stringstream semesterNames(currentYear->semester);
		while (getline(semesterNames, sem, ',')) {
			CourseInfo* courseList = nullptr;
			readCourseListFromFile(courseList, currentYear->academicYear, sem);
			CourseInfo* currentCourse = courseList;
			while (currentCourse != nullptr) {
				if (currentCourse->defaultClass == newClass)
					registerCourseForStudentList(changeStudent, currentCourse);
				currentCourse = currentCourse->next;
			}
			currentYear = currentYear->next;
			deleteCourseInfo(courseList);
		}
	}
	deleteAcademicYears(academicYears);

	// Read new class linked list.
	studentList = nullptr;
	cout << "\tReading new class from file...\n";
	readClassFromFile(newClass, studentList);
	Student* currentStudent = studentList;
	while (currentStudent->next != nullptr)
		currentStudent = currentStudent->next;
	currentStudent->next = changeStudent;
	
	// Write info of student to new class.
	cout << "\tWriting class to new file...\n";
	writeClassToFile(studentList, newClass);

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "\nChange class of student successfully." << endl;
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
	getline(cin, className); toUpper(className);
	cout << "\n";

	// Check if class exists.
	if (!isClassExist(className)) {
		cout << "Edit failed. Error: Can't find class.\n\n";
		return;
	}

	// Read all students' information in class.
	Student* studentList = nullptr;
	readClassFromFile(className, studentList);

	// Print all students' name in class.
	cout << "The list of students in class " << className << ":\n";
	printStudentListTable(studentList);
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
			readCourseListFromFile(courseList, academicYear, semester);
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
	cout << "Please input the following information:\n";
	string filepath; // store the path to CSV file
	int academicYear;
	string semester;
	cout << "\tAcademic year: ";
	cin >> academicYear; // store academic year
	cout << "\tSemester: ";
	cin >> semester; // store semester
	semester = toFormalCase(semester);
	cout << "\tEnter the path to CSV file: ";
	cin >> filepath;
	cout << "\n";

	// Try to open file at given filepath.
	ifstream in;
	in.open(filepath);
	while (!in.is_open()) {
		cout << "\tThe file path you entered is not valid. Please input another path or ""0"" to stop: ";
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
	cout << "\tLecturer account: " << lecturerUsername << "\n";
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
void editExistingCourse() {
	// Ask for academic year, semester, course ID and default class.
	cout << "Please input course information with the same format:\n";
	cout << "<academic-year>,<semester>,<course-id>,<default-class>\n\t";
	string row, academicYear, semester, courseId, defaultClass;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, academicYear, ',');
	getline(info, semester, ',');
	semester = toFormalCase(semester);
	getline(info, courseId, ',');
	toUpper(courseId);
	getline(info, defaultClass, ',');
	toUpper(defaultClass);

	// Check if course exists.
	CourseInfo* editedInfo = new CourseInfo;
	editedInfo->academicYear = stoi(academicYear);
	editedInfo->semester = semester;
	editedInfo->courseName = courseId;
	editedInfo->defaultClass = defaultClass;
	editedInfo->next = nullptr;
	if (!isCourseExist(editedInfo)) {
		cout << "Edit failed. Error: Can't find course.\n\n";
		deleteCourseInfo(editedInfo);
		return;
	}

	// Confirm course info.
	Course* editedCourse = new Course;
	readCourseFromFile(editedInfo, editedCourse);
	findLecturerFromUsername(editedCourse->lecturer.username, editedCourse->lecturer);
	printCourseInfo(editedCourse);

	// Ask for field to edit and prompt editing.
	cout << "What field do you want to edit?\n";
	cout << "\t1-Course name\n\t2-Lecturer\n";
	cout << "Input in increasing order with a space between.\n\t";
	getline(cin, row);
	cout << "\n";
	info = stringstream(row);
	int choice = 0;
	bool isNameChanged = false, isLecChanged = false;
	while (info >> choice) {
		if (choice == 1) {
			string name;
			cout << "New name: ";
			getline(cin, name);
			name = toFormalCase(name);
			cout << "Do you want to change name from "
				<< editedCourse->courseName << " to " << name << "? Y/N\n\t";
			cin >> row;
			cout << "\n";
			toUpper(row);
			if (row == "Y") {
				editedCourse->courseName = name;
				isNameChanged = true;
			}
		}
		else {
			string username;
			cout << "New lecturer username: ";
			cin.ignore();
			getline(cin, username);
			toLower(username);
			Lecturer newLecturer;
			if (!findLecturerFromUsername(username, newLecturer)) {
				cout << "Edit failed. Error: Can't find new lecturer.\n\n";

				// Edit "<course-id>-<default-class>.txt" file.
				writeCourseToFile(editedCourse);
				deleteCourse(editedCourse);

				// Delete linked list.
				deleteCourseInfo(editedInfo);

				if (!isNameChanged)
					cout << "Edit course name successfully.\n\n";
				return;
			}
			else {
				cout << "Do you want to change lecturer from "
					<< editedCourse->lecturer.title << " " << editedCourse->lecturer.name << " to "
					<< newLecturer.title << " " << newLecturer.name << "? Y/N\n\t";
				cin >> row;
				cout << "\n";
				toUpper(row);
				if (row == "Y") {
					isLecChanged = true;
					// Edit "Lecturer.txt" file.
					Lecturer* lecturers = nullptr;
					readLecturersFromFile(lecturers);
					Lecturer* currentLecturer = lecturers;
					// Loop twice to modify info of new/old lecturer.
					for (int i = 0; i < 2; ++i) {
						while (currentLecturer != nullptr) {
							if (currentLecturer->username == editedCourse->lecturer.username) {
								currentLecturer->totalCourse--;
								CourseInfo* currentInfo = currentLecturer->myCourse, * previousInfo = nullptr;
								while (currentInfo != nullptr) {
									if (currentInfo->courseName == editedInfo->courseName && 
										currentInfo->defaultClass == editedInfo->defaultClass) {
										CourseInfo* temp = currentInfo;
										if (previousInfo == nullptr)
											currentLecturer->myCourse = currentLecturer->myCourse->next;
										else
											previousInfo->next = currentInfo->next;
										currentInfo = currentInfo->next;
										delete temp;
									}
									else {
										previousInfo = currentInfo;
										currentInfo = currentInfo->next;
									}
								}
								currentLecturer = currentLecturer->next;
								break;
							}
							if (currentLecturer->username == newLecturer.username) {
								currentLecturer->totalCourse++;
								CourseInfo* temp = editedInfo;
								editedInfo->next = currentLecturer->myCourse;
								currentLecturer->myCourse = temp;
								currentLecturer = currentLecturer->next;
								break;
							}
							currentLecturer = currentLecturer->next;
						}
					}
					writeLecturersToFile(lecturers);
					deleteLecturers(lecturers);
					editedCourse->lecturer.username = newLecturer.username;
					editedCourse->lecturer.title = newLecturer.title;
				}
			}
		}
	}

	// Edit "<course-id>-<default-class>.txt" file.
	writeCourseToFile(editedCourse);
	deleteCourse(editedCourse);

	// Delete linked lists.
	if (!isLecChanged)
		deleteCourseInfo(editedInfo);

	if (!isNameChanged && !isLecChanged)
		cout << "No change was made.\n\n";
	else
		cout << "Edit course successfully.\n\n";
}

// 3.5
void removeCourse() {
	// Ask for input.
	cout << "Please input the academic year:\n";
	cout << "\t(input 2018 for AY 2018-2019)\n\t";
	int academicYear;
	cin >> academicYear;
	string semester, courseID, defaultClass;
	cout << "Please input semester: \n";
	cout << "\t(Summer, Spring or Fall)\n\t";
	cin >> semester;
	semester = toFormalCase(semester);
	cout << "Please input course ID: \n\t";
	cin >> courseID;
	toUpper(courseID);
	cout << "Please input default class: \n\t";
	cin >> defaultClass;
	toUpper(defaultClass);

	// Read courses.txt to find remove course.
	CourseInfo* courseList = nullptr;
	readCourseListFromFile(courseList, academicYear, semester);

	// Find remove course.
	if (courseList == nullptr) {
		cout << "Error: Cannot find the course.\n\n";
		return;
	}
	CourseInfo* currentCourseList = courseList;
	CourseInfo* previous = currentCourseList;
	while (currentCourseList->courseName != courseID && currentCourseList->defaultClass!= defaultClass) {
		previous = currentCourseList;
		currentCourseList = currentCourseList->next;
		if (currentCourseList == nullptr) {
			cout << "Error: Cannot find the course.\n\n";
			deleteCourseInfo(courseList);
			return;
		}
	}

	// Remove  course name in file Courses.txt.
	if (currentCourseList == courseList) courseList = courseList->next;
	else {
		previous->next = currentCourseList->next;
		deleteCourseInfo(currentCourseList);
	}
	writeCourseListToFile(courseList, academicYear, semester);
	deleteCourseInfo(courseList);

	// Remove course file <courseID-defaultClass>.txt.
	string filePath= "Database/"
		+ to_string(academicYear) + "-" + to_string(academicYear+1) + "/"
		+ semester + "/"
		+ courseID + "-" + defaultClass
		+ ".txt";
	const char* filepath = filePath.c_str();
	remove(filepath);

	// Remove course in student course.
	ifstream in;
	in.open("Database/Class/Classes.txt");
	if (in.is_open()) {
		string className;
		while (in >> className) {
			Student* studentList = nullptr;
			readClassFromFile(className,studentList);
			Student* currentStudent = studentList;
			while (currentStudent != nullptr) {
				if (currentStudent->status == 1 && currentStudent->myCourse!=nullptr) {
					if ( currentStudent->myCourse->academicYear == academicYear
						 && currentStudent->myCourse->semester == semester
						 && currentStudent->myCourse->courseName == courseID
						 && currentStudent->myCourse-> defaultClass == defaultClass) {
						CourseInfo* temp = currentStudent->myCourse;
						currentStudent->myCourse = currentStudent->myCourse->next;
						delete temp;
					}
					else {
						CourseInfo* currentCourse = currentStudent->myCourse;
						while (currentCourse->next != nullptr) {
							if ( currentCourse->next->academicYear == academicYear
								&& currentCourse->next->semester == semester
								&& currentCourse->next->courseName == courseID
								&& currentCourse->next->defaultClass==defaultClass) {
								CourseInfo* temp;
								temp = currentCourse->next;
								currentCourse->next = currentCourse->next->next;
								delete temp;
								break;
							}
							currentCourse = currentCourse->next;
							if (currentCourse == nullptr) break;
						}
					}
				}
				currentStudent = currentStudent->next;
			}
			writeClassToFile(studentList, className);
			deleteStudent(studentList);
		}
	}

	// Remove course in lecterer course.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	while (lecturers != nullptr && lecturers->myCourse!=nullptr) {
		if (lecturers->myCourse->academicYear == academicYear
			&& lecturers->myCourse->semester == semester
			&& lecturers->myCourse->courseName == courseID
			&& lecturers->myCourse->defaultClass == defaultClass) {
			CourseInfo* temp = lecturers->myCourse;
			lecturers->myCourse = lecturers->myCourse->next;
			delete temp;
		}
		else {
			CourseInfo* currentCourse = lecturers->myCourse;
			while (currentCourse->next != nullptr) {
				if (currentCourse->next->academicYear == academicYear
					&& currentCourse->next->semester == semester
					&& currentCourse->next->courseName == courseID
					&& currentCourse->next->defaultClass == defaultClass) {
					CourseInfo* temp;
					temp = currentCourse->next;
					currentCourse->next = currentCourse->next->next;
					delete temp;
					break;
				}
				currentCourse = currentCourse->next;
				if (currentCourse == nullptr) break;
			}
		}
	}
	writeLecturersToFile(lecturers);
	deleteLecturers(lecturers);

	// Annoucement
	cout << "Remove course successfully!\n";
}

// 3.6
void removeStudentFromCourse() {
	// Get input.
	string studentID, studentClass, semester, courseID, courseClass;
	cout << "Please enter student ID: \n\t";
	cin >> studentID;
	cout << "Please enter student class: \n\t";
	cin >> studentClass;
	toUpper(studentClass);
	cout << "Please enter academic year, semester, courseID and default class of that course with the same format: \n";
	cout << "\t<academicYear> <Semester> <courseID> <defaulClass> \n";
	cout << "\t(Note that academic year 2018-2019 enter 2018 only)\n\t";
	int academicYear;
	cin >> academicYear;
	cin >> semester;
	semester = toFormalCase(semester);
	cin >> courseID;
	toUpper(courseID);
	cin >> courseClass;
	toUpper(courseClass);

	// Remove student from file <course>-<default>.txt.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = academicYear;
	courseInfo->semester = semester;
	courseInfo->courseName = courseID;
	courseInfo->defaultClass = courseClass;
	courseInfo->next = nullptr;
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Student* currentStudent = course->students;
	StudentCourseInfo* currentStudentCourseInfo = course->studentCourseInfo;
	while (currentStudent != nullptr) {
		if (currentStudent->studentId == studentID) {
			currentStudentCourseInfo->status = 0;
			break;
		}
		currentStudent = currentStudent->next;
		currentStudentCourseInfo = currentStudentCourseInfo->next;
	}
	if (currentStudent == nullptr) {
		cout << "Error: Can not find student in given course.\n";
		return;
	}
	writeCourseToFile(course);
	
	// Read class file to find student and remove in his/her courses.
	Student* studentList = nullptr;
	readClassFromFile(studentClass, studentList);
	Student* current = studentList;
	while (current != nullptr) {
		if (current->studentId == studentID) {
			if (current->myCourse->academicYear== academicYear
				&& current->myCourse->semester == semester
				&& current->myCourse->courseName==courseID
				&& current->myCourse->defaultClass==courseClass) {
				CourseInfo* temp = current->myCourse;
				current->myCourse = current->myCourse->next;
				delete temp;
				break;
			}
			else {
				CourseInfo* currentCourseInfo = current->myCourse;
				while (currentCourseInfo->next!= nullptr) {
					if (currentCourseInfo->next->academicYear==academicYear
						&& currentCourseInfo->next->semester==semester
						&& currentCourseInfo->next->courseName==courseID
						&& currentCourseInfo->next->defaultClass==courseClass) {
						CourseInfo* temp = currentCourseInfo->next;
						currentCourseInfo->next = currentCourseInfo->next->next;
						delete temp;
						break;
					}
					currentCourseInfo=currentCourseInfo->next;
					if (currentCourseInfo == nullptr) {
						cout << "Error: Can not find given course in student's courses.\n";
						return;
					}
				}
			}
		}
		current = current->next;
	}
	writeClassToFile(studentList, studentClass);

	// Delete pointer.
	deleteCourse(course);
	deleteCourseInfo(courseInfo);
	deleteStudentList(studentList);

	// Annoucement.
	cout << "Remove student from course successfully!\n";
}

// 3.7
void addAStudentToCourse() {
	// Get input.
	string studentID, studentClass, semester, courseID, courseClass;
	cout << "Please enter student ID: \n\t";
	cin >> studentID;
	cout << "Please enter student class: \n\t";
	cin >> studentClass;
	toUpper(studentClass);
	cout << "Please enter academic year, semester, courseID and default class of that course with the same format: \n";
	cout << "\t<academicYear> <Semester> <courseID> <defaulClass> \n";
	cout << "\t(Note that academic year 2018-2019 enter 2018 only)\n\t";
	int academicYear;
	cin >> academicYear;
	cin >> semester;
	semester = toFormalCase(semester);
	cin >> courseID;
	toUpper(courseID);
	cin >> courseClass;
	toUpper(courseClass);

	// Read class file to read the information of added student and add course in student's courses.
	Student* studentList = nullptr;
	readClassFromFile(studentClass, studentList);
	Student* currentStudent = studentList;
	while (currentStudent!= nullptr) {
		if (currentStudent->studentId == studentID) {
			// add course
			if (currentStudent->myCourse == nullptr) {
				currentStudent->myCourse = new CourseInfo;
				currentStudent->myCourse->academicYear = academicYear;
				currentStudent->myCourse->semester = semester;
				currentStudent->myCourse->courseName = courseID;
				currentStudent->myCourse->defaultClass = courseClass;
				currentStudent->myCourse->next = nullptr;
				break;
			}
			else {
				CourseInfo* currentCourse = currentStudent->myCourse;
				while (currentCourse->next != nullptr) {
					currentCourse = currentCourse->next;
				}
				currentCourse->next = new CourseInfo;
				currentCourse->next->academicYear = academicYear;
				currentCourse->next->semester = semester;
				currentCourse->next->courseName = courseID;
				currentCourse->next->defaultClass = courseClass;
				currentCourse->next->next = nullptr;
				break;
			}
		}
		currentStudent = currentStudent->next;
	}
	if (currentStudent == nullptr) {
		cout << "Error: Cannot find student in given class. \n";
		return;
	}
	writeClassToFile(studentList, studentClass);
	Student* addedStudent = currentStudent;

	// Add student to course file.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = academicYear;
	courseInfo->semester = semester;
	courseInfo->courseName = courseID;
	courseInfo->defaultClass = courseClass;
	courseInfo->next = nullptr;
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Student* curStudent = course->students;
	StudentCourseInfo* curStudentCourseInfo = course->studentCourseInfo;
	while (curStudent->next != nullptr) {
		curStudent=curStudent->next;
		curStudentCourseInfo = curStudentCourseInfo->next;
	}

	curStudent->next = new Student;
	curStudent->next->username = addedStudent->username;
	curStudent->next->name = addedStudent->name;
	curStudent->next->studentId = addedStudent->studentId;
	curStudent->next->gender = addedStudent->gender;
	curStudent->next->dob = addedStudent->dob;
	curStudent->next->next = nullptr;

	curStudentCourseInfo->next = new StudentCourseInfo;
	curStudentCourseInfo->next->midterm = 0;
	curStudentCourseInfo->next->lab = 0;
	curStudentCourseInfo->next->final = 0;
	curStudentCourseInfo->next->bonus = 0;
	curStudentCourseInfo->next->status = 1;
	curStudentCourseInfo->next->next = nullptr;
	curStudentCourseInfo->next->attendance = nullptr;
	Attendance* currentAttendance = nullptr;
	Date nextSession = course->startDate;
	int daysToNext;
	SessionInfo* currentSession = course->sessionInfo;
	// Link session info list circularly.
	while (currentSession != nullptr && currentSession->next != nullptr)
		currentSession = currentSession->next;
	currentSession->next = course->sessionInfo;
	currentSession = currentSession->next;
	for (int i = 0; i < course->totalSessions; ++i) {
		if (curStudentCourseInfo->next->attendance == nullptr) {
			curStudentCourseInfo->next->attendance = new Attendance;
			currentAttendance = curStudentCourseInfo->next->attendance;
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
	currentSession = course->sessionInfo->next;
	while (currentSession->next != course->sessionInfo)
		currentSession = currentSession->next;
	currentSession->next = nullptr;

	writeCourseToFile(course);

	// Delete pointer.
	deleteStudentList(studentList);
	deleteCourse(course);
	deleteCourseInfo(courseInfo);

	// Annoucement.
	cout << "Add student to course successfully. \n";
}

// 3.8
void viewListOfCourses() {
	cout << "Please input the following information:\n";
	int academicYear;
	string semester;
	cout << "\tAcademic year: ";
	cin >> academicYear;
	cout << "\tSemester: ";
	cin >> semester; semester = toFormalCase(semester);
	cout << "\n";

	if (!isSemesterExist(academicYear, semester)) {
		cout << "View list of courses failed. Error: Semester does not exist.\n\n";
		return;
	}

	CourseInfo* courseList = nullptr;
	readCourseListFromFile(courseList, academicYear, semester);
	cout << "List of courses in " << semester << " semester of academic year "
		 << academicYear << "-" << academicYear + 1 << ":\n";
	CourseInfo* currentCourse = courseList;
	if (currentCourse != nullptr) {
		cout << "\t" << setw(20) << "Course ID |" << " Default class\n";
		cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << " " << "\n";
		while (currentCourse != nullptr) {
			string year = to_string(currentCourse->academicYear) + "-"
				+ to_string(currentCourse->academicYear + 1);
			cout << "\t" << setfill(' ') << setw(19) << currentCourse->courseName << "| "
				<< currentCourse->defaultClass << "\n";
			currentCourse = currentCourse->next;
		}
	}
	cout << "\n";
	deleteCourseInfo(courseList);
}

// 3.9
void viewListOfStudentsOfCourse() {
	// Get course information.
	cout << "Please input the following information:\n";
	int academicYear;
	string semester, courseId, defaultClass;
	cout << "\tAcademic year: ";
	cin >> academicYear;
	cout << "\tSemester: ";
	cin >> semester; semester = toFormalCase(semester);
	cout << "\tCourse ID: ";
	cin >> courseId; toUpper(courseId);
	cout << "\tDefault class: ";
	cin >> defaultClass; toUpper(defaultClass);
	cout << "\n";

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
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);

	// Print students list.
	cout << "List of students enrolled in course " << courseId
		 << " of " << semester << " semester, academic year "
		 << academicYear << "-" << academicYear + 1 << ":\n";
	printStudentListTable(course->students);
	cout << "\n";

	// Delete linked list.
	deleteCourseInfo(courseInfo);
	deleteCourse(course);
}

// 3.10
void viewAttendanceListOfCourse() {
	// Get course information.
	cout << "Please input the following information:\n";
	int academicYear;
	string semester, courseId, defaultClass;
	cout << "\tAcademic year: ";
	cin >> academicYear;
	cout << "\tSemester: "; 
	cin >> semester; semester = toFormalCase(semester);
	cout << "\tCourse ID: ";
	cin >> courseId; toUpper(courseId);
	cout << "\tDefault class: ";
	cin >> defaultClass; toUpper(defaultClass);
	cout << "\n";

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
	toUpper(semester);
	cout << "\t\tATTENDANCE LIST OF COURSE " << courseId << " OF " 
		<< semester << " SEMESTER, "
		<< academicYear << "-" << academicYear + 1 << ":\n";
	semester = toFormalCase(semester);

	// Print session list.
	printAllSessionsTable(course->studentCourseInfo->attendance);

	// Print student list.
	printAttendanceListOfCourse(course);

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


// ====== STAFF - SCOREBOARD ======

// 4.2
void exportScoreboardToCsv() {
	// Get course information.
	cout << "Please input course information with the same format:\n";
	cout << "<academic-year>,<semester>,<course-id>,<default-class>\n\t";
	string row, academicYear, semester, courseId, defaultClass;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, academicYear, ',');
	getline(info, semester, ',');
	semester = toFormalCase(semester);
	getline(info, courseId, ',');
	toUpper(courseId);
	getline(info, defaultClass, ',');
	toUpper(defaultClass);

	// Check whether course exists.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = stoi(academicYear);
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	if (!isCourseExist(courseInfo)) {
		cout << "Export failed. Error: Can't find course.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}

	// Read course from file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);

	// Export scoreboard to file csv
	cout << "Please enter the path to file csv used to store scoreboard: \n\t";
	getline(cin, row);
	cout << "\n";
	ofstream out(row);
	if (!out.is_open()) {
		cout << "Export failed. Error: Can't create file.\n\n";
		deleteCourseInfo(courseInfo);
		deleteCourse(course);
		return;
	}
	else {
		out << "No., StudentID, Full name, Midterm, Final, Lab, Bonus \n";
		int count = 0;
		Student* curStudent = course->students;
		StudentCourseInfo* curStudentCourseInfo = course->studentCourseInfo;
		while (curStudent != nullptr) {
			out << count++ << ","
				<< curStudent->studentId << ","
				<< curStudent->name << ","
				<< curStudentCourseInfo->midterm << ","
				<< curStudentCourseInfo->final << ","
				<< curStudentCourseInfo->lab << ","
				<< curStudentCourseInfo->bonus << "\n";
			curStudent = curStudent->next;
			curStudentCourseInfo = curStudentCourseInfo->next;
		}
		out.close();
	}

	// Delete linked list.
	deleteCourseInfo(courseInfo);
	deleteCourse(course);

	// Annoucement
	cout << "Export successfully. Please check CSV file in " << row << "\n\n";
}

// ====== STAFF - ATTENDANCE ======

// 5.1
void searchAndViewAttendance() {
	// Get student information.
	cout << "Please input student information with the same format:\n";
	cout << "<academic-year>,<semester>,<course-id>,<default-class>,<student-id>\n\t";
	string row, academicYear, semester, courseId, defaultClass, studentId;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, academicYear, ',');
	int AY = stoi(academicYear);
	getline(info, semester, ',');
	semester = toFormalCase(semester);
	getline(info, courseId, ',');
	toUpper(courseId);
	getline(info, defaultClass, ',');
	toUpper(defaultClass);
	getline(info, studentId, ',');

	// Check whether student exists, if yes store student info.
	Student student;
	if (!findStudentInfoFromId(student, studentId)) {
		cout << "Search failed. Error: Student not found.\n\n";
		return;
	}

	// Check whether student studies the course.
	bool isStudy = false;
	CourseInfo* currentCourse = student.myCourse;
	while (currentCourse != nullptr) {
		if (currentCourse->academicYear == AY && currentCourse->semester == semester &&
			currentCourse->courseName == courseId && currentCourse->defaultClass == defaultClass) {
			isStudy = true;
			break;
		}
		currentCourse = currentCourse->next;
	}
	if (!isStudy) {
		cout << "Search failed. Error: Course not found in student course list.\n\n";
		deleteCourseInfo(student.myCourse);
		return;
	}

	// Read attendance list.
	Attendance* attendance = nullptr;
	readAttendanceList(attendance, currentCourse, student);

	// Ask for way to view and prompt editing.
	cout << "Which way do you want to view attendance of student " 
		<< student.name << " - " << student.studentId << "?\n";
	cout << "\t1-All attendance\n\t2-Attendance in specific date\n";
	cout << "Please choose one.\n\t";
	int choice;
	cin >> choice;
	cout << "\n";
	if (choice == 1) {
		cout << "\t" << setw(20) << "Date |" << setw(20) << "Study time |" << " Check-in time\n";
		int totalSession = 0, onTime = 0;
		Attendance* currentAttendance = attendance;
		while (currentAttendance->next != nullptr) {
			cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+" << setw(20) << "\n";
			string date, time, checkin;
			date = numToDay(getDayOfWeek(currentAttendance->date)) + ", " + dateToString(currentAttendance->date) + " |";
			time = timeToString(currentAttendance->startTime) + " - " + timeToString(currentAttendance->endTime) + " |";
			checkin = " " + timeToString(currentAttendance->time) + "\n";
			cout << "\t" << setfill(' ') << setw(20) << date << setw(20) << time << checkin;
			if (isPresent(currentAttendance))
				onTime++;
			totalSession++;
			currentAttendance = currentAttendance->next;
		}
		cout << "\nSummary:\n";
		cout << "\t" << setfill(' ') << setw(10) << "On time |" << " " << onTime << "\n";
		cout << "\t" << setfill('-') << setw(10) << "+" << setw(5) << "\n";
		cout << "\t" << setfill(' ') << setw(10) << "Absent |" << " " << totalSession - onTime << "\n";
	}
	else {
		// Print session list.
		printAllSessionsTable(attendance);

		// Choose date to view.
		cout << "Please input date to view with the same format:\n";
		cout << "<yyyy>-<mm>-<dd>\n\t";
		cin.ignore();
		getline(cin, row);
		Date date = getDate(row);
		Attendance* currentAttendance = attendance;
		int valid = false;
		while (currentAttendance != nullptr) {
			if (currentAttendance->date.day == date.day &&
				currentAttendance->date.month == date.month &&
				currentAttendance->date.year == date.year) {
				valid = true;
				break;
			}
			currentAttendance = currentAttendance->next;
		}
		string DATE, TIME;
		DATE = numToDay(getDayOfWeek(date)) + ", " + dateToString(date) + "|";
		TIME = timeToString(currentAttendance->time) + "|";
		cout << "\t" << setfill(' ') << setw(25) << "DATE |" << setw(17) << "Check-in time |" << " Status\n";
		cout << "\t" << setfill('-') << setw(25) << "+" << setw(17) << "+" << setw(8) << "\n";
		cout << "\t" << setfill(' ') << setw(25) << DATE << setw(17) << TIME << ((isPresent(currentAttendance)) ? " On time" : " Absent\n\n");
	}

	// Delete linked list.
	deleteCourseInfo(student.myCourse);
	deleteAttendance(attendance);
}

// 5.2
void exportAttendanceListToCsv() {
	// Get course information.
	cout << "Please input course information with the same format:\n";
	cout << "<academic-year>,<semester>,<course-id>,<default-class>\n\t";
	string row, academicYear, semester, courseId, defaultClass;
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, academicYear, ',');
	getline(info, semester, ',');
	semester = toFormalCase(semester);
	getline(info, courseId, ',');
	toUpper(courseId);
	getline(info, defaultClass, ',');
	toUpper(defaultClass);

	// Check whether course exists.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = stoi(academicYear);
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	if (!isCourseExist(courseInfo)) {
		cout << "Export failed. Error: Can't find course.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}

	// Read course from file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);

	// Export attendance list to csv file.
	cout << "Please enter filepath to store attendance CSV file:\n\t";
	getline(cin, row);
	cout << "\n";
	ofstream out(row);
	if (!out.is_open()) {
		cout << "Export failed. Error: Can't create file.\n\n";
		deleteCourseInfo(courseInfo);
		deleteCourse(course);
		return;
	}
	else {
		out << "Number,"
			<< "Name,"
			<< "Status,"
			<< "Session date,"
			<< "Start time,"
			<< "End time,"
			<< "Check-in time\n";
		int count = 1;
		Student* currentStudent = course->students;
		StudentCourseInfo* currentInfo = course->studentCourseInfo;
		while (currentStudent != nullptr) {
			out << count << ","
				<< currentStudent->name << ","
				<< currentInfo->status << ","
				<< currentInfo->attendance->date.year << "-"
				<< currentInfo->attendance->date.month << "-"
				<< currentInfo->attendance->date.day << ","
				<< currentInfo->attendance->startTime.hour << " "
				<< currentInfo->attendance->startTime.minute << ","
				<< currentInfo->attendance->endTime.hour << " "
				<< currentInfo->attendance->endTime.minute << ","
				<< currentInfo->attendance->time.hour << " "
				<< currentInfo->attendance->time.minute << "\n";
			count++;
			currentStudent = currentStudent->next;
			currentInfo = currentInfo->next;
		}
		out.close();
	}
	
	// Delete linked list.
	deleteCourseInfo(courseInfo);
	deleteCourse(course);

	cout << "Export successfully. Please check CSV file in " << row << "\n\n";
}
