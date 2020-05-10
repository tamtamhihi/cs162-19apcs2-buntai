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

// Generate password from name for new staff/lecturer.
string toPasswordGeneral(string name) {
	stringstream NAME(name);
	string password = "", tmp;
	NAME >> tmp;
	while (NAME >> tmp) {
		toLower(tmp);
		password += tmp;
	}
	password += "123";
	return password;
}

// Check if a date string is in form yyyy-mm-dd.
bool isDateStringSuitable(string date) {
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	for (int i = 0; i < 10; ++i)
		if (i != 4 && i != 7 && (date[i] - '0' < 0 || date[i] - '0' > 9))
			return false;
	return true;
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

// Turn a date into a string.
string dateToString(Date date) {
	string dateString = "";
	if (date.day < 10) dateString += "0";
	dateString += to_string(date.day) + "-";
	if (date.month < 10) dateString += "0";
	dateString += to_string(date.month) + "-" + to_string(date.year);
	return dateString;
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
	int gender, totalCourse;
	while (in >> user) {
		if (user == lecturerAccount) {
			in.close();
			return true;
		}
		in.ignore();
		getline(in, name);
		getline(in, title);
		in >> gender >> totalCourse;
		in.ignore();
		for (int i = 0; i < totalCourse; ++i) getline(in, name);
	}
	return false;
}

// Check if a studentId already exists in all classes.
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

// Check if an academic year already exists in AcademicYears.txt.
bool isAcademicYearExist(int academicYear) {
	ifstream in("Database/AcademicYears.txt");
	int year, numberOfSemester;
	if (in.is_open()) {
		while (in >> year) {
			if (year == academicYear) {
				in.close();
				return true;
			}
			in >> year >> numberOfSemester;
			in.ignore();
			for (int i = 0; i < numberOfSemester; ++i)
				in.ignore();
		}
		in.close();
	}
	return false;
}

// Check if a semester of an academic year already exists.
bool isSemesterExist(int academicYear, string mySemester) {
	ifstream in("Database/AcademicYears.txt");
	string semester;
	int year, numberOfSemester;
	if (in.is_open()) {
		while (in >> year) {
			in >> year >> numberOfSemester;
			for (int i = 0; i < numberOfSemester; ++i) {
				in >> semester;
				if (year - 1 == academicYear && semester == mySemester) {
					in.close();
					return true;
				}
			}
		}
		in.close();
	}
	return false;
}

// Remove all files in the semester directory.
void removeSemesterDirectory(int academicYear, string semester) {
	string semesterDir = "Database\\" + to_string(academicYear) + "-" +
		to_string(academicYear + 1) + "\\" + semester;
	string coursesDir = semesterDir + "/Courses.txt";
	ifstream in(coursesDir);
	if (!in.is_open()) {
		remove(semesterDir.c_str());
		return;
	}
	string courseId, defaultClass, courseDir;
	while (in >> courseId >> defaultClass) {
		courseDir = semesterDir + "/" + courseId + "-" + defaultClass + ".txt";
		remove(courseDir.c_str());
	}
	in.close();
	remove(coursesDir.c_str());
	string command = "rd " + semesterDir;
	system(command.c_str());
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

// Add a single lecturer to "Lecturer.txt" file using lecturer information from a new Course.
void addLecturerFromNewCourse(Lecturer lecturer, CourseInfo *courseInfo) {
	ofstream out("Database/Lecturer.txt", ios::app);
	out << lecturer.username << "\n" 
		<< lecturer.name << "\n" 
		<< lecturer.title << "\n" 
		<< lecturer.gender << "\n"
		<< 1 << "\n"
		<< courseInfo->academicYear << " " << courseInfo->academicYear + 1 << " " 
		<< courseInfo->semester << " " 
		<< courseInfo->courseName << " "
		<< courseInfo->defaultClass << "\n\n";
	out.close();
}

// Add a single lecturer to "Lecturer.txt" file with no course.
void addLecturer(Lecturer*& lecturer) {
	ofstream out("Database/Lecturer.txt", ios::app);
	out << lecturer->username << "\n"
		<< lecturer->name << "\n"
		<< lecturer->title << "\n"
		<< lecturer->gender << "\n"
		<< 0 << "\n\n";
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
bool findLecturerFromUsername(string username, Lecturer& lecturer) {
	ifstream in("Database/Lecturer.txt");
	string user, name, title;
	int gender;
	while (in >> user) {
		getline(in, name, '\n');
		getline(in, title, '\n');
		in >> gender;
		if (user == username) {
			lecturer.username = user;
			lecturer.name = name;
			lecturer.title = title;
			lecturer.gender = gender;
			return true;
		}
		in.get();
	}
	return false;
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

// Print a table displaying all students in a student list.
void printStudentListTable(Student*& students) {
	Student* currentStudent = students;
	int count = 0;
	cout << "\t" << setw(5) << "No. |" << setw(15) << "Student ID |" << setw(30) << "Full name |"
		<< setw(15) << "DOB |" << " Gender\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(15) << "+" << setw(30) << "+"
		<< setw(15) << "+" << setw(15) << " " << "\n";
	while (currentStudent != nullptr) {
		++count;
		string dob = dateToString(currentStudent->dob);
		cout << "\t" << setfill(' ') << setw(4) << count << "|" 
			<< setw(14) << currentStudent->studentId << "|"
			<< setw(29) << currentStudent->name << "|"
			<< setw(14) << dob << "| ";
		if (currentStudent->gender == MALE)
			cout << "male\n";
		else cout << "female\n";
		currentStudent = currentStudent->next;
	}
	cout << "\n";
}

// Print a single lecturer's info.
void printLecturerInfo(Lecturer*& lecturer) {
	cout << "Lecturer information:\n";
	cout << "\tFull name: " << lecturer->name<< "\n";
	cout << "\tUsername: " << lecturer->username << "\n";
	cout << "\tPassword: " << lecturer->password << "\n";
	cout << "\tTitle: " << lecturer->title << "\n";
	cout << "\tGender: ";
	if (lecturer->gender == MALE) cout << "male\n";
	else cout << "female\n";
	cout << "\tNumber of course: " << lecturer->totalCourse << "\n";
	if (lecturer->totalCourse) {
		cout << "\t" << setw(20) << "Academic year |" << setw(20) << "Semester |" 
			<< setw(20) << "Course ID |" <<" Default class\n";
		cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+" 
			<< setw(20) << "+" << setw(20) << " " << "\n";
		CourseInfo* currentCourse = lecturer->myCourse;
		while (currentCourse != nullptr) {
			string year = to_string(currentCourse->academicYear) + "-" 
				+ to_string(currentCourse->academicYear + 1);
			cout << "\t" << setfill(' ') << setw(19) << year << "|"
				<< setw(19) << currentCourse->semester << "|" 
				<< setw(19) << currentCourse->courseName << "| " 
				<< currentCourse->defaultClass << "\n";
			currentCourse = currentCourse->next;
		}
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
		int year = stoi(academicYear.substr(0, 4));
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
			currentCourseList->academicYear = year;
			currentCourseList->semester = semester;
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
			out << courseList->courseName << " " << courseList->defaultClass << "\n";
			courseList = courseList->next;
		}
		out.close();
	}
}

// Add a new directory for academic year and add to AcademicYears.txt
void addAcademicYear(int academicYear) {
	if (isAcademicYearExist(academicYear)) {
		cout << "Error: Academic year already exists.\n\n";
		return;
	}
	ofstream out("Database/AcademicYears.txt", ios::app);
	out << academicYear << " " << academicYear + 1 << "\n0\n\n";
	out.close();
	string filepath = "Database\\\\" + to_string(academicYear)
		+ "-" + to_string(academicYear + 1);
	string command = "md " + filepath;
	system(command.c_str());
	cout << "Adding academic year successful.\n\n";
}

// Add a new directory for semester of an academic year.
void addSemester(int academicYear, string mySemester) {
	if (isSemesterExist(academicYear, mySemester)) {
		cout << "Error: Semester already exists.\n\n";
		return;
	}
	AcademicYear* academicYears = nullptr;
	readAcademicYearsFromFile(academicYears);
	AcademicYear* currentYear = academicYears;
	while (currentYear != nullptr) {
		if (currentYear->academicYear == academicYear) {
			currentYear->numberOfSemester++;
			currentYear->semester += mySemester + ',';
			break;
		}
		currentYear = currentYear->next;
	}
	writeAcademicYearsToFile(academicYears);
	deleteAcademicYears(academicYears);

	string filepath = "Database\\\\" + to_string(academicYear)
		+ "-" + to_string(academicYear + 1) + "\\\\" + mySemester;
	string command = "md " + filepath;
	system(command.c_str());

	cout << "Adding semester successful.\n\n";
}

// Read all academic years in AcademicYears.txt.
// The semester string in this struct is all semester names separated by comma.
void readAcademicYearsFromFile(AcademicYear*& academicYears) {
	ifstream in("Database/AcademicYears.txt");
	if (in.is_open()) {
		int year, numberOfSemester;
		string semester;
		AcademicYear* currentYear = academicYears;
		while (in >> year) {
			in >> year >> numberOfSemester;
			if (academicYears == nullptr) {
				academicYears = new AcademicYear;
				currentYear = academicYears;
			}
			else {
				currentYear->next = new AcademicYear;
				currentYear = currentYear->next;
			}
			currentYear->academicYear = year - 1;
			currentYear->numberOfSemester = numberOfSemester;
			currentYear->semester = "";
			for (int i = 0; i < numberOfSemester; ++i) {
				in >> semester;
				currentYear->semester += semester + ",";
			}
			currentYear->next = nullptr;
		}
	}
}

// Delete AcademicYear linked list.
void deleteAcademicYears(AcademicYear*& academicYears) {
	while (academicYears != nullptr) {
		AcademicYear* temp = academicYears;
		academicYears = academicYears->next;
		delete temp;
	}
}

// Write/rewrite AcademicYears.txt.
void writeAcademicYearsToFile(AcademicYear*& academicYears) {
	ofstream out("Database/AcademicYears.txt");
	AcademicYear* currentYear = academicYears;
	while (currentYear != nullptr) {
		out << currentYear->academicYear << " "
			<< currentYear->academicYear + 1 << "\n";
		out << currentYear->numberOfSemester << "\n";
		stringstream semester(currentYear->semester);
		string semesterName;
		while (getline(semester, semesterName, ','))
			out << semesterName << "\n";
		out << "\n";
		currentYear = currentYear->next;
	}
	out.close();
}

// Delete a semester.
void deleteSemester(int academicYear, string semester) {
	if (!isSemesterExist(academicYear, semester)) {
		cout << "Delete semester failed. Error: Semester does not exist.\n\n";
		return;
	}

	// Delete from AcademicYears.txt.
	cout << "\tDeleting semester " << semester << " in AcademicYears.txt...\n";
	AcademicYear* academicYears = nullptr;
	readAcademicYearsFromFile(academicYears);
	AcademicYear* currentYear = academicYears;
	while (currentYear != nullptr) {
		if (currentYear->academicYear == academicYear) {
			currentYear->numberOfSemester--;
			string semesterName, newSemesters = "";
			stringstream sem(currentYear->semester);
			while (getline(sem, semesterName, ','))
				if (semesterName != semester)
					newSemesters += semesterName + ',';
			currentYear->semester = newSemesters;
			break;
		}
		currentYear = currentYear->next;
	}
	writeAcademicYearsToFile(academicYears);
	deleteAcademicYears(academicYears);

	// Delete directory of that semester in the directory of academic year.
	cout << "\tDeleting semester " << semester << " directory in academic year folder...\n";
	removeSemesterDirectory(academicYear, semester);

	// Delete courses of students that enrolled in any course in this semester.
	cout << "\tDeleting semester's courses of enrolled students...\n";
	ifstream in("Database/Class/Classes.txt");
	if (in.is_open()) {
		string className;
		while (in >> className) {
			Student* studentList = nullptr;
			readClassFromFile(className, studentList);
			Student* currentStudent = studentList;
			while (currentStudent != nullptr) {
				CourseInfo* currentCourse = currentStudent->myCourse, * previousCourse = nullptr;
				while (currentCourse != nullptr) {
					if (currentCourse->academicYear == academicYear && currentCourse->semester == semester) {
						currentStudent->numberOfCourse--;
						CourseInfo* temp = currentCourse;
						currentCourse = currentCourse->next;
						delete temp;
						if (previousCourse != nullptr)
							previousCourse->next = currentCourse;
						else
							currentStudent->myCourse = currentCourse;
					}
					else {
						previousCourse = currentCourse;
						currentCourse = currentCourse->next;
					}
				}
				currentStudent = currentStudent->next;
			}
			writeClassToFile(studentList, className);
			deleteStudentList(studentList);
		}
		in.close();
	}

	// Delete courses of lecturers that teach any course in this semester.
	cout << "\tDeleting semester's courses of lecturers...\n";
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer != nullptr) {
		CourseInfo* currentCourse = currentLecturer->myCourse, *previousCourse = nullptr;
		while (currentCourse != nullptr) {
			if (currentCourse->academicYear == academicYear
				&& currentCourse->semester == semester) {
				currentLecturer->totalCourse--;
				CourseInfo* temp = currentCourse;
				currentCourse = currentCourse->next;
				delete temp;
				if (previousCourse != nullptr)
					previousCourse->next = currentCourse;
				else
					currentLecturer->myCourse = currentCourse;
			}
			else {
				previousCourse = currentCourse;
				currentCourse = currentCourse->next;
			}
		}
		currentLecturer = currentLecturer->next;
	}
	writeLecturersToFile(lecturers);
	deleteLecturers(lecturers);

	cout << "Delete semester " << semester << " successful.\n\n";
}

// Read all lecturers from Lecturer.txt.
void readLecturersFromFile(Lecturer*& lecturers) {
	ifstream in("Database/Lecturer.txt");
	if (in.is_open()) {
		string username, fullName, title, semester, courseId, defaultClass;
		int gender, totalCourse, year;
		Lecturer* currentLecturer = lecturers;
		while (in >> username) {
			in.ignore();
			getline(in, fullName);
			getline(in, title);
			in >> gender >> totalCourse;
			if (lecturers == nullptr) {
				lecturers = new Lecturer;
				currentLecturer = lecturers;
			}
			else {
				currentLecturer->next = new Lecturer;
				currentLecturer = currentLecturer->next;
			}
			currentLecturer->username = username;
			currentLecturer->name = fullName;
			currentLecturer->title = title;
			currentLecturer->gender = gender;
			currentLecturer->totalCourse = totalCourse;
			currentLecturer->myCourse = nullptr;
			CourseInfo* currentCourse = currentLecturer->myCourse;
			for (int i = 0; i < totalCourse; ++i) {
				in >> year >> year >> semester >> courseId >> defaultClass;
				if (currentLecturer->myCourse == nullptr) {
					currentLecturer->myCourse = new CourseInfo;
					currentCourse = currentLecturer->myCourse;
				}
				currentCourse->academicYear = year - 1;
				currentCourse->semester = semester;
				currentCourse->courseName = courseId;
				currentCourse->defaultClass = defaultClass;
				currentCourse->next = nullptr;
			}
			currentLecturer->next = nullptr;
		}
		in.close();
	}
}

// Write/rewrite all lecturers.
void writeLecturersToFile(Lecturer*& lecturers) {
	ofstream out("Database/Lecturer.txt");
	Lecturer* currentLecturer = lecturers;
	while (currentLecturer != nullptr) {
		out << currentLecturer->username << "\n"
			<< currentLecturer->name << "\n"
			<< currentLecturer->title << "\n"
			<< currentLecturer->gender << "\n"
			<< currentLecturer->totalCourse << "\n";
		CourseInfo* currentCourse = currentLecturer->myCourse;
		while (currentCourse != nullptr) {
			out << currentCourse->academicYear << " "
				<< currentCourse->academicYear + 1 << " "
				<< currentCourse->semester << " "
				<< currentCourse->courseName << " "
				<< currentCourse->defaultClass << "\n";
			currentCourse = currentCourse->next;
		}
		out << "\n";
		currentLecturer = currentLecturer->next;
	}
	out.close();
}

// Delete Lecturer linked list.
void deleteLecturers(Lecturer*& lecturers) {
	while (lecturers != nullptr) {
		deleteCourseInfo(lecturers->myCourse);
		Lecturer* temp = lecturers;
		lecturers = lecturers->next;
		delete temp;
	}
}

// Delete an academic year.
void deleteAcademicYear(int academicYear) {
	if (!isAcademicYearExist(academicYear)) {
		cout << "Delete academic year failed. Error: Academic year does not exist.\n\n";
		return;
	}

	// Delete each semester if existent, including deleting from AcademicYears.txt,
	// from courses of enrolled students, from lecturers and deleting the directory.
	if (isSemesterExist(academicYear, "Summer"))
		deleteSemester(academicYear, "Summer");
	if (isSemesterExist(academicYear, "Spring"))
		deleteSemester(academicYear, "Spring");
	if (isSemesterExist(academicYear, "Fall"))
		deleteSemester(academicYear, "Fall");

	// Delete AcademicYear in AcademicYears.txt.
	cout << "\tDeleting academic year in AcademicYears.txt...\n";
	AcademicYear* academicYears = nullptr;
	readAcademicYearsFromFile(academicYears);
	AcademicYear* currentYear = academicYears,* previousYear = nullptr;
	while (currentYear != nullptr) {
		if (currentYear->academicYear == academicYear) {
			AcademicYear* temp = currentYear;
			currentYear = currentYear->next;
			delete temp;
			if (previousYear == nullptr)
				academicYears = currentYear;
			else
				previousYear->next = currentYear;
		}
		else {
			previousYear = currentYear;
			currentYear = currentYear->next;
		}
	}
	writeAcademicYearsToFile(academicYears);
	deleteAcademicYears(academicYears);

	// Delete directory of academic year.
	cout << "\tDeleting directory of academicYear " << academicYear << "-" << academicYear + 1 << "...\n";
	string filepath = "Database\\\\" + to_string(academicYear) + "-"
		+ to_string(academicYear + 1);
	string command = "rd " + filepath;
	system(command.c_str());

	cout << "\nDelete academic year successful.\n\n";
}

// Register a course for all students in a student list (including edit linked list and edit course file.
// If that student already has the same course (same AY, semester, courseID and default class)
// the course won't be added.
void registerCourseForStudentList(Student*& students, CourseInfo*& courseInfo) {
	ofstream out("Database/" + to_string(courseInfo->academicYear) + "-"
		+ to_string(courseInfo->academicYear + 1) + "/"
		+ courseInfo->semester + "/" + courseInfo->courseName + "-"
		+ courseInfo->defaultClass + ".txt", ios::app);
	Attendance* attendance = nullptr;
	findAttendanceDateOfCourse(attendance, courseInfo);
	Student* currentStudent = students;
	while (currentStudent != nullptr) {
		CourseInfo* currentCourse = currentStudent->myCourse;
		bool haveCourse = false;
		if (currentCourse == nullptr) {
			currentStudent->myCourse = new CourseInfo;
			currentCourse = currentStudent->myCourse;
		}
		else {
			while (currentCourse->next != nullptr) {
				if (currentCourse->academicYear == courseInfo->academicYear
					&& currentCourse->semester == courseInfo->semester
					&& currentCourse->courseName == courseInfo->courseName
					&& currentCourse->defaultClass == courseInfo->defaultClass) {
					currentStudent = currentStudent->next;
					haveCourse = true;
					break;
				}
				currentCourse = currentCourse->next;
			}
			if (currentCourse->next == nullptr) {
				currentCourse->next = new CourseInfo;
				currentCourse = currentCourse->next;
			}
		}
		if (!haveCourse) {
			currentStudent->numberOfCourse++;
			currentCourse->academicYear = courseInfo->academicYear;
			currentCourse->semester = courseInfo->semester;
			currentCourse->courseName = courseInfo->courseName;
			currentCourse->defaultClass = courseInfo->defaultClass;
			currentCourse->next = nullptr;
			// Appending to course file.
			out << currentStudent->username << "\n" << currentStudent->name << "\n"
				<< currentStudent->studentId << "\n" << currentStudent->gender << "\n"
				<< currentStudent->dob.day << " " << currentStudent->dob.month << " " << currentStudent->dob.year << "\n"
				<< 1 << "\n" << "0 0 0 0" << "\n";
			Attendance* currentDate = attendance;
			while (currentDate != nullptr) {
				out << currentDate->date.day << " " << currentDate->date.month << " "
					<< currentDate->date.year << " "
					<< currentDate->startTime.hour << " " << currentDate->startTime.minute << " "
					<< currentDate->endTime.hour << " " << currentDate->endTime.minute << " 0 0\n";
				currentDate = currentDate->next;
			}
			out << "\n";
		}
		currentStudent = currentStudent->next;
	}
	deleteAttendance(attendance);
	out.close();
}

// Find all attendance dates (sessions) of a course.
void findAttendanceDateOfCourse(Attendance*& attendanceDate, CourseInfo*& courseInfo) {
	ifstream in("Database/" + to_string(courseInfo->academicYear) + "-"
		+ to_string(courseInfo->academicYear + 1) + "/"
		+ courseInfo->semester + "/" + courseInfo->courseName + "-"
		+ courseInfo->defaultClass + ".txt");
	if (in.is_open()) {
		cout << "\tFinding attendance date of course...\n";
		string waste;
		int totalSessions;
		Attendance* currentAttendance = nullptr;
		for (int i = 0; i < 6; ++i) getline(in, waste); // 6 lines of irrelevant information.
		in >> totalSessions;
		for (int i = 0; i < 13; ++i) getline(in, waste); // 13 lines of irrelevant information.
		for (int i = 0, day, month, year, hour, minute; i < totalSessions; ++i) {
			in >> day >> month >> year >> hour >> minute;
			if (currentAttendance == nullptr) {
				attendanceDate = new Attendance;
				currentAttendance = attendanceDate;
			}
			else {
				currentAttendance->next = new Attendance;
				currentAttendance = currentAttendance->next;
			}
			currentAttendance->date = Date{ day, month, year };
			currentAttendance->startTime = Time{ hour, minute };
			in >> hour >> minute;
			currentAttendance->endTime = Time{ hour, minute };
			currentAttendance->next = nullptr;
			in >> hour >> minute;
		}
		in.close();
	}
}

// Show menu of staff, lecturer and student.
void showStaffMenu() {
	int choice;
	int chon;
	cout << "\t1.Class" << endl;
	cout << "\t2.Course" << endl;
	cout << "\t3.Scoreboard" << endl;
	cout << "\tAttendance list" << endl;
	cout << "Which object do you want to work with ?(please enter a number)" << endl;
	cin >> choice;
	if (choice == 1) {
		cout << "1. Import students of a class from a csv file" << endl;
		cout << "2. Manually add a new student to a class." << endl;
		cout << "3. Edit an existing student." << endl;
		cout << "4. Remove a student." << endl;
		cout << "5. Change students from class A to class B" << endl;
		cout << "6. View list of classes." << endl;
		cout << "7. View list of students in a class." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		default:
			break;
		}
	}
	else if (choice == 2) {
		cout << "1. Create / update / delete / view academic years and semesters" << endl;
		cout << "2. From a semester, import courses from a csv file." << endl;
		cout << "3. Manually add a new course." << endl;
		cout << "4. Edit an existing course." << endl;
		cout << "5. Remove a course." << endl;
		cout << "6. Remove a specific student from a course." << endl;
		cout << "7. Add a specific student to a course." << endl;
		cout << "8. View list of courses in the current semester." << endl;
		cout << "9. View list of students of a course." << endl;
		cout << "10. View attendance list of a course." << endl;
		cout << "11. Create / update / delete / view all lecturers." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		default:
			break;
		}
	}
	else if (choice == 3) {
		cout << "1. Search and view the scoreboard of a course." << endl;
		cout << "2. Export a scoreboard to a csv file." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		default:
			break;
		}
	}
	else if (choice == 4) {
		cout << "1.Search and view attendance list of a course." << endl;
		cout << "2. Export a attendance list to a csv file." << endl;
		cout << "Which function do you want to perform ? (please enter a number)" << endl;
		cin >> chon;
		switch (chon)
		{
		case 1:
		case 2:
		default:
			break;
		}
	}
	else cout << "Your input is not valid" << endl;
}
void showLecturerMenu() {
	int choice;
	cout << "1. View list of courses in the current semester." << endl;
	cout << "2. View list of students of a course." << endl;
	cout << "3. View attendance list of a course." << endl;
	cout << "4. Edit an attendance." << endl;
	cout << "5. Import scoreboard of a course (midterm, final, lab, bonus) from a csv file." << endl;
	cout << "6. Edit grade of a student." << endl;
	cout << "7. View a scoreboard." << endl;
	cout << "Which function do you want to perform ? (please enter a number)" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	default:
		break;
	}
}
void showStudentMenu() {
	int choice;
	cout << "1. Check-in." << endl;
	cout << "2. View check-in result." << endl;
	cout << "3. View schedules." << endl;
	cout << "4. View my scores of a course." << endl;
	cout << "Which function do you want to perform ? (please enter a number)" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	default:
		break;
	}
}

