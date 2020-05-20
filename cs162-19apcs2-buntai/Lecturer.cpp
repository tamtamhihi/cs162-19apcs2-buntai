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

	cout << "\nInput the action:\n";
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
						cout << "\t\t     YOUR COURSES\n";
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
			cout << "\t\t     ALL COURSES\n";
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

	// Ask for the specific course to print student list.
	string row, ay, courseId, semester, defaultClass;
	int academicYear;
	cout << "\nPlease enter information of course to view student list:\n\t";
	cout << "<academic-year>,<semester>,<course-Id>,<default-class>\n\t";
	getline(cin, row);
	stringstream info(row);
	getline(info, ay, ',');
	academicYear = stoi(ay);
	getline(info, semester, ','); semester = toFormalCase(semester);
	getline(info, courseId, ','); toUpper(courseId);
	getline(info, defaultClass, ','); toUpper(defaultClass);
	cout << "\n";
	while (!isCourseInCourseList(academicYear, semester, courseId, currentLecturer->myCourse)) {
		cout << "You don't have the inputted course. Please try again:\n\t";
		getline(cin, row);
		info.clear();
		info << row;
		getline(info, ay, ',');
		academicYear = stoi(ay);
		getline(info, semester, ','); semester = toFormalCase(semester);
		getline(info, courseId, ','); toUpper(courseId);
		getline(info, defaultClass, ','); toUpper(defaultClass);
		cout << "\n";
	}

	// Read that course and print.
	Course* course = new Course;
	CourseInfo* courseInfo = new CourseInfo;
	courseInfo->academicYear = academicYear;
	courseInfo->semester = semester;
	courseInfo->courseName = courseId;
	courseInfo->defaultClass = defaultClass;
	courseInfo->next = nullptr;
	readCourseFromFile(courseInfo, course);
	cout << "Student list of course " << courseId << "-" << defaultClass << ":\n";
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

	// Get input.
	cout << "Please input the following information:\n";
	int academicYear;
	CourseInfo* courseInfo = new CourseInfo;
	cout << "\tAcademic year: ";
	cin >> courseInfo->academicYear;
	cout << "\tSemester: ";
	cin >> courseInfo->semester; courseInfo->semester = toFormalCase(courseInfo->semester);
	cout << "\tCourese ID: ";
	cin >> courseInfo->courseName; toUpper(courseInfo->courseName);
	cout << "\tDefault class: ";
	cin >> courseInfo->defaultClass; toUpper(courseInfo->defaultClass);
	courseInfo->next = nullptr;
	cout << "\tFilepath to csv file: ";
	string filepath;
	cin >> filepath;
	cout << "\n";

	// Check if course exist.
	if (!isCourseExist(courseInfo)) {
		cout << "Error: Course not found.\n\n";
		return;
	}

	// Check whether given course is in lecturer's courses.
	if (!isLecturerCourse(courseInfo, lecturerUsername)) {
		cout << "Sorry, you do not have a right to update scoreboard of this course. \n\n";
		return;
	}

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
		curStudentScore->midterm = stoi(midterm);
		curStudentScore->final = stoi(final);
		curStudentScore->lab = stoi(lab);
		curStudentScore->bonus = stoi(bonus);
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
	deleteCourseInfo(courseInfo);
	deleteCourse(course);
	deleteStudent(studentCsv);
	deleteStudentCourseInfo(studentScore);

	// Annoucement.
	cout << "Import scoreboard from file csv successfully!\n\n";
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
		cout << "Sorry, you have no courses to view student list.\n\n";
		deleteLecturers(lecturers);
		return;
	}

	// Get input.
	cout << "Please input the following information:\n";
	int academicYear;
	CourseInfo* courseInfo = new CourseInfo;
	cout << "\tAcademic year: ";
	cin >> courseInfo->academicYear;
	cout << "\tSemester: ";
	cin >> courseInfo->semester; courseInfo->semester = toFormalCase(courseInfo->semester);
	cout << "\tCourese ID: ";
	cin >> courseInfo->courseName; toUpper(courseInfo->courseName);
	cout << "\tDefault class: ";
	cin >> courseInfo->defaultClass; toUpper(courseInfo->defaultClass);
	courseInfo->next = nullptr;
	cout << "\n";

	// Check if course exist.
	if (!isCourseExist(courseInfo)) {
		cout << "Error: Course not found.\n\n";
		return;
	}

	// Check whether given course is in lecturer's courses.
	if (!isLecturerCourse(courseInfo, lecturerUsername)) {
		cout << "Sorry, you do not have a right to view scoreboard of this course. \n\n";
		return;
	}
	// If yes print scoreboard.
	cout << "Loading scoreboard...\n";
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	printScoreboardTable(course);
	cout << "\n";

	deleteCourseInfo(courseInfo);
	deleteCourse(course);
	return;
}
