#include "Function.h"


/*
========= SUPPORTING FUNCTIONS DEFINITION =========
*/



/*
====== GENERAL ======
*/

// Turn a string to uppercase.
void toUpper(string& text) {
	for (char& c : text)
		c = toupper(c);
}

// Turn a string to lowercase.
void toLower(string& text) {
	for (char& c : text)
		c = tolower(c);
}

// Generate username from full name for new students.
string toUsername(string fullName) {
	stringstream name(fullName);
	string word, username = "";
	while (name >> word)
		username += word[0]; // add the first letter of every 
	for (int i = 1; i < word.size(); ++i)
		username += word[i];
	toLower(username);
	return username;
}

// Generate password from date of birth for new students.
string toPassword(Date dob) {
	string password = "";
	if (dob.day < 10)
		password += "0";
	password += to_string(dob.day);
	if (dob.month < 10)
		password += "0";
	password += to_string(dob.month);
	password += to_string(dob.year);
	return password;
}

// Retrieve date of birth from a string of form "yyyy-mm-dd".
Date getDate(string date) {
	stringstream DATEss(date);
	string year, month, day;
	getline(DATEss, year, '-');
	getline(DATEss, month, '-');
	getline(DATEss, day, '-');
	Date DATE;
	DATE.day = stoi(day);
	DATE.month = stoi(month);
	DATE.year = stoi(year);
	return DATE;
}

// Turn a full name string to formal case 
// (uppercase for first letter of each word).
string toFormalCase(string name) {
	stringstream text(name);
	string word, formalName = "";
	while (text >> word) {
		if (formalName != "")
			formalName += " ";
		toLower(word);
		word[0] = toupper(word[0]);
		formalName += word;
	}
	return formalName;
}

// Check if a class exists.
bool isClassExist(string className) {
	ifstream in("Database/Class/Classes.txt");
	if (in.is_open()) {
		string word;
		while (in >> word)
			if (word == className)
				return true;
	}
	return false;
}

// Convert day of week to number.
int dayToNumber(string dayOfWeek) {
	if (dayOfWeek == "Sun") return 1;
	if (dayOfWeek == "Mon") return 2;
	if (dayOfWeek == "Tue") return 3;
	if (dayOfWeek == "Wed") return 4;
	if (dayOfWeek == "Thu") return 5;
	if (dayOfWeek == "Fri") return 6;
	if (dayOfWeek == "Sat") return 7;
}

// Get day of week from given date.
int getDayOfWeek(Date date) {
	static int t[] = { 0, 3, 2, 5, 0, 3,
					   5, 1, 4, 6, 2, 4 };
	date.year -= date.month < 3;
	return (date.year + date.year / 4 - date.year / 100 +
		date.year / 400 + t[date.month - 1] + date.day) % 7 + 1;
}

