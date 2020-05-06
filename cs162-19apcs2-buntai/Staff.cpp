#include "Function.h"


// ========= STAFF'S FUNCTIONS DEFINITION =========


// ====== STAFF - CLASS ======

// 2.1
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
	
	// Write to database.
	writeClassToFile(studentList, className);
	addClass(className);
	addStudentUsers(studentList);
	deleteStudentList(studentList);
	cout << "Import succesful. You can find the database at folder Database/Class.\n\n";
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
	getline(info, name, ',');
	name = toFormalCase(name);
	getline(info, className, ',');
	toUpper(className);
	getline(info, dob, ',');
	Date Dob = getDate(dob);
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
	cout << "Please input student class with the same format:\n";
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
	cout << "Please input student'ID, old class and new class of that student with the same fotmat:\n";
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
		<< changeStudent->dob.year << " " << "\n";
	out << changeStudent->numberOfCourse << "\n";
	CourseInfo* changeCourseInfo = changeStudent->myCourse;
	while (changeCourseInfo != nullptr) {
		out << changeCourseInfo->academicYear << " "
			<< changeCourseInfo->academicYear + 1 << " "
			<< changeCourseInfo->semester << " "
			<< changeCourseInfo->courseName << "\n";
		changeCourseInfo = changeCourseInfo->next;
	}
	out << "\n";
	out.close();

	// Delete linked lists.
	deleteStudentList(studentList);

	cout << "Change class of student successfully" << endl;
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

// 3.2
void importCourseFromCsv() {
	string filepath; // store the path to CSV file
	int academicYear;
	string semester;
	cout << "Academic year: ";
	cin >> academicYear; // store academic year
	cout << "Semester: ";
	cin >> semester; // store semester
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
		}

		// Check if class exists.
		if (!isClassExist(defautClass)) {
			cout << "Import failed. Error: Can't find default class.\n\n";
			return;
		} 

		currentCourse = new Course;
		currentCourse->academicYear = academicYear;
		currentCourse->semester = semester;
		currentCourse->courseId = courseId;
		currentCourse->courseName = courseName;
		currentCourse->defaultClass = defautClass;

		string lecturerUsername = toUsername(lecturerName);
		if (!findLecturerFromUsername(lecturerUsername, currentCourse->lecturer)) {
			string title, gender, password;
			cout << "The lecturer is not exist yet. Please input his/her additional information with the same format:\n";
			cout << "<title>,<male/female>\n\t";
			string temp;
			cin.ignore();
			getline(cin, temp);
			stringstream TEMP(temp);
			getline(TEMP, title, ',');
			getline(TEMP, gender, ',');
			password = toPasswordGeneral(lecturerName);
			currentCourse->lecturer.username = lecturerUsername;
			currentCourse->lecturer.name = lecturerName;
			currentCourse->lecturer.title = title;
			currentCourse->lecturer.gender = (gender == "male") ? 1 : 0;
			addUser(lecturerUsername, password, 1);
			addLecturer(currentCourse->lecturer);
		} // Check if lecturer exists, if not create a new account.

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
			while (currentStudentCourse != nullptr && currentStudentCourse->next != nullptr)
				currentStudentCourse = currentStudentCourse->next;
			if (currentStudentCourse == nullptr) {
				currentStudent->myCourse = new CourseInfo;
				currentStudentCourse = currentStudent->myCourse;
			}
			else {
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
		deleteCourse(currentCourse);
		deleteCourseInfo(courseInfo);
	}
	in.close();

	cout << "Import successful. You can find the database at folder Database/" 
		 << academicYear << "-" << academicYear + 1 << "/" << semester << ".\n\n";
}

