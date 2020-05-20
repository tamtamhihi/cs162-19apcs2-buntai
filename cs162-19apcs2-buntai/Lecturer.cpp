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