// Determine leap year.
bool isLeap(int year) {
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

// Get number of days in given month.
int daysInMonth(int month, int year) {
	switch (month) {
		case 2:
			if (isLeap(year)) return 29;
			else return 28;
		case 4:
		case 6:
		case 9:
		case 11: return 30;
		default: return 31;
	}
}

// Calculate days between two dates.
int calculateDaysBetDates(Date startDate, Date endDate) {
	int daysBetween = 0;
	for (int i = startDate.year; i < endDate.year; ++i)
		daysBetween += (isLeap(i)) ? 366 : 365;
	if (startDate.month < endDate.month)
		for (int i = startDate.month; i < endDate.month; ++i)
			daysBetween += daysInMonth(i, endDate.year);
	else
		for (int i = endDate.month; i < startDate.month; ++i)
			daysBetween -= daysInMonth(i, endDate.year - 1);
	daysBetween += endDate.day - startDate.day;
	return daysBetween;
}

// Calculate total sessions based on start date, end date and day of week.
int calculateTotalSessions(Course* course) {
	int daysInStudyWeeks = calculateDaysBetDates(course->startDate, course->endDate) + 1;
	daysInStudyWeeks += getDayOfWeek(course->startDate) - 1;
	daysInStudyWeeks += 7 - getDayOfWeek(course->endDate);
	return daysInStudyWeeks / 7 * course->sessionsPerWeek;
}

// Determine the date for next session.
// Note: applied for days <= 7.
Date dateAfterDays(Date startDate, int days) {
	int day = startDate.day, month = startDate.month, year = startDate.year;
	day += days;
	if (day > daysInMonth(startDate.month, startDate.year)) {
		day -= daysInMonth(startDate.month, startDate.year);
		if (startDate.month == 12)
			year++;
	}
	return { day, month, year };
}

// Retrieve time from a string of form "<hour> <minute>"
Time getTime(string time) {
	stringstream TIME(time);
	string HOUR, MINUTE;
	TIME >> HOUR >> MINUTE;
	return { stoi(HOUR), stoi(MINUTE) };
}

// Convert day in number to text format.
string numToDay(int day) {
	if (day == 1) return "Sun";
	if (day == 2) return "Mon";
	if (day == 3) return "Tue";
	if (day == 4) return "Wed";
	if (day == 5) return "Thu";
	if (day == 6) return "Fri";
	if (day == 7) return "Sat";
}

// Check if a course exists.
bool isCourseExist(CourseInfo* courseInfo) {
	string filepath = "Database/"
		+ to_string(courseInfo->academicYear) + "-"
		+ to_string(courseInfo->academicYear + 1) + "/"
		+ courseInfo->semester + "/Courses.txt";
	ifstream in(filepath);
	if (in.is_open()) {
		string courseId, defaultClass;
		while (in >> courseId >> defaultClass)
			if (courseInfo->courseName == courseId && courseInfo->defaultClass == defaultClass)
				return true;
	}
	return false;
}

// Check if a lecturer with given username exists.
bool isLecturerExist(string lecturerAccount) {
	ifstream in("Database/Lecturer.txt");
	string user, name, title;
	int gender;
	while (in >> user) {
		getline(in, name, '\n');
		getline(in, title, '\n');
		in >> gender;
		if (user == lecturerAccount)
			return true;
		in.get();
	}
	return false;
}

bool isStudentIdExist(string studentId) {
	ifstream in("Database/Class/Classes.txt");
	string className;
	while (in >> className) {
		Student* studentList = nullptr;
		readClassFromFile(className, studentList);
		Student* currentStudent = studentList;
		while (currentStudent != nullptr) {
			if (currentStudent->studentId == studentId) {
				deleteStudentList(studentList);
				in.close();
				return true;
			}
			currentStudent = currentStudent->next;
		}
		deleteStudentList(studentList);
	}
	in.close();
	return false;
}



/* 
====== APP-RELATED ======
*/

// Write/overwrite a "<class-name>.txt" file from a Student linked list.
void writeClassToFile(Student*& students, string className) {
	ofstream out;
	toUpper(className);
	out.open("Database/Class/" + className + ".txt");
	Student* currentStudent = students;
	while (currentStudent != nullptr) {
		out << currentStudent->username << "\n";
		out << currentStudent->status << "\n";
		out << currentStudent->name << "\n";
		out << currentStudent->studentId << "\n";
		out << currentStudent->gender << "\n";
		out << currentStudent->dob.day << " "
			<< currentStudent->dob.month << " "
			<< currentStudent->dob.year << "\n";
		out << currentStudent->numberOfCourse << "\n";
		CourseInfo* currentCourseInfo = currentStudent->myCourse;
		while (currentCourseInfo != nullptr) {
			out << currentCourseInfo->academicYear << " "
				<< currentCourseInfo->academicYear + 1 << " "
				<< currentCourseInfo->semester << " "
				<< currentCourseInfo->courseName << " "
				<< currentCourseInfo->defaultClass << "\n";
			currentCourseInfo = currentCourseInfo->next;
		}
		out << "\n";
		currentStudent = currentStudent->next;
	}
	out.close();
}

// Check if an existent username is a version of the new username.
// E.g: "nnbtam1" is a version of "nnbtam" -> returns 1.
int getVersion(string existent, string username) {
	/*
	Check if the username and the existent username in User.txt is the same (with numbers).
	*/
	if (existent == username)
		return 0;
	if (existent.size() <= username.size())
		return -1;
	for (int i = 0; i < username.size(); ++i)
		if (username[i] != existent[i])
			return -1;
	int version = 0;
	for (int i = username.size(); i < existent.size(); ++i)
		version = version * 10 + (existent[i] - '0');
	return version;
}

// Get a valid username for new students from their full name.
string getValidUsername(string name) {
	string username = toUsername(name);
	ifstream in("Database/User.txt");
	if (in.is_open()) {
		string name, password, type;
		int maxVersion = -1;
		while (in >> name >> password >> type) {
			int version = getVersion(name, username);
			if (version > maxVersion)
				maxVersion = version;
		}
		if (maxVersion < 0)
			return username;
		username += to_string(maxVersion + 1);
		return username;
	}
	return username;
}

// Add a single user to "User.txt" file using username, password and account type.
void addUser(string username, string password, int type) {
	ofstream out("Database/User.txt", ios::app);
	out << username << "\n" << password << "\n" << type << "\n";
	out.close();
}

// Add a Student linked list to "User.txt".
void addStudentUsers(Student*& studentList) {
	ofstream out("Database/User.txt", ios::app);
	Student* current = studentList;
	while (current != nullptr) {
		out << current->username << "\n" 
			<< current->password << "\n2\n";
		current = current->next;
	}
	out.close();
}

// Add a className to "Classes.txt" if it hasn't been there.
void addClass(string className) {
	ifstream in("Database/Class/Classes.txt");
	if (in.is_open()) {
		string name;
		while (in >> name)
			if (name == className) {
				in.close();
				return;
			}
		in.close();
	}
	ofstream out("Database/Class/Classes.txt", ios::app);
	out << className << "\n";
	out.close();
}

// Delete a Student linked list with CourseInfo.
void deleteStudentList(Student*& studentList) {
	while (studentList != nullptr) {
		deleteCourseInfo(studentList->myCourse);
		Student* temp = studentList;
		studentList = studentList->next;
		delete temp;
	}
}
void deleteCourseInfo(CourseInfo*& courseInfo) {
	while (courseInfo != nullptr) {
		CourseInfo* temp = courseInfo;
		courseInfo = courseInfo->next;
		delete temp;
	}
}

// Search "User.txt" to find password of a username.
string findPasswordFromUsername(string username) {
	ifstream in("Database/User.txt");
	string user, pw, type;
	while (in >> user >> pw >> type)
		if (user == username)
			return pw;
	return NULL;
}

// Search "Lecturer.txt" to find a lecture from username.
Lecturer findLecturerFromUsername(string username) {
	Lecturer leturer;
	ifstream in("Database/Lecturer.txt");
	string user, name, title;
	int gender;
	while (in >> user) {
		getline(in, name, '\n');
		getline(in, title, '\n');
		in >> gender;
		if (user == username) {
			leturer.username = user;
			leturer.name = name;
			leturer.title = title;
			leturer.gender = gender;
			return leturer;
		}
		in.get();
	}
}

// Print a single student's info.
void printStudentInfo(Student*& student) {
	cout << "Student info:\n";
	cout << "\tUsername: " << student->username << "\n";
	cout << "\tPassword: " << student->password << "\n";
	cout << "\tStatus: ";
	if (student->status) cout << "Available\n";
	else cout << "Dropped\n";
	cout << "\tName: " << student->name << "\n";
	cout << "\tStudent ID: " << student->studentId << "\n";
	cout << "\tGender: ";
	if (student->gender == FEMALE) cout << "female\n";
	else cout << "male\n";
	cout << "\tDate of birth: " << student->dob.day << "-" 
		<< student->dob.month << "-" << student->dob.year << "\n";
	cout << "\tCourses: " << student->numberOfCourse << "\n";
	CourseInfo* courseInfo = student->myCourse;
	for (int i = 0; i < student->numberOfCourse; ++i) {
		cout << "\t- " << courseInfo->academicYear << "-" 
			<< courseInfo->academicYear + 1 << ", " 
			<< courseInfo->semester << " semester, " 
			<< courseInfo->courseName << "\n";
	}
	cout << "\n";
}

// Read a "<class-name>.txt" file to a Student linked list.
void readClassFromFile(string className, Student*& studentList) {
	ifstream in("Database/Class/" + className + ".txt");
	Student* current = studentList;
	string username, name, id, semester, courseName, defaultClass;
	int status, gender, day, month, year, numOfCourse, academicYear;
	while (in >> username) {
		in >> status;
		in.ignore();
		getline(in, name);
		in >> id >> gender >> day >> month >> year >> numOfCourse;
		if (studentList == nullptr) {
			studentList = new Student;
			current = studentList;
		}
		else {
			current->next = new Student;
			current = current->next;
		}
		current->username = username;
		current->status = status;
		current->name = name;
		current->studentId = id;
		current->gender = gender;
		current->dob = Date{ day, month, year };
		current->numberOfCourse = numOfCourse;
		current->myCourse = nullptr;
		CourseInfo* currentCourse = current->myCourse;
		for (int i = 0; i < numOfCourse; ++i) {
			in >> academicYear;
			in >> academicYear;
			in >> semester >> courseName >> defaultClass;
			if (current->myCourse == nullptr) {
				current->myCourse = new CourseInfo;
				currentCourse = current->myCourse;
			}
			else {
				currentCourse->next = new CourseInfo;
				currentCourse = currentCourse->next;
			}
			currentCourse->academicYear = academicYear - 1;
			currentCourse->courseName = courseName;
			currentCourse->semester = semester;
			currentCourse->defaultClass = defaultClass;
			currentCourse->next = nullptr;
		}
		current->next = nullptr;
	}
	in.close();
}

// Read a "<course-name>-<default-class>.txt" file to a Course.
void readCourseFromFile(CourseInfo* courseInfo, Course*& course) {
	// File path of course including academic year, semester and course name.
	string filepath = "Database/"
		+ to_string(courseInfo->academicYear) + "-"
		+ to_string(courseInfo->academicYear + 1) + "/"
		+ courseInfo->semester + "/"
		+ courseInfo->courseName + "-"
		+ courseInfo->defaultClass + ".txt";
	ifstream in(filepath);

	// Read all information of course.
	if (in.is_open()) {
		course->academicYear = courseInfo->academicYear;
		course->semester = courseInfo->semester;
		course->defaultClass = courseInfo->defaultClass;
		// Read general information.
		in >> course->courseId;
		in.ignore();
		getline(in, course->courseName);
		in >> course->defaultClass >> course->lecturer.username;
		int day, month, year, hour, minute;
		in >> day >> month >> year;
		course->startDate = Date{ day, month, year };
		in >> day >> month >> year;
		course->endDate = Date{ day, month, year };
		in >> course->totalSessions >> course->sessionsPerWeek;
		course->sessionInfo = nullptr;
		SessionInfo* currentSession = nullptr;
		for (int i = 0; i < course->sessionsPerWeek; ++i) {
			if (course->sessionInfo == nullptr) {
				course->sessionInfo = new SessionInfo;
				currentSession = course->sessionInfo;
			}
			else {
				currentSession->next = new SessionInfo;
				currentSession = currentSession->next;
			}
			in >> day >> hour >> minute;
			currentSession->day = day;
			currentSession->startTime = Time{ hour, minute };
			in >> hour >> minute;
			currentSession->endTime = Time{ hour, minute };
			currentSession->next = nullptr;
		}
		in >> course->room;
		
		// Read enrolled students information.
		course->students = nullptr;
		course->studentCourseInfo = nullptr;
		Student* currentStudent = nullptr;
		StudentCourseInfo* currentStudentInfo = nullptr;
		string username, name, id;
		int gender, status;
		double midterm, final, lab, bonus;
		while (in >> username) {
			in.ignore();
			getline(in, name);
			in >> id >> gender >> day >> month >> year >> status
				>> midterm >> final >> lab >> bonus;
			if (course->students == nullptr) {
				course->students = new Student;
				currentStudent = course->students;
				course->studentCourseInfo = new StudentCourseInfo;
				currentStudentInfo = course->studentCourseInfo;
			}
			else {
				currentStudent->next = new Student;
				currentStudent = currentStudent->next;
				currentStudentInfo->next = new StudentCourseInfo;
				currentStudentInfo = currentStudentInfo->next;
			}
			currentStudent->username = username;
			currentStudent->name = name;
			currentStudent->studentId = id;
			currentStudent->gender = gender;
			currentStudent->dob = Date{ day, month, year };
			currentStudent->next = nullptr;
			currentStudentInfo->status = status;
			currentStudentInfo->midterm = midterm;
			currentStudentInfo->final = final;
			currentStudentInfo->lab = lab;
			currentStudentInfo->bonus = bonus;
			currentStudentInfo->next = nullptr;
			currentStudentInfo->attendance = nullptr;
			Attendance* currentAttendance = nullptr;
			for (int i = 0; i < course->totalSessions; ++i) {
				if (currentStudentInfo->attendance == nullptr) {
					currentStudentInfo->attendance = new Attendance;
					currentAttendance = currentStudentInfo->attendance;
				}
				else {
					currentAttendance->next = new Attendance;
					currentAttendance = currentAttendance->next;
				}
				in >> day >> month >> year >> hour >> minute;
				currentAttendance->date = Date{ day, month, year };
				currentAttendance->startTime = Time{ hour, minute };
				in >> hour >> minute;
				currentAttendance->endTime = Time{ hour, minute };
				in >> hour >> minute;
				currentAttendance->time = Time{ hour, minute };
				currentAttendance->next = nullptr;
			}
		}

		in.close();
	}
}

// Write/overwrite a "<course-name>.txt" file from a Course.
void writeCourseToFile(Course*& course) {
	// File path of course including academic year, semester and course name.
	string filepath = "Database/"
		+ to_string(course->academicYear) + "-"
		+ to_string(course->academicYear + 1) + "/"
		+ course->semester + "/"
		+ course->courseId + "-"
		+ course->defaultClass + ".txt";
	ofstream out(filepath);
	out << course->courseId << "\n" 
		<< course->courseName << "\n" 
		<< course->defaultClass << "\n" 
		<< course->lecturer.username << "\n"
		<< course->startDate.day << " " << course->startDate.month << " " << course->startDate.year << "\n"
		<< course->endDate.day << " " << course->endDate.month << " " << course->endDate.year << "\n"
		<< course->totalSessions << "\n"
		<< course->sessionsPerWeek << "\n";
	SessionInfo* currentSession = course->sessionInfo;
	while (currentSession != nullptr) {
		out << currentSession->day << " " 
			<< currentSession->startTime.hour << " " << currentSession->startTime.minute << " " 
			<< currentSession->endTime.hour << " " << currentSession->endTime.minute << "\n";
		currentSession = currentSession->next;
	}
	out << course->room << "\n\n";
	Student* currentStudent = course->students;
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	while (currentStudent != nullptr) {
		out << currentStudent->username << "\n"
			<< currentStudent->name << "\n"
			<< currentStudent->studentId << "\n"
			<< currentStudent->gender << "\n"
			<< currentStudent->dob.day << " " << currentStudent->dob.month << " " << currentStudent->dob.year << "\n"
			<< currentStudentInfo->status << "\n"
			<< currentStudentInfo->midterm << " " << currentStudentInfo->final << " " 
			<< currentStudentInfo->lab << " " << currentStudentInfo->bonus << "\n";
		Attendance* currentAttendance = currentStudentInfo->attendance;
		while (currentAttendance != nullptr) {
			out << currentAttendance->date.day << " "
				<< currentAttendance->date.month << " "
				<< currentAttendance->date.year << " "
				<< currentAttendance->startTime.hour << " " << currentAttendance->startTime.minute << " "
				<< currentAttendance->endTime.hour << " " << currentAttendance->endTime.minute << " "
				<< currentAttendance->time.hour << " " << currentAttendance->time.minute << "\n";
			currentAttendance = currentAttendance->next;
		}
		out << "\n";
		currentStudent = currentStudent->next;
		currentStudentInfo = currentStudentInfo->next;
	}
	out.close();
}

// Delete linked lists associated with Course.
void deleteSessionInfo(SessionInfo*& sessionInfo) {
	while (sessionInfo != nullptr) {
		SessionInfo* temp = sessionInfo;
		sessionInfo = sessionInfo->next;
		delete temp;
	}
}
void deleteStudent(Student*& students) {
	while (students != nullptr) {
		Student* temp = students;
		students = students->next;
		delete temp;
	}
}
void deleteAttendance(Attendance*& attendance) {
	while (attendance != nullptr) {
		Attendance* temp = attendance;
		attendance = attendance->next;
		delete temp;
	}
}
void deleteStudentCourseInfo(StudentCourseInfo*& studentCourseInfo) {
	while (studentCourseInfo != nullptr) {
		deleteAttendance(studentCourseInfo->attendance);
		StudentCourseInfo* temp = studentCourseInfo;
		studentCourseInfo = studentCourseInfo->next;
		delete temp;
	}
}
void deleteCourse(Course*& myCourse) {
	deleteSessionInfo(myCourse->sessionInfo);
	deleteStudent(myCourse->students);
	deleteStudentCourseInfo(myCourse->studentCourseInfo);
}

// Read "Courses.txt" file to a course list.
void readCourseListFromFile(CourseInfo*& courseList, string academicYear, string semester) {
	ifstream in;
	in.open("Database/" + academicYear + "/" + semester + "/" + "Courses.txt");
	if (in.is_open()) {
		string currentCourseID;
		CourseInfo* currentCourseList = courseList;
		while (in >> currentCourseID) {
			if (courseList == nullptr) {
				courseList = new CourseInfo;
				currentCourseList = courseList;
			}
			else {
				currentCourseList->next = new CourseInfo;
				currentCourseList = currentCourseList->next;
			}
			currentCourseList->courseName = currentCourseID;
			in >> currentCourseList->defaultClass;
			currentCourseList->next = nullptr;
		}
		in.close();
	}
}

// Write/overwrite a course list into "Courses.txt" file.
void writeCourseListToFile(CourseInfo* courseList, string academicYear, string semester) {
	ofstream out;
	out.open("Database/" + academicYear + "/" + semester + "/" + "Courses.txt");
	if (out.is_open()) {
		while (courseList != nullptr) {
			out << courseList->courseName << " " << courseList->defaultClass << "/n";
			courseList = courseList->next;
		}
		out.close();
	}
}