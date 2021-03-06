#include "Function.h"

// ========= LECTURERS' FUNCTIONS DEFINITION =========

// 6.1
void viewCoursesInSemester(string lecturerUsername) {
	cout << "Please input the current academic year and semester:\n\t";
	cout << "For example: ""2018 Summer""\n\t";
	int academicYear;
	string semester, action;
	cin >> academicYear >> semester; semester = toFormalCase(semester);
	cout << "\n";

	cout << "Input the action:\n";
	cout << "\tM  | View your courses in current semester\n";
	cout << "\tA  | View all courses in current semester\n";
	cout << "Your choice: ";
	cin >> action; toUpper(action);
	while (action != "M" && action != "A") {
		cout << "Your action is not suitable. Please choose again: ";
		cin >> action; toUpper(action);
	}
	cout << "\n";

	if (action == "M") {
		Lecturer* lecturers = nullptr;
		readLecturersFromFile(lecturers);
		Lecturer* currentLecturer = lecturers;
		while (currentLecturer->username != lecturerUsername)
			currentLecturer = currentLecturer->next;
		int semesterCourse = 0;
		if (currentLecturer->totalCourse) {
			CourseInfo* currentCourse = currentLecturer->myCourse;
			while (currentCourse != nullptr) {
				if (currentCourse->academicYear == academicYear && currentCourse->semester == semester) {
					if (!semesterCourse) {
						cout << "\t\t\t\t\t\tYOUR COURSES\n\n";
						cout << "\t" << setw(15) << " Course ID |" << setw(16) << " Default class |"
							<< setw(25) << " Duration |" << setw(25) << " Session |" << setw(8) << " Room |" << "\n";
						cout << "\t" << setfill('-') << setw(15) << "+" << setw(16) << "+"
							<< setw(25) << "+" << setw(25) << "+" << setw(8) << "+" << "\n";
					}
					else
						cout << "\t" << setfill('-') << setw(15) << "+" << setw(16) << "+"
						<< setw(25) << "+" << setw(25) << "+" << setw(8) << "+" << "\n" << setfill(' ');
					Course* course = new Course;
					readCourseFromFile(currentCourse, course);
					string duration = dateToString(course->startDate) + " - " + dateToString(course->endDate);
					SessionInfo* session = course->sessionInfo;
					string sessionInfo = numToDay(session->day) + ", " + timeToString(session->startTime) 
						+ " - " + timeToString(session->endTime);
					cout << "\t" << setfill(' ') << setw(14) << currentCourse->courseName << "|" << setw(15)
						<< currentCourse->defaultClass << "|" << setw(24) << duration << "|" << setw(24) 
						<< sessionInfo << "|" << setw(7) << course->room << "|\n";
					session = session->next;
					while (session != nullptr) {
						sessionInfo = numToDay(session->day) + ", " + timeToString(session->startTime)
							+ " - " + timeToString(session->endTime);
						cout << "\t" << setw(14) << " " << "|" << setw(15)
							<< " " << "|" << setw(24) << " " << "|" << setw(24)
							<< sessionInfo << "|" << setw(7) << " " << "|\n";
						session = session->next;
					}
					semesterCourse++;
					deleteCourse(course);
				}
				currentCourse = currentCourse->next;
			}
		}
		if (!currentLecturer->totalCourse || !semesterCourse)
			cout << "Oops, you don't have any course yet in this semester. Have a rest!\n";
		cout << "\n";
		deleteLecturers(lecturers);
	}
	else {
		CourseInfo* courseList = nullptr;
		readCourseListFromFile(courseList, academicYear, semester);
		CourseInfo* currentCourse = courseList;
		if (currentCourse != nullptr) {
			cout << "\t\t\t\t\t\tALL COURSES\n\n";
			cout << "\t" << setw(15) << " Course ID |" << setw(16) << " Default class |" << setw(15) << " Lecturer |"
				<< setw(25) << " Duration |" << setw(8) << " Room |" << "\n";
			cout << "\t" << setfill('-') << setw(15) << "+" << setw(16) << "+"
				<< setw(15) << "+" << setw(25) << "+" << setw(8) << "+" << "\n";
			while (currentCourse != nullptr) {
				Course* course = new Course;
				readCourseFromFile(currentCourse, course);
				string duration = dateToString(course->startDate) + " - " + dateToString(course->endDate);
				cout << "\t" << setfill(' ') << setw(14) << currentCourse->courseName << "|" << setw(15)
					<< currentCourse->defaultClass << "|" << setw(14) << course->lecturer.username << "|" << setw(24)
					<< duration << "|" << setw(7) << course->room << "|\n";
				deleteCourse(course);
				currentCourse = currentCourse->next;
			}
		}
		else cout << "Oops, there's not any course in this semester yet.\n";
		cout << "\n";
		deleteCourseInfo(courseList);
	}
}

