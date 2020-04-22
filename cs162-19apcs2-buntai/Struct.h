#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ========= ALL STRUCTURES' DEFINITION =========


// ====== GENERAL ======
struct Date {
	int day, month, year;
};
struct Time {
	int hour, minute;
};


// ====== STUDENT ======
struct CourseInfo {
	int academicYear;
	string semester, courseName;
};
struct Student {
	string username, password;
	int status;
	string name;
	string studentId;
	int gender;
	Date dob;
	string myClass;
	int numberOfCourse;
	CourseInfo* myCourse;
};


// ====== LECTURER ======
struct Lecturer {
	string username;
	string password;
	string name;
	string title;
	int gender;
};


// ====== STAFF ======
struct Score {
	double midterm, final, lab, bonus;
};
struct Attendance {
	Date* date;
	Time* time;
};
struct Course {
	string courseId;
	string courseName;
	string defaultClass;
	Lecturer lecturer;
	Date startDate, endDate;
	int* dayOfWeek;
	Time* startTime, * endTime;
	int numberOfSessions;
	string room;
	Student* students;
	int* status;
	Score* score;
	Attendance* attendance;
};
struct Staff {
	string username;
	string password;
	string name;
	int gender;
};