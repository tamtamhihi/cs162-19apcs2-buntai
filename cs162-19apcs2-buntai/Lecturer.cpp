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