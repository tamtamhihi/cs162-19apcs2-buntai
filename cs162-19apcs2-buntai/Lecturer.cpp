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
						cout << "\tYOUR COURSES\n";
						cout << "\t" << setw(20) << "Course ID |" << " Default class\n";
						cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << " " << "\n";
					}
					string year = to_string(currentCourse->academicYear) + "-"
						+ to_string(currentCourse->academicYear + 1);
					cout << "\t" << setfill(' ') << setw(19) << currentCourse->courseName << "| "
						<< currentCourse->defaultClass << "\n";
					semesterCourse++;
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
			cout << "\tALL COURSES\n";
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
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view student list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print student list.
	cout << "Please enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse) {
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
	cout << "STUDENT LIST OF COURSE " << courseInfo->courseName << "-" << courseInfo->defaultClass << ":\n";
	printStudentListTable(course->students);
	deleteCourseInfo(courseInfo);
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
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view attendance list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print attendance list.
	cout << "Please enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Read that course and print.
	toUpper(courseInfo->semester);
	cout << "\t\tATTENDANCE LIST OF COURSE " << courseInfo->courseName << " OF "
		<< courseInfo->semester << " SEMESTER, "
		<< courseInfo->academicYear << "-" << courseInfo->academicYear + 1 << ":\n";
	courseInfo->semester = toFormalCase(courseInfo->semester);
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Attendance* attendanceDate = new Attendance;
	findAttendanceDateOfCourse(attendanceDate, courseInfo);
	printAllSessionsTable(attendanceDate);
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
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view attendance list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to print attendance list.
	cout << "Please enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Read that course and print.
	toUpper(courseInfo->semester);
	cout << "\t\tATTENDANCE LIST OF COURSE " << courseInfo->courseName << " OF "
		<< courseInfo->semester << " SEMESTER, "
		<< courseInfo->academicYear << "-" << courseInfo->academicYear + 1 << ":\n";
	courseInfo->semester = toFormalCase(courseInfo->semester);
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Attendance* attendanceDate = new Attendance;
	findAttendanceDateOfCourse(attendanceDate, courseInfo);
	printAllSessionsTable(attendanceDate);
	printAttendanceListWithId(course);

	// Ask for student ID and date to edit.
	cout << "Please input student id and date with the same format:\n";
	cout << "<student-id>,<yyyy-mm-dd>\n\t";
	string row, studentId, date;
	cin.ignore();
	getline(cin, row);
	cout << "\n";
	stringstream info(row);
	getline(info, studentId, ',');
	getline(info, date, ',');
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
	while (choice > currentLecturer->totalCourse) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Ask filepath of csv file
	cout << "Please input filepath to csv file: \n";
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
	writeCourseToFile(course);

	// Delete linked list.
	deleteLecturers(lecturers);
	deleteCourse(course);
	deleteStudent(studentCsv);
	deleteStudentCourseInfo(studentScore);

	// Announcement.
	cout << "Import scoreboard from file csv successfully!\n\n";
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
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to edit grade.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to edit grade.
	cout << "Please enter the number of the course to edit grade: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse) {
		cout << "Course number not available. Please enter again: ";
		cin >> choice;
	}
	cout << "\n";
	CourseInfo* courseInfo = currentLecturer->myCourse;
	for (int i = 0; i < choice - 1; ++i)
		courseInfo = courseInfo->next;

	// Ask for student ID to edit grade.
	cout << "Please input student ID: ";
	string studentID;  
	cin >> studentID;
	cout << "\n";

	// Find student in course file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
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
		cout << "Error: Can not find given student in your course.";
		deleteCourse(course);
		deleteCourseInfo(courseInfo);
		return ;
	}

	// If yes, print student scoreboard.
	cout << "Scoreboard of the student:\n";
	printScoreboardOfStudent(curStudentCourseInfo);

	// Add what type of score lecturer want to edit and  edit them.
	cout << "What types of score you want to edit?\n"
		<< "\t1. Midterm \n\t2. Final \n\t3. Lab \n\t4. Bonus \n";
	cout << "Please input in an increasing order with a space between. \n\t";
	string row;
	cin.ignore();
	getline(cin, row);
	
	cout << "\n";
	stringstream in(row);
	int choiceScore;
	double score;
	while (in >> choiceScore) {
		if (choiceScore == 1) {
			cout << "New midterm score: ";
			cin >> score;
			cout << "You want to change midterm score from " << curStudentCourseInfo->midterm << " to " << setprecision(2)<< score << "? Y/N \n\t";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->midterm = score;
				cout << "The midterm score has been changed successfully.\n\n";
			}
		}
		else if (choiceScore == 2) {
			cout << "New final score: ";
			cin >> score; 
			cout << "You want to change final score from " << curStudentCourseInfo-> final << " to " << setprecision(2) << score << "? Y/N \n\t";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->final = score; 
				cout << "The final score has been changed successfully.\n\n";
			}
		}
		else if (choiceScore == 3) {
			cout << "New lab score: ";
			cin >> score;
			cout << "You want to change lab score from " << curStudentCourseInfo->lab << " to " << setprecision(2) << score << "? Y/N \n\t";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->lab = score;
				cout << "The lab score has been changed successfully.\n\n";
			}
		}
		else if (choiceScore == 4) {
			cout << "New bonus score: ";
			cin >> score;
			cout << "You want to change bonus score from " << curStudentCourseInfo->bonus << " to " << setprecision(2) << score << "? Y/N \n\t";
			cin >> row; toUpper(row);
			if (row == "Y") {
				curStudentCourseInfo->bonus = score;
				cout << "The lab score has been changed successfully.\n\n";
			}
		}
	}

	// Print scoreboard of student again.
	cout << "The scoreboard of student after editing: \n";
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
		cout << "LIST OF YOUR COURSES:\n";
		printCourseListTable(currentLecturer->myCourse);
	}
	else {
		cout << "Sorry, you have no courses to view scoreboard.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Ask for specific course to import csv file.
	cout << "Please enter the number of the course to view attendance list: ";
	int choice;
	cin >> choice;
	while (choice > currentLecturer->totalCourse) {
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
	cout << "\n";

	// Delete linked list.
	deleteLecturers(lecturers);
	deleteCourse(course);
}
