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
	int start = getDayOfWeek(course->startDate), end = getDayOfWeek(course->endDate);
	int countStart = 0, countEnd = 0;
	SessionInfo* currentSession = course->sessionInfo;
	while (currentSession != nullptr) {
		if (currentSession->day == start)
			break;
		countStart++;
		currentSession = currentSession->next;
	}
	currentSession = course->sessionInfo;
	while (currentSession != nullptr) {
		if (currentSession->day > end)
			break;
		countEnd++;
		currentSession = currentSession->next;
	}

	int totalSessions = 0;
	totalSessions += course->sessionsPerWeek - countStart;
	int days = calculateDaysBetDates(course->startDate, course->endDate) + 1;
	if (start <= end)
		totalSessions += (days / 7 - 1) * course->sessionsPerWeek;
	else
		totalSessions += days / 7 * course->sessionsPerWeek;
	totalSessions += countEnd;
	return totalSessions;
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

// Check if a course info is in a course list.
bool isCourseInCourseList(int academicYear, string semester, string courseId, CourseInfo*& courseList) {
	CourseInfo* currentCourse = courseList;
	while (currentCourse != nullptr) {
		if (currentCourse->academicYear == academicYear 
			&& currentCourse->semester == semester 
			&& currentCourse->courseName == courseId)
			return true;
		currentCourse = currentCourse->next;
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
		string command = "rd " + semesterDir;
		system(command.c_str());
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

// Clear screen and restart.
void clearScreen() {
	cin.clear();
	cout << "Press any key to continue.\n";
	getch();
	system("cls");
	cout << "\t\t\t===============BUNTAI SCHOOL MANAGEMENT=============\n\n";
}

// Clear screen without press any key.
void clearScreenWithoutPress() {
	system("cls");
	cout << "\t\t\t===============BUNTAI SCHOOL MANAGEMENT=============\n\n";
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
	if (in.is_open()) {
		string user, name, title, temp;
		int gender, totalCourse;
		while (in >> user) {
			in.ignore();
			getline(in, name);
			getline(in, title);
			in >> gender >> totalCourse;
			in.ignore();
			for (int i = 0; i < totalCourse; ++i)
				getline(in, temp);
			if (user == username) {
				lecturer.username = user;
				lecturer.name = name;
				lecturer.title = title;
				lecturer.gender = gender;
				return true;
			}
		}
		return false;
		in.close();
	}
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
	cout << "\tDate of birth: " << dateToString(student->dob) << "\n";
	cout << "\tCourses: " << student->numberOfCourse << "\n";
	if (student->numberOfCourse) {
		cout << "\t" << setw(20) << "Academic year |" << setw(20) << "Semester |"
			<< setw(20) << "Course ID |" << " Default class\n";
		cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+"
			<< setw(20) << "+" << setw(20) << " " << "\n";
		CourseInfo* currentCourse = student->myCourse;
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

// Print a table displaying scoreboard of a course.
void printScoreboardTable(Course* course) {
	Student* curStudent = course->students;
	StudentCourseInfo* curStudentCourseInfo = course->studentCourseInfo;
	cout << "\t" << setw(5) << "No. |" << setw(15) << "Student ID |" << setw(30) << "Full name |"
		<< setw(10) << "Midterm |"<< setw(10) << "Final |" << setw(10) << "Lab |"<< setw(10) << "Bonus\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(15) << "+" << setw(30) << "+"
		<< setw(10) << "+" << setw(10) << "+" << setw(10) << "+" << setw(10) << " " << "\n";
	int count = 0;
	while (curStudent != nullptr) {
		++count;
		cout << "\t" << setfill(' ') << setw(4) << count << "|"
			<< setw(14) << curStudent->studentId << "|"
			<< setw(29) << curStudent->name << "|"
			<< setw(9) << curStudentCourseInfo->midterm << "|"
			<< setw(9) << curStudentCourseInfo->final << "|"
			<< setw(9) << curStudentCourseInfo->lab << "|"
			<< setw(9) << curStudentCourseInfo->bonus << "\n";
		curStudent = curStudent->next;
		curStudentCourseInfo = curStudentCourseInfo->next;
	}
	cout << "\n";
}

// Print a table displaying scoreboard of a single student.
void printScoreboardOfStudent(StudentCourseInfo* student) {
	cout << "\t" << setw(5) << "No.|" << setw(15) << "Score type |" << setw(10) << "Score\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(15) << "+" << setw(10) << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 1 << "|" << setw(15) << "Midterm |" << setw(9) << student->midterm << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 2 << "|" << setw(15) << "  Final |" << setw(9) << student->final << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 3 << "|" << setw(15) << "    Lab |" << setw(9) << student->lab << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 4 << "|" << setw(15) << "  Bonus |" << setw(9) << student->bonus << "\n\n";
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

// Print all lecturers' information into a table and return the total number of lecturers.
int printAllLecturersTable(Lecturer*& lecturers) {
	cout << "\t\t\t\t\t  ALL LECTURERS\n\n";
	cout << "\t" << setw(5) << "No |" << setw(25) << "Full name |" << setw(15) << "Account |"
		<< setw(10) << "Title |" << " Number of courses\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(25) << "+" << setw(15) << "+"
		<< setw(10) << "+" << setw(20) << " " << "\n";
	Lecturer* currentLecturer = lecturers;
	int count = 0;
	while (currentLecturer != nullptr) {
		cout << "\t" << setfill(' ') << setw(4) << ++count << "|"
			<< setw(24) << currentLecturer->name << "|"
			<< setw(14) << currentLecturer->username << "|"
			<< setw(9) << currentLecturer->title << "| "
			<< "\t" << currentLecturer->totalCourse << "\n";
		currentLecturer = currentLecturer->next;
	}
	cout << "\n";
	return count;
}

// Print a course list into a table.
int printCourseListTable(CourseInfo* courseList) {
	if (courseList == nullptr) {
		cout << "Sorry there's no courses to view.\n\n";
		return 0;
	}
	cout << "\t" << setw(5) << "No |" << setw(20) << "Academic year |" << setw(20) << "Semester |"
		<< setw(20) << "Course ID |" << " Default class\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(20) << "+" << setw(20) << "+"
		<< setw(20) << "+" << setw(20) << " " << "\n";
	int count = 0;
	CourseInfo* currentCourse = courseList;
	while (currentCourse != nullptr) {
		string year = to_string(currentCourse->academicYear) + "-"
			+ to_string(currentCourse->academicYear + 1);
		cout << "\t" << setfill(' ') << setw(4) << ++count << "|" 
			<< setw(19) << year << "|"
			<< setw(19) << currentCourse->semester << "|"
			<< setw(19) << currentCourse->courseName << "| "
			<< currentCourse->defaultClass << "\n";
		currentCourse = currentCourse->next;
	}
	cout << "\n";
	return count;
}

// Print all sessions info of a course into a table.
void printAllSessionsTable(Attendance*& attendanceDate) {
	cout << "\t" << setw(5) << "No. |" << setw(20) << "Date |"
		<< setw(18) << "Study time\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(20) << "+"
		<< setw(25) << " " << "\n";
	int count = 0;
	Attendance* currentAttendance = attendanceDate;
	while (currentAttendance != nullptr) {
		string date = numToDay(getDayOfWeek(currentAttendance->date)) + ", " + dateToString(currentAttendance->date);
		string sessionTime = "\t" + timeToString(currentAttendance->startTime) 
			+ "-" + timeToString(currentAttendance->endTime);
		cout << "\t" << setfill(' ') << setw(4) << ++count << "|"
			<< setw(19) << date << "|"
			<< sessionTime << "\n";
		currentAttendance = currentAttendance->next;
	}
	cout << "\n";
}

// Print an attendance list in a table.
void printAttendanceListOfCourse(Course* course) {
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	Attendance* currentAttendance;
	cout << "\t" << setw(20) << "STUDENT NAME |";
	int* studentCount = new int[course->totalSessions];
	for (int i = 0; i < course->totalSessions - 1; ++i) {
		studentCount[i] = 0;
		string session = " S" + to_string(i + 1) + " |";
		cout << setw(6) << session;
	}
	studentCount[course->totalSessions - 1] = 0;
	cout << " S" + to_string(course->totalSessions) + "\n";
	cout << "\t" << setfill('-') << setw(20);
	for (int i = 0; i < course->totalSessions; ++i)
		cout << "+" << setw(6);
	cout << "\n";
	Student* currentStudent = course->students;
	while (currentStudent != nullptr) {
		string name = currentStudent->name + " |";
		cout << "\t" << setfill(' ') << setw(20) << name;
		currentAttendance = currentStudentInfo->attendance;
		for (int i = 0; i < course->totalSessions - 1; ++i) {
			string time = timeToString(currentAttendance->time) + "|";
			cout << setw(6) << time;
			if (isPresent(currentAttendance))
				studentCount[i]++;
			currentAttendance = currentAttendance->next;
		}
		cout << timeToString(currentAttendance->time) << "\n";
		if (isPresent(currentAttendance))
			studentCount[course->totalSessions - 1]++;
		cout << "\t" << setfill('-') << setw(20);
		for (int i = 0; i < course->totalSessions; ++i)
			cout << "+" << setw(6);
		cout << "\n";
		currentStudent = currentStudent->next;
	}
	cout << "\t" << setfill(' ') << setw(20) << "Total |";
	for (int i = 0; i < course->totalSessions - 1; ++i) {
		string total = to_string(studentCount[i]) + " |";
		cout << setw(6) << total;
	}
	cout << " " << studentCount[course->totalSessions - 1] << "\n\n";
	delete[] studentCount;
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
void readCourseListFromFile(CourseInfo*& courseList, int academicYear, string semester) {
	ifstream in;
	in.open("Database/" + to_string (academicYear) +"-"+ to_string (academicYear+1) + "/" + semester + "/" + "Courses.txt");
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
			currentCourseList->academicYear = academicYear;
			currentCourseList->semester = semester;
			currentCourseList->courseName = currentCourseID;
			in >> currentCourseList->defaultClass;
			currentCourseList->next = nullptr;
		}
		in.close();
	}
}

// Read all available courses into a CourseInfo linked list.
void readAllCourseInfo(CourseInfo*& courseList) {
	AcademicYear* academicYears = nullptr;
	readAcademicYearsFromFile(academicYears);
	AcademicYear* currentYear = academicYears;
	string semester;
	while (currentYear != nullptr) {
		stringstream sem(currentYear->semester);
		while (getline(sem, semester, ','))
			readCourseListFromFile(courseList, currentYear->academicYear, semester);
		currentYear = currentYear->next;
	}
	deleteAcademicYears(academicYears);
}

// Write/overwrite a course list into "Courses.txt" file.
void writeCourseListToFile(CourseInfo* courseList, int academicYear, string semester) {
	ofstream out;
	out.open("Database/" + to_string(academicYear)+"-"+ to_string(academicYear+1) + "/" + semester + "/" + "Courses.txt");
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
	AcademicYear* currentYear = academicYears,* previousYear = nullptr;
	while (currentYear != nullptr) {
		if (currentYear->academicYear == academicYear) {
			currentYear->numberOfSemester++;
			currentYear->semester += mySemester + ',';
			break;
		}
		previousYear = currentYear;
		currentYear = currentYear->next;
	}
	if (currentYear == nullptr) {
		if (academicYears == nullptr) {
			academicYears = new AcademicYear;
			currentYear = academicYears;
		}
		else {
			previousYear->next = new AcademicYear;
			currentYear = previousYear->next;
		}
		currentYear->academicYear = academicYear;
		currentYear->numberOfSemester = 1;
		currentYear->semester = mySemester + ',';
		currentYear->next = nullptr;
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

// Change the status of a single student in a course to be 0.
void unregisterCourseForStudent(Student*& student, CourseInfo*& courseInfo) {
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	Student* currentStudent = course->students;
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	while (currentStudent->username != student->username) {
		currentStudent = currentStudent->next;
		currentStudentInfo = currentStudentInfo->next;
	}
	currentStudentInfo->status = 0;
	writeCourseToFile(course);
	deleteCourse(course);
}

// Find all attendance dates (sessions) of a course.
void findAttendanceDateOfCourse(Attendance*& attendanceDate, CourseInfo*& courseInfo) {
	ifstream in("Database/" + to_string(courseInfo->academicYear) + "-"
		+ to_string(courseInfo->academicYear + 1) + "/"
		+ courseInfo->semester + "/" + courseInfo->courseName + "-"
		+ courseInfo->defaultClass + ".txt");
	if (in.is_open()) {
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

// Show menu of staff-class.
void showStaffClassMenu(string& username) {
	int chon;
	cout << "\t1. Import students of a class from a csv file." << endl;
	cout << "\t2. Manually add a new student to a class." << endl;
	cout << "\t3. Edit an existing student." << endl;
	cout << "\t4. Remove a student." << endl;
	cout << "\t5. Change students from class A to class B" << endl;
	cout << "\t6. View list of classes." << endl;
	cout << "\t7. View list of students in a class." << endl;
	cout << "\t8. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> chon;
	clearScreenWithoutPress();
	fflush(stdin);
	switch (chon)
	{
	case 1:
	case 2:
	case 3:
	case 4: removeStudent(); break;
	case 5: changeStudentClass(); break;
	case 6: viewListOfClasses(); break;
	case 7: viewListOfStudentInAClass(); break;
	case 8: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showStaffClassMenu(username);
}

// Show menu of staff-course.
void showStaffCourseMenu(string& username) {
	int chon;
	cout << "\t1. Create / update / delete / view academic years and semesters" << endl;
	cout << "\t2. From a semester, import courses from a csv file." << endl;
	cout << "\t3. Manually add a new course." << endl;
	cout << "\t4. Edit an existing course." << endl;
	cout << "\t5. Remove a course." << endl;
	cout << "\t6. Remove a specific student from a course." << endl;
	cout << "\t7. Add a specific student to a course." << endl;
	cout << "\t8. View list of courses in the current semester." << endl;
	cout << "\t9. View list of students of a course." << endl;
	cout << "\t10. View attendance list of a course." << endl;
	cout << "\t11. Create / update / delete / view all lecturers." << endl;
	cout << "\t12. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> chon;
	clearScreenWithoutPress();
	switch (chon)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5: removeCourse(); break;
	case 6: removeStudentFromCourse(); break;
	case 7: addAStudentToCourse(); break;
	case 8: 
	case 9: viewListOfStudentInAClass(); break;
	case 10:
	case 11:
	case 12: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showStaffCourseMenu(username);
}

// Show menu of staff-scoreboard
void showStaffScoreboardMenu(string& username) {
	int chon;
	cout << "\t1. Search and view the scoreboard of a course." << endl;
	cout << "\t2. Export a scoreboard to a csv file." << endl;
	cout << "\t3. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> chon;
	fflush(stdin);
	clearScreenWithoutPress();
	switch (chon)
	{
	case 1:
	case 2: exportScoreboardToCsv(); break;
	case 3: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showStaffScoreboardMenu(username);
}

// Show menu of staff-attendance 
void showStaffAttendanceMenu(string& username) {
	int chon;
	cout << "\t1. Search and view attendance list of a course." << endl;
	cout << "\t2. Export a attendance list to a csv file." << endl;
	cout << "\t3. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> chon;
	clearScreenWithoutPress();
	switch (chon)
	{
	case 1:
	case 2:
	case 3: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showStaffAttendanceMenu(username);
}

// Show menu of staff
void showStaffMenu(string& username) {
	clearScreenWithoutPress();
	int choice;
	int chon;
	cout << "\t1. Class" << endl;
	cout << "\t2. Course" << endl;
	cout << "\t3. Scoreboard" << endl;
	cout << "\t4. Attendance list" << endl;
	cout << "\t5. Return to previous menu" << endl << endl;
	cout << "Which object do you want to work with ? Please enter a number: ";
	cin >> choice;
	clearScreenWithoutPress();
	if (choice == 1) {
		showStaffClassMenu(username);
	}
	else if (choice == 2) {
		showStaffCourseMenu(username);
	}
	else if (choice == 3) {
		showStaffScoreboardMenu(username);
	}
	else if (choice == 4) {
		showStaffAttendanceMenu(username);
	}
	else if (choice == 5) return;
	else cout << "Your input is not valid" << endl;
	clearScreenWithoutPress();
	showStaffMenu(username);
}

// Show lecturer menu.
void showLecturerMenu(string& username) {
	int choice;
	cout << "\t1. View list of courses in the current semester." << endl;
	cout << "\t2. View list of students of a course." << endl;
	cout << "\t3. View attendance list of a course." << endl;
	cout << "\t4. Edit an attendance." << endl;
	cout << "\t5. Import scoreboard of a course (midterm, final, lab, bonus) from a csv file." << endl;
	cout << "\t6. Edit grade of a student." << endl;
	cout << "\t7. View a scoreboard." << endl;
	cout << "\t8. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> choice;
	clearScreenWithoutPress();
	switch (choice)
	{
	case 1: viewCoursesInSemester(username); break;
	case 2: viewStudentListOfCourse(username); break;
	case 3: viewAttendanceListOfCourseByLecturer(username); break;
	case 4: editAnAttendanceByLecturer(username); break;
	case 5: importScoreboardFromCsv(username); break;
	case 6: editGradeOfStudent(username); break;
	case 7: viewScoreboardOfCourse(username); break;
	case 8: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showLecturerMenu(username);
}

// Show student menu.
void showStudentMenu(string& username) {
	int choice;
	cout << "\t1. Check-in." << endl;
	cout << "\t2. View check-in result." << endl;
	cout << "\t3. View schedules." << endl;
	cout << "\t4. View my scores of a course." << endl;
	cout << "\t5. Return to previous menu." << endl << endl;
	cout << "Which function do you want to perform ? Please enter a number: ";
	cin >> choice;
	clearScreenWithoutPress();
	switch (choice)
	{
	case 1: checkin(username); break;
	case 2: viewCheckinResult(username); break;
	case 3: viewSchedules(username); break;
	case 4: viewScoresOfACourse(username); break;
	case 5: return;
	default:
		cout << "Wrong option number!\n";
		break;
	}
	clearScreen();
	showStudentMenu(username);
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

// Find class of a student from student ID.
string findClassFromStudentId(string& studentId) {
	ifstream in("Database/Class/Classes.txt");
	if (in.is_open()) {
		string className;
		while (in >> className) {
			Student* students = nullptr;
			readClassFromFile(className, students);
			Student* currentStudent = students;
			while (currentStudent != nullptr && currentStudent->studentId != studentId)
				currentStudent = currentStudent->next;
			if (currentStudent != nullptr) {
				deleteStudentList(students);
				in.close();
				return className;
			}
			deleteStudentList(students);
		}
	}
}

// Print a single course's info.
void printCourseInfo(Course* course) {
	cout << "Course info:\n";
	cout << "\tAcademic year: " << course->academicYear << "\n";
	cout << "\tSemester: " << course->semester << "\n";
	cout << "\tCourse ID: " << course->courseId << "\n";
	cout << "\tCourse name: " << course->courseName << "\n";
	cout << "\tDefault class: " << course->defaultClass << "\n";
	cout << "\tLecturer account: " << course->lecturer.title << " " << course->lecturer.name << "\n";
	cout << "\tStart date: " << course->startDate.day << "-" << course->startDate.month << "-" << course->startDate.year << "\n";
	cout << "\tEnd date: " << course->endDate.day << "-" << course->endDate.month << "-" << course->endDate.year << "\n";
	cout << "\tSessions per week: " << course->sessionsPerWeek << "\n";
	cout << "\tSessions info:\n";
	cout << "\t" << setw(20) << "Day of week |" << setw(20) << "Start time |" << " End time\n";
	cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+" << setw(20) << " " << "\n";
	SessionInfo* currentSession = course->sessionInfo;
	for (int i = 0; i < course->sessionsPerWeek; ++i) {
		string sessionStart = to_string(currentSession->startTime.hour) + ":"
			+ to_string(currentSession->startTime.minute);
		string sessionEnd = to_string(currentSession->endTime.hour) + ":"
			+ to_string(currentSession->endTime.minute);
		cout << "\t" << setfill(' ') << setw(19) << numToDay(currentSession->day) << "|"
			<< setw(19) << sessionStart << "| " << sessionEnd << "\n";
		currentSession = currentSession->next;
	}
	cout << "\tRoom: " << course->room << "\n";
	cout << "\n";
}

// Check whether given course is in lecturer's courses.
bool isLecturerCourse(CourseInfo* courseInfo, string lecturerUsername) {
	Lecturer* lecturers = nullptr;
	readLecturersFromFile(lecturers);
	Lecturer* curLecturer = lecturers;
	while (curLecturer->username != lecturerUsername) {
		curLecturer = curLecturer->next;
	}
	if (curLecturer->totalCourse) {
		CourseInfo* curCourse = curLecturer->myCourse;
		while (curCourse != nullptr) {
			if (curCourse->academicYear == courseInfo->academicYear
				&& curCourse->semester == courseInfo->semester
				&& curCourse->courseName == courseInfo->courseName
				&& curCourse->defaultClass == courseInfo->defaultClass) {
				deleteLecturers(lecturers);
				return true;
			}
		}
	}
	deleteLecturers(lecturers);
	return false;
}

// Turn a time into a string.
string timeToString(Time time) {
	string timeString = "";
	if (time.hour < 10) timeString += "0";
	timeString += to_string(time.hour) + ":";
	if (time.minute < 10) timeString += "0";
	timeString += to_string(time.minute);
	return timeString;
}

// Compare check-in time vs scheduled time.
bool isPresent(Attendance* attendance) {
	if ((attendance->time.hour < attendance->startTime.hour) ||
		(attendance->time.hour == attendance->startTime.hour &&
			attendance->time.minute < attendance->startTime.minute))
		return false;
	else if ((attendance->time.hour > attendance->endTime.hour) ||
		(attendance->time.hour == attendance->endTime.hour &&
			attendance->time.minute > attendance->endTime.minute))
		return false;
	else
		return true;
}

// Find whether a student ID exists, if yes store the info.
bool findStudentInfoFromId(Student& student, string studentId) {
	ifstream in("Database/Class/Classes.txt");
	string className;
	while (in >> className) {
		Student* studentList = nullptr;
		readClassFromFile(className, studentList);
		Student* currentStudent = studentList;
		while (currentStudent != nullptr) {
			if (currentStudent->studentId == studentId) {
				student.name = currentStudent->name;
				student.studentId = studentId;
				student.numberOfCourse = currentStudent->numberOfCourse;
				student.myCourse = nullptr;
				CourseInfo* currentInfo = currentStudent->myCourse;
				for (int i = 0; i < currentStudent->numberOfCourse; ++i) {
					if (student.myCourse == nullptr) {
						student.myCourse = new CourseInfo;
						currentInfo = student.myCourse;
					}
					else {
						currentInfo->next = new CourseInfo;
						currentInfo = currentInfo->next;
					}
					currentInfo->academicYear = currentStudent->myCourse->academicYear;
					currentInfo->semester = currentStudent->myCourse->semester;
					currentInfo->courseName = currentStudent->myCourse->courseName;
					currentInfo->defaultClass = currentStudent->myCourse->defaultClass;
					currentInfo->next = nullptr;
					currentStudent->myCourse = currentStudent->myCourse->next;
				}
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

// Read attendance list from a course info.
void readAttendanceList(Attendance*& attendance, CourseInfo* courseInfo, Student student) {
	// Read student list from file.
	Course* course = new Course;
	readCourseFromFile(courseInfo, course);
	
	// Find student course info and read attendance list.
	Student* currentStudent = course->students;
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	while (currentStudent != nullptr) {
		if (currentStudent->studentId == student.studentId) {
			Attendance* currentAttendance = currentStudentInfo->attendance, * currentList = attendance;
			while (currentAttendance != nullptr) {
				if (currentList == nullptr) {
					attendance = new Attendance;
					currentList = attendance;
				}
				else {
					currentList->next = new Attendance;
					currentList = currentList->next;
				}
				currentList->date = currentAttendance->date;
				currentList->startTime = currentAttendance->startTime;
				currentList->endTime = currentAttendance->endTime;
				currentList->time = currentAttendance->time;
				currentList->next = nullptr;
				currentAttendance = currentAttendance->next;
			}
			break;
		}
		currentStudent = currentStudent->next;
		currentStudentInfo = currentStudentInfo->next;
	}

	// Delete linked list.
	deleteCourse(course);
}

// Get info of student from file.
void getInfoOfStudent(Student& newTurn, string studentUsername) {
	ifstream in;
	int count = 0;
	in.open("Database/Class/Classes.txt");
	count++;
	if (!in) cout << "Cannot open class file, please try it later" << endl;
	else (in >> newTurn.myClass);
	in.close();
	while (findStudentInfoFromFile(newTurn, studentUsername) == false) {
		ifstream in;
		in.open("Database/Class/Classes.txt");
		while (in) {
			for (int i = 0; i < count; i++) in >> newTurn.myClass;
		}
		in.close();
	}
}

// Print attendance list of course in a table with student ID.
void printAttendanceListWithId(Course* course) {
	StudentCourseInfo* currentStudentInfo = course->studentCourseInfo;
	Attendance* currentAttendance;
	cout << "\t" << setw(20) << "STUDENT ID |";
	int* studentCount = new int[course->totalSessions];
	for (int i = 0; i < course->totalSessions - 1; ++i) {
		studentCount[i] = 0;
		string session = " S" + to_string(i + 1) + " |";
		cout << setw(6) << session;
	}
	studentCount[course->totalSessions - 1] = 0;
	cout << " S" + to_string(course->totalSessions) + "\n";
	cout << "\t" << setfill('-') << setw(20);
	for (int i = 0; i < course->totalSessions; ++i)
		cout << "+" << setw(6);
	cout << "\n";
	Student* currentStudent = course->students;
	while (currentStudent != nullptr) {
		string id = currentStudent->studentId + " |";
		cout << "\t" << setfill(' ') << setw(20) << id;
		currentAttendance = currentStudentInfo->attendance;
		for (int i = 0; i < course->totalSessions - 1; ++i) {
			string time = timeToString(currentAttendance->time) + "|";
			cout << setw(6) << time;
			if (isPresent(currentAttendance))
				studentCount[i]++;
			currentAttendance = currentAttendance->next;
		}
		cout << timeToString(currentAttendance->time) << "\n";
		if (isPresent(currentAttendance))
			studentCount[course->totalSessions - 1]++;
		cout << "\t" << setfill('-') << setw(20);
		for (int i = 0; i < course->totalSessions; ++i)
			cout << "+" << setw(6);
		cout << "\n";
		currentStudent = currentStudent->next;
	}
	cout << "\t" << setfill(' ') << setw(20) << "Total |";
	for (int i = 0; i < course->totalSessions - 1; ++i) {
		string total = to_string(studentCount[i]) + " |";
		cout << setw(6) << total;
	}
	cout << " " << studentCount[course->totalSessions - 1] << "\n\n";
	delete[] studentCount;
}

// Check whether a student exists in a course, based on student ID, if yes store the info.
bool isStudentExistInCourse(string studentId, Course* course) {
	Student* currentStudent = course->students;
	while (currentStudent != nullptr) {
		if (currentStudent->studentId == studentId)
			return true;
		currentStudent = currentStudent->next;
	}
	return false;
}

// Check whether a session date is valid.
bool isSessionDateExist(Date date, Attendance* attendance) {
	Attendance* currentAttendance = attendance;
	while (currentAttendance != nullptr) {
		if (currentAttendance->date.year == date.year &&
			currentAttendance->date.month == date.month &&
			currentAttendance->date.day == date.day)
			return true;
		currentAttendance = currentAttendance->next;
	}
}