// 6.2
void viewStudentListOfCourse(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "\t\t\t\t\tLIST OF YOUR COURSES\n\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view student list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print student list.
	cout << "\tPlease enter the number of the course to view student list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Read that course and print.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	cout << "\t\t\t\tSTUDENT LIST OF COURSE " << courseInfo->courseName << "-" << courseInfo->defaultClass << "\n\n";
	printStudentListTable(course->students);
	deleteCourse(course);
	deleteLecturers(lecturers);
}

// 6.3
void viewAttendanceListOfCourseByLecturer(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "\t\t\t\t\tLIST OF YOUR COURSES\n\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view attendance list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print attendance list.
	cout << "\tPlease enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Read that course and print.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Attendance* attendanceDate = new Attendance;
	findAttendanceDateOfCourse(attendanceDate, courseInfo);
	cout << "\t\t\tALL SESSIONS\n\n";
	printAllSessionsTable(attendanceDate);
	toUpper(courseInfo->semester);
	cout << "\t\t\tATTENDANCE LIST OF COURSE " << courseInfo->courseName << " OF "
		<< courseInfo->semester << " SEMESTER, "
		<< courseInfo->academicYear << "-" << courseInfo->academicYear + 1 << "\n\n";
	courseInfo->semester = toFormalCase(courseInfo->semester);
	printAttendanceListOfCourse(course);
	deleteAttendance(attendanceDate);
	deleteCourse(course);
	deleteLecturers(lecturers);
}

// 6.4
void editAnAttendanceByLecturer(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "\t\t\t\t\tLIST OF YOUR COURSES\n\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view attendance list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print attendance list.
	cout << "\tPlease enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Read that course and print.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Attendance* attendanceDate = new Attendance;
	findAttendanceDateOfCourse(attendanceDate, courseInfo);
	cout << "\t\t\tALL SESSIONS\n\n";
	printAllSessionsTable(attendanceDate);
	toUpper(courseInfo->semester);
	cout << "\t\t\tATTENDANCE LIST OF COURSE " << courseInfo->courseName << " OF "
		<< courseInfo->semester << " SEMESTER, "
		<< courseInfo->academicYear << "-" << courseInfo->academicYear + 1 << "\n\n";
	courseInfo->semester = toFormalCase(courseInfo->semester);
	printAttendanceListWithId(course);

	// Ask for student ID and date to edit.
	cout << "Please input the following information:\n";
	string row, studentId, date;
	cout << "\tStudent ID: ";
	cin >> studentId;
	cout << "\tDate to edit: <yyyy>-<mm>-<dd>\n"
		<< "\t              ";
	cin >> date;
	cin.ignore();
	cout << "\n";
	Date editedDate = getDate(date);

	// Check if student exists in course.
	if (!isStudentExistInCourse(studentId, course)) {
		cout << "Edit failed. Error: Student ID doesn't exist in course.\n\n";
		deleteAttendance(attendanceDate);
		deleteCourse(course);
		deleteLecturers(lecturers);
		return;
	}

	// Check if date is valid.
	if (!isSessionDateExist(editedDate, attendanceDate)) {
		cout << "Edit failed. Error: Session date is not valid.\n\n";
		deleteAttendance(attendanceDate);
		deleteCourse(course);
		deleteLecturers(lecturers);
		return;
	}

	// Edit attendance.
	Student* currentStudent = course->students;
	StudentCourseInfo* currentInfo = course->studentCourseInfo;
	while (currentStudent != nullptr) {
		if (currentStudent->studentId == studentId) {
			Attendance* currentAttendance = currentInfo->attendance;
			while (currentAttendance != nullptr) {
				if (currentAttendance->date.year == editedDate.year &&
					currentAttendance->date.month == editedDate.month &&
					currentAttendance->date.day == editedDate.day) {
					cout << "Please enter modified check-in time in the same format:\n";
					cout << "<hh mm>\n\t";
					getline(cin, row);
					cout << "\n";
					Time time = getTime(row);

					// Check if time is valid.
					Attendance* attendance = new Attendance;
					attendance->startTime = currentAttendance->startTime;
					attendance->endTime = currentAttendance->endTime;
					attendance->time = time;
					attendance->next = nullptr;
					if (!isPresent(attendance)) {
						cout << "Edit failed. Error: Input time is not valid.\n\n";
						deleteAttendance(attendance);
						deleteAttendance(attendanceDate);
						deleteCourse(course);
						deleteLecturers(lecturers);
						return;
					}

					currentAttendance->time.hour = time.hour;
					currentAttendance->time.minute = time.minute;
					deleteAttendance(attendance);
					break;
				}
				currentAttendance = currentAttendance->next;
			}
			break;
		}
		currentStudent = currentStudent->next;
		currentInfo = currentInfo->next;
	}

	// Update course file.
	writeCourseToFile(course);

	// Delete linked list.
	deleteAttendance(attendanceDate);
	deleteCourse(course);
	deleteLecturers(lecturers);

	cout << "Edit attendance successfully.\n\n";
}

