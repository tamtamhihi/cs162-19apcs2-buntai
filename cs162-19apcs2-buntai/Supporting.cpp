#include "Function.h"


// ========= SUPPORTING FUNCTIONS DEFINITION =========


// ====== GENERAL ======
void toUpper(string& text) {
	for (char& c : text)
		c = toupper(c);
}
void toLower(string& text) {
	for (char& c : text)
		c = tolower(c);
}
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
Date getDob(string dob) {
	stringstream DOB(dob);
	string year, month, day;
	getline(DOB, year, '-');
	getline(DOB, month, '-');
	getline(DOB, day, '-');
	Date Dob;
	Dob.day = stoi(day);
	Dob.month = stoi(month);
	Dob.year = stoi(year);
	return Dob;
}
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


// ====== APP-RELATED ======
void writeClassToFile(Student*& students, string className) {
	ofstream out;
	toUpper(className);
	out.open("Database/Class/" + className + ".txt", ios::app);
	Student* currentStudent = students;
	while (currentStudent != nullptr) {
		out << currentStudent->username << "\n";
		out << currentStudent->password << "\n";
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
				<< currentCourseInfo->courseName << "\n";
		}
		out << "\n";
		currentStudent = currentStudent->next;
	}
	out.close();
}
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
void addUser(string username, string password, int type) {
	ofstream out("Database/User.txt", ios::app);
	out << username << "\n" << password << "\n" << type << "\n";
	out.close();
}
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
void deleteStudentList(Student*& studentList) {
	while (studentList != nullptr) {
		if (studentList->myCourse != nullptr)
			delete studentList->myCourse;
		Student* temp = studentList;
		studentList = studentList->next;
		delete temp;
	}
}