// 3.3
void manuallyAddCourse() {
	// Ask for information in one line.
	cout << "Please input the following information with the same format:\n";
	cout << "<academic-year>,<semester>,<course-id>,<course-name>,<default-class>,<lecturer-name>,"
		 << "<start-date yyyy-mm-dd>,<end-date yyyy-mm-dd>,<sessions-per-week>,"
		 << "<day-of-week-1>,<start-hour-1>,<end-hour-1>,(...)<room>\n\t";
	string row, academicYear, semester, courseId, courseName, defautClass, lecturerName,
		   startDate, endDate, sessionsPerWeek, dayOfWeek, startHour, endHour, room;
	getline(cin, row);
	cout << "\n";

	// Retrieve each information.
	stringstream info(row);
	getline(info, academicYear, ',');
	getline(info, semester, ','); semester = toFormalCase(semester);
	getline(info, courseId, ','); toUpper(courseId);
	getline(info, courseName, ',');
	getline(info, defautClass, ','); toUpper(defautClass);
	getline(info, lecturerName, ',');
	getline(info, startDate, ',');
	getline(info, endDate, ',');
	getline(info, sessionsPerWeek, ',');

	// Check course information.
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->next = nullptr;
	courseInfo->academicYear = stoi(academicYear);
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defautClass;
	if (isCourseExist(courseInfo)) {
		cout << "Error: Course alrealdy exists.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}
	if (!isClassExist(defautClass)) {
		cout << "Error: Default class does not exist.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}
	if (!isLecturerExist(toUsername(lecturerName))) {
		
		cout << "Error: Lecturer account does not exist.\n\n";
		deleteCourseInfo(courseInfo);
		return;
	}

	// Continue retrieving.
	Course* courseTmp = new Course;
	courseInfo->next = nullptr;
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
	getline(info, room, ',');

	// Ask again.
	cout << "Course info:\n";
	cout << "\tAcademic year: " << academicYear << "\n";
	cout << "\tSemester: " << semester << "\n";
	cout << "\tCourse ID: " << courseId << "\n";
	cout << "\tCourse name: " << courseName << "\n";
	cout << "\tDefault class: " << defautClass << "\n";
	cout << "\tLecturer Account: " << lecturerName << "\n";
	Date start = getDate(startDate), end = getDate(endDate);
	cout << "\tStart date: " << start.day << "-" << start.month << "-" << start.year << "\n";
	cout << "\tEnd date: " << end.day << "-" << end.month << "-" << end.year << "\n";
	cout << "\tSessions per week: " << sessionsPerWeek << "\n";
	cout << "\tSessions info:\n";
	currentSession = courseTmp->sessionInfo;
	for (int i = 0; i < courseTmp->sessionsPerWeek; ++i) {
		cout << "\t\tSection " << i + 1 << ": \n";
		cout << "\t\tDay: " << numToDay(currentSession->day) << "\n";
		cout << "\t\tStart hour: " << currentSession->startTime.hour << ":"
								   << currentSession->startTime.minute << "\n";
		cout << "\t\tEnd hour: " << currentSession->endTime.hour << ":"
								 << currentSession->endTime.minute << "\n";
	}
	cout << "\tRoom: " << room << "\n";
	cout << "Do you want to add this course? Y/N\n";
	cin >> row;
	cout << "\n";
	toUpper(row);
	if (row == "N") {
		deleteCourseInfo(courseInfo);
		deleteSessionInfo(courseTmp->sessionInfo);
		deleteStudentList(courseTmp->students);
		return;
	}

	// Save to database.
	string filepath = "Database/"
		+ academicYear + "-"
		+ to_string(stoi(academicYear) + 1) + "/"
		+ semester + "/"
		+ courseId + "-"
		+ defautClass + ".txt";
	ofstream out(filepath);
	out << courseId << "\n"
		<< courseName << "\n"
		<< defautClass << "\n"
		<< lecturerName << "\n"
		<< start.day << " " << start.month << " " << start.year << "\n"
		<< end.day << " " << end.month << " " << end.year << "\n";
	courseTmp->startDate = start;
	courseTmp->endDate = end;
	courseTmp->sessionsPerWeek = courseTmp->sessionsPerWeek;
	courseTmp->totalSessions = calculateTotalSessions(courseTmp);
	out << courseTmp->totalSessions << "\n"
		<< courseTmp->sessionsPerWeek << "\n";
	currentSession = courseTmp->sessionInfo;
	for (int i = 0; i < courseTmp->sessionsPerWeek; ++i) {
		out << currentSession->day << " "
			<< currentSession->startTime.hour << " " << currentSession->startTime.minute << " "
			<< currentSession->endTime.hour << " " << currentSession->endTime.minute << "\n";
	}
	out << room << "\n\n";
	courseTmp->students = nullptr;
	readClassFromFile(defautClass, courseTmp->students);
	Student* currentStudent = courseTmp->students;
	currentSession = courseTmp->sessionInfo;
	while (currentSession != nullptr && currentSession->next != nullptr)
		currentSession = currentSession->next;
	currentSession->next = courseTmp->sessionInfo;
	currentSession = currentSession->next;
	while (currentStudent != nullptr) {
		out << currentStudent->username << "\n"
			<< currentStudent->name << "\n"
			<< currentStudent->studentId << "\n"
			<< currentStudent->gender << "\n"
			<< currentStudent->dob.day << " " << currentStudent->dob.month << " " << currentStudent->dob.year << "\n"
			<< currentStudent->status << "\n"
			<< "0 0 0 0\n";
		Date nextSession = start;
		int daysToNext;
		currentSession = courseTmp->sessionInfo;
		for (int i = 0; i < courseTmp->totalSessions; ++i) {
			out << nextSession.day << " " << nextSession.month << " " << nextSession.year << " "
				<< currentSession->startTime.hour << " " << currentSession->startTime.minute << " "
				<< currentSession->endTime.hour << " " << currentSession->endTime.minute << " "
				<< "0 0\n";
			daysToNext = currentSession->next->day - currentSession->day;
			daysToNext += (daysToNext > 0) ? 0 : 7;
			nextSession = dateAfterDays(nextSession, daysToNext);
			currentSession = currentSession->next;
		}
		out << "\n";
		currentStudent = currentStudent->next;
	}
	out.close();

	// Edit other related files.
	// Update "Courses.txt" file.
	filepath = "Database/"
		+ academicYear + "-"
		+ to_string(stoi(academicYear) + 1) + "/"
		+ semester + "/Courses.txt";
	out.open(filepath, ios::app);
	out << courseId << " " << defautClass << "\n";
	out.close();
	// Update students information in default class.
	currentStudent = courseTmp->students;
	while (currentStudent != nullptr) {
		currentStudent->numberOfCourse++;
		CourseInfo* currentCourse = currentStudent->myCourse;
		while (currentCourse != nullptr && currentCourse->next != nullptr)
			currentCourse = currentCourse->next;
		if (currentCourse == nullptr) {
			currentStudent->myCourse = new CourseInfo;
			currentCourse = currentStudent->myCourse;
		}
		else {
			currentCourse->next = new CourseInfo;
			currentCourse = currentCourse->next;
		}
		currentCourse->academicYear = courseInfo->academicYear;
		currentCourse->semester = courseInfo->semester;
		currentCourse->courseName = courseInfo->courseName;
		currentCourse->defaultClass = courseInfo->defaultClass;
		currentCourse->next = nullptr;
		currentStudent = currentStudent->next;
	}
	writeClassToFile(courseTmp->students, defautClass);

	// Delete linked list.
	currentSession = courseTmp->sessionInfo->next;
	while (currentSession->next != courseTmp->sessionInfo)
		currentSession = currentSession->next;
	currentSession->next = nullptr;
	deleteCourseInfo(courseInfo);
	deleteSessionInfo(courseTmp->sessionInfo);
	deleteStudentList(courseTmp->students);

	cout << "Course added successfully.\n\n";
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

	// Find remove course
	if (courseList == nullptr) {
		cout << "Cannot find the course\n";
		return;
	}
	CourseInfo* currentCourseList = courseList;
	CourseInfo* previous = currentCourseList;
	while (currentCourseList->courseName != courseID) {
		previous = currentCourseList;
		currentCourseList = currentCourseList->next;
		if (currentCourseList == nullptr) {
			cout << "Cannot find the given course\n";
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
			if (currentStudent == nullptr)break;
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