// Read "<class>.txt, find whether Student is in this class, if yes store the info.
bool findStudentInfoFromFile(Student& newTurn, string userName) {
	ifstream in;
	in.open("Database/Class/" + newTurn.myClass + ".txt");
	while (in >> newTurn.username) {
		in >> newTurn.status;
		in.ignore();
		getline(in, newTurn.name);
		in >> newTurn.studentId >> newTurn.gender >> newTurn.dob.day >> newTurn.dob.month >> newTurn.dob.year >> newTurn.numberOfCourse;
		if (newTurn.numberOfCourse != 0)
		{
			newTurn.myCourse = nullptr;
			CourseInfo* currentCourse = 0;
			for (int i = 0; i < newTurn.numberOfCourse; ++i) {
				if (newTurn.myCourse == nullptr) {
					newTurn.myCourse = new CourseInfo;
					in >> newTurn.myCourse->academicYear;
					in >> newTurn.myCourse->academicYear;
					in >> newTurn.myCourse->semester >> newTurn.myCourse->courseName >> newTurn.myCourse->defaultClass;
					newTurn.myCourse->next = nullptr;
					currentCourse = newTurn.myCourse;
				}
				else {
					currentCourse->next = new CourseInfo;
					currentCourse = currentCourse->next;
					in >> currentCourse->academicYear;
					in >> currentCourse->academicYear;
					in >> currentCourse->semester >> currentCourse->courseName >> currentCourse->defaultClass;
					currentCourse->next = nullptr;
				}

			}
		}
		if (newTurn.username == userName) {
			in.close();
			return true;
		}

	}
	in.close();
	return false;
}
