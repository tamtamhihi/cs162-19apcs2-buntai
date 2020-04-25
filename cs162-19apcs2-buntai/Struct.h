#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
using namespace std;

const int FEMALE = 0, MALE = 1;
const int STAFF = 0, LECTURER = 1, STUDENT = 2;


// ========= ALL STRUCTURES' DEFINITION =========


// ====== GENERAL ======
struct Date {
	int day, month, year;
};
struct Time {
	int hour, minute;
	Time* next;
};
struct AllRole {
	string username;
	string password;
	int role; //0.staff 1.lecturer 2.student
};

// ====== STUDENT ======
struct CourseInfo {
	int academicYear;
	string semester, courseName;
	CourseInfo* next;
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
	Student* next;
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
struct Attendance {
	Date date;
	Time time, startTime, endTime;
	Attendance* next;
};
struct StudentCourseInfo {
	double midterm, final, lab, bonus;
	int status;
	Attendance* attendance;
	StudentCourseInfo* next;
};
struct SessionInfo {
	int day;
	Time startTime, endTime;
	SessionInfo* next;
};
struct Course {
	int academicYear;
	string semester;
	string courseId;
	string courseName;
	string defaultClass;
	Lecturer lecturer;
	Date startDate, endDate;
	int totalSessions;
	int sessionsPerWeek;
	SessionInfo* sessionInfo;
	string room;
	Student* students;
	StudentCourseInfo* studentCourseInfo;
};
struct Staff {
	string username;
	string password;
	string name;
	int gender;
};

#endif