// 6.5
void importScoreboardFromCsv(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to import scoreboard.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to import csv file.
	cout << "Please enter the number of the course to import scoreboard: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Ask filepath of csv file
	cout << "Please input filepath to csv file: \n\t";
	string filepath;
	cin >> filepath;
	cout << "\n";

	// Try to open file at given filepath.
	ifstream in;
	in.open(filepath);
	while (!in.is_open()) {
		cout << "The file path you entered is not valid. Please input another path or ""0"" to stop: ";
		cin >> filepath;
		cout << "\n";
		if (filepath == "0")
			return;
		in.open(filepath);
	}

	// Read all infomation from csv
	string row, no, studentID, name, midterm, final, lab, bonus;

	// Check whether the csv file is in right format.
	getline(in, row);
	stringstream columnNames(row);
	int columnCount = 0;
	while (getline(columnNames, no, ','))
		columnCount++;
	if (columnCount != 7) {
		cout << "Import unsuccesful. Error: The number of columns is not compatible.\n\n";
		in.close();
		return;
	}

	Student* studentCsv = nullptr;
	Student* curStudent = studentCsv;
	StudentCourseInfo* studentScore = nullptr;
	StudentCourseInfo* curStudentScore = nullptr;
	while (getline(in, row)) {
		stringstream thisRow(row);
		getline(thisRow, no, ',');
		getline(thisRow, studentID, ',');
		getline(thisRow, name, ',');
		getline(thisRow, midterm, ',');
		getline(thisRow, final, ',');
		getline(thisRow, lab, ',');
		getline(thisRow, bonus);

		if (studentCsv == nullptr) {
			studentCsv = new Student;
			curStudent = studentCsv;
		}
		else {
			curStudent->next = new Student;
			curStudent = curStudent->next;
		}
		curStudent->studentId = studentID;
		curStudent->name = name;
		curStudent->next = nullptr;

		if (studentScore == nullptr) {
			studentScore = new StudentCourseInfo;
			curStudentScore = studentScore;
		}
		else {
			curStudentScore->next = new StudentCourseInfo;
			curStudentScore = curStudentScore->next;
		}
		curStudentScore->midterm = stod(midterm);
		curStudentScore->final = stod(final);
		curStudentScore->lab = stod(lab);
		curStudentScore->bonus = stod(bonus);
		curStudentScore->attendance = nullptr;
		curStudentScore->next = nullptr;
	}

	// Update scoreboard to course file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	StudentCourseInfo* curStudentCourseScore = course->studentCourseInfo;
	curStudentScore = studentScore;
	while (curStudentCourseScore != nullptr) {
		curStudentCourseScore->midterm = curStudentScore->midterm;
		curStudentCourseScore->final = curStudentScore->final;
		curStudentCourseScore->lab = curStudentScore->lab;
		curStudentCourseScore->bonus = curStudentScore->bonus;
		curStudentCourseScore = curStudentCourseScore->next;
		curStudentScore = curStudentScore->next;
	}

	// Print scoreboard to confirm.
	cout << "SCOREBOARD LOADED FROM CSV FILE: \n";
	printScoreboardTable(course);
	cout << "Are you sure to import scoreboard to course (Y/N): ";
	string c;
	cin >> c; toUpper(c);
	if (c == "Y") {
		writeCourseToFile(course);
		cout << "Import scoreboard from file csv successfully!\n\n";
	}
	else {
		cout << "Cancel import scoreboard. \n\n";
	}

	// Delete linked list.
	deleteLecturers(lecturers);
	deleteCourse(course);
	deleteStudent(studentCsv);
	deleteStudentCourseInfo(studentScore);

}

// 6.6
void editGradeOfStudent(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "\t\t\t\t\tLIST OF YOUR COURSES\n\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to edit grade.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to edit grade.
	cout << "\tPlease enter the number of the course to edit grade: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Find student in course file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	toUpper(courseInfo->semester);
	cout << "\t\t   STUDENT LIST OF COURSE " << courseInfo->courseName << " OF "
		<< courseInfo->semester << " SEMESTER, "
		<< courseInfo->academicYear << "-" << courseInfo->academicYear + 1 << "\n\n";
	courseInfo->semester = toFormalCase(courseInfo->semester);
	printStudentListTable(course->students);

	// Ask for student ID to edit grade.
	cout << "\tPlease input student ID: ";
	string studentID;
	cin >> studentID;
	cout << "\n";

	Student* curStudent = course->students;
	StudentCourseInfo* curStudentCourseInfo = course->studentCourseInfo;
	while (curStudent != nullptr) {
		if (curStudent->studentId == studentID) {
			break;
		}
		curStudent = curStudent->next;
		curStudentCourseInfo = curStudentCourseInfo->next;
	}

	// If student doesn't exist.
	if (curStudent == nullptr) {
		cout << "\tError: Can not find given student in your course.";
		deleteLecturers(lecturers);
		deleteCourse(course);
		return ;
	}

	// If yes, print student scoreboard.
	cout << "\tScoreboard of the student:\n";
	printScoreboardOfStudent(curStudentCourseInfo);

	// Add what type of score lecturer want to edit and  edit them.
	cout << "\tWhat types of score you want to edit?\n"
		<< "\t\t1. Midterm \n\t\t2. Final \n\t\t3. Lab \n\t\t4. Bonus \n";
	cout << "\tPlease input in an increasing order with a space between: \n\t\t";
	string row;
	cin.ignore();
	getline(cin, row);
	
	cout << "\n";
	stringstream in(row);
	int choiceScore;
	double score;
	while (in >> choiceScore) {
		if (choiceScore == 1) {
			cout << "\tNew midterm score: ";
			cin >> score;
			cout << "\tYou want to change midterm score from " << curStudentCourseInfo->midterm << " to " << setprecision(2)<< score << "? Y/N: ";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->midterm = score;
				cout << "\tThe midterm score has been changed successfully.\n";
			}
			cout << "\n";
		}
		else if (choiceScore == 2) {
			cout << "\tNew final score: ";
			cin >> score; 
			cout << "\tYou want to change final score from " << curStudentCourseInfo-> final << " to " << setprecision(2) << score << "? Y/N: ";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->final = score; 
				cout << "\tThe final score has been changed successfully.\n";
			}
			cout << "\n";
		}
		else if (choiceScore == 3) {
			cout << "\tNew lab score: ";
			cin >> score;
			cout << "\tYou want to change lab score from " << curStudentCourseInfo->lab << " to " << setprecision(2) << score << "? Y/N: ";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->lab = score;
				cout << "\tThe lab score has been changed successfully.\n";
			}
			cout << "\n";
		}
		else if (choiceScore == 4) {
			cout << "\tNew bonus score: ";
			cin >> score;
			cout << "\tYou want to change bonus score from " << curStudentCourseInfo->bonus << " to " << setprecision(2) << score << "? Y/N: \n";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->bonus = score;
				cout << "\tThe lab score has been changed successfully.\n";
			}
			cout << "\n";
		}
	}

	// Print scoreboard of student again.
	cout << "\tThe scoreboard of student after editing: \n";
	printScoreboardOfStudent(curStudentCourseInfo);

	// Write course to file.
	writeCourseToFile(course);

	// Delete linked list.
	deleteLecturers(lecturers);
	deleteCourse(course);
}

// 6.7
void viewScoreboardOfCourse(string lecturerUsername) {
	// Read all courses info of that lecturer.
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer->username != lecturerUsername)
		currentLecturer = currentLecturer->next;
	if (currentLecturer->totalCourse) {
		cout << "\t\t\t\t\tLIST OF YOUR COURSES\n\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view scoreboard.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to import csv file.
	cout << "\tPlease enter the number of the course to view scoreboard: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse || choice < 1) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Print scoreboard.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	printScoreboardTable(course);

	// Delete linked list.
	deleteLecturers(lecturers);
	deleteCourse(course);
}
