#include "Function.h"
#include<ctime>
// ========= STUDENTS' FUNCTIONS DEFINITION =========


// 7.1
void checkin(string studentUsername) {
	Student newTurn;
	getInfoOfStudent(newTurn, studentUsername);

	//get all courses of a student
	CourseInfo* cur = newTurn.myCourse;
	int  n = newTurn.numberOfCourse;
	if (n == 0) {
		cout << "You are currently not taking any course." << endl;
		cout << endl;
		return;
	}
	Attendance* attendanceDate = nullptr;
	printCourseListTable(newTurn.myCourse);
	cout << "\tEnter the no. of course to check-in: ";
	int choice;
	cin >> choice;
	cout << endl;
	CourseInfo* current = newTurn.myCourse;
	for (int j = 0; j < choice; j++) {
		current = current->next;
	}
	ifstream(in);
	in.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear +1 ) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
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

	time_t t = time(0); // get time now 
	struct tm* now = localtime(&t);
	Date currentDate = { now->tm_mday, now->tm_mon + 1, now->tm_year + 1900 };
	Time currentTime = { now->tm_hour, now->tm_min };
	cout << "\tCurrent time: " << timeToString(currentTime) << "\n\tCurrent date: " << dateToString(currentDate) << endl;
	Attendance* temp = attendanceDate;
	int count = 0, x = 0, o = 0;
	while (temp != nullptr) {
		if (temp->date.year == now->tm_year + 1900 &&
			temp->date.month == now->tm_mon + 1 &&
			temp->date.day == now->tm_mday) {
			if ((now->tm_hour < temp->startTime.hour) ||
				(now->tm_hour == temp->startTime.hour &&
					now->tm_min < temp->startTime.minute)) {
				cout << "\tThere is no session to check-in now.\n" << endl;
				deleteAttendance(attendanceDate);
				deleteCourseInfo(newTurn.myCourse);
				return;
			}
			else if ((now->tm_hour > temp->endTime.hour) ||
				(now->tm_hour == temp->endTime.hour &&
					now->tm_min > temp->endTime.minute))
			{
				cout << "\tThere is no session to check-in now.\n" << endl;
				deleteAttendance(attendanceDate);
				deleteCourseInfo(newTurn.myCourse);
				return;
			}
			else
				break;
		}
		else if (temp->next == nullptr)
		{
			cout << "\tThere is no session today.\n" << endl;
			deleteAttendance(attendanceDate);
			deleteCourseInfo(newTurn.myCourse);
			return;
		}
		else {
			temp = temp->next;
			count++;
		}
	}
	string* userFile;
	string check;

	ifstream filein;
	filein.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
	while (filein) {
		getline(filein, check);
		x++;
	}
	filein.close();

	ifstream filin;
	filin.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
	while (filin) {
		getline(filin, check);
		o++;
		if (check == studentUsername) break;
	}
	filin.close();

	userFile = new string[x];
	ifstream file;
	file.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
	for (int i = 0; i < x; i++) {
		getline(file, userFile[i]);
	}
	file.close();

	//rewrite file text
	ofstream out;
	out.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
	for (int l = 0; l < x; l++) {
		if (l == o + count + 6) {
			out << temp->date.day << " " << temp->date.month << " " << temp->date.year << " "
				<< temp->startTime.hour << " " << temp->startTime.minute << " "
				<< temp->endTime.hour << " " << temp->endTime.minute << " "
				<< now->tm_hour << " " << now->tm_min << endl;
		}
		else out << userFile[l] << endl;
	}
	out.close();
	cout << "\tCheckin successfully.\n" << endl;

	//deallocated
	deleteAttendance(attendanceDate);
	deleteCourseInfo(newTurn.myCourse);
	delete[]userFile;
}

// 7.2
void viewCheckinResult(string studentUsername) {
	Student newTurn;
	getInfoOfStudent(newTurn, studentUsername);

	//get all courses of a student
	CourseInfo* cur = newTurn.myCourse;
	int  n = newTurn.numberOfCourse;
	if (n == 0) {
		cout << "You are currently not taking any course." << endl;
		cout << endl;
		return;
	}
	// Read attendance list.
	Attendance* attendance = nullptr;
	printCourseListTable(newTurn.myCourse);
	cout << "\tEnter the no. of course to view checkin results: ";
	int choice;
	cin >> choice;
	CourseInfo* current = newTurn.myCourse;
	for (int j = 0; j < choice; j++) {
		current = current->next;
	}

	ifstream(in);
	in.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
		+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
	int totalSessions;
	Attendance* currentAttendance = nullptr;
	if (in) {
		string waste;
		for (int i = 0; i < 6; ++i) getline(in, waste); // 6 lines of irrelevant information.
		in >> totalSessions;
	}
	while (in) {
		string waste;
		getline(in, waste);
		if (waste == studentUsername) {
			for (int i = 0; i < 6; ++i) getline(in, waste);
			for (int i =0, day , month, year , starthour,
				startminute, endhour,endminute,hour,minute ; i < totalSessions; ++i) {
				in >> day >> month >> year >> starthour >>
					startminute >> endhour >> endminute >> hour >> minute;
				if (currentAttendance == nullptr) {
					attendance = new Attendance;
					currentAttendance = attendance;
				}
				else {
					currentAttendance->next = new Attendance;
					currentAttendance = currentAttendance->next;
				}
				currentAttendance->date = Date{ day, month, year };
				currentAttendance->startTime = Time{ starthour, startminute };
				currentAttendance->endTime = Time{ endhour, endminute };
				currentAttendance->time = Time{ hour, minute };
				currentAttendance->next = nullptr;
			}
		}
	}
	in.close();
	// print out result
	cout << "\n\t\t\t\tCHECKIN RESULTS\n";
	cout << "\t" << setw(20) << "Date |" << setw(20) << "Study time |" << " Check-in time\n";
	cout << "\t" << setfill('-') << setw(20) << "+" << setw(20) << "+" << setw(20) << "\n";
	Attendance* curAttendance = attendance;
	while (curAttendance != nullptr)
	{
		Date date = { curAttendance->date.day, curAttendance->date.month, curAttendance->date.year };
		cout << "\t" << setfill(' ') << setw(19) << dateToString(date) << "|";
		string sessionTime = timeToString(curAttendance->startTime) + "-" + timeToString(curAttendance->endTime);
		cout << setw(19) << sessionTime << "| " << timeToString(curAttendance->time) << "\n";
		curAttendance = curAttendance->next;
	}
	cout << "\n";
	deleteCourseInfo(newTurn.myCourse);
	deleteAttendance(attendance);
}

// 7.3
void viewSchedules(string studentUsername) {
	Student newTurn;
	getInfoOfStudent(newTurn, studentUsername);

	// Get all courses of a student.
	CourseInfo* cur = newTurn.myCourse;
	int  n = newTurn.numberOfCourse;
	if (n == 0) {
		cout << "You are currently not taking any course." << endl;
		cout << endl;
		return;
	}
	Course* coursesOfStudent;
	coursesOfStudent = new Course[n];
	string day[4], startDay, endDay;
	for (int i = 0; i < n; i++)
	{
		ifstream(fin);
		fin.open("Database/" + to_string(cur->academicYear) + "-" + to_string(cur->academicYear + 1) + "/"
			+ cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
		while (fin) {
			getline(fin, coursesOfStudent[i].courseId);
			getline(fin, coursesOfStudent[i].courseName);
			getline(fin, coursesOfStudent[i].defaultClass);
			getline(fin, coursesOfStudent[i].lecturer.username);
			getline(fin, startDay);
			getline(fin, endDay);
			fin >> coursesOfStudent[i].totalSessions;
			fin >> coursesOfStudent[i].sessionsPerWeek;
			coursesOfStudent[i].sessionInfo = nullptr;
			SessionInfo* tmp = 0;
			for (int k = 0; k < coursesOfStudent[i].sessionsPerWeek; k++) {
				if (coursesOfStudent[i].sessionInfo == nullptr) {
					coursesOfStudent[i].sessionInfo = new SessionInfo;
					fin >> coursesOfStudent[i].sessionInfo->day;
					fin >> coursesOfStudent[i].sessionInfo->startTime.hour;
					fin >> coursesOfStudent[i].sessionInfo->startTime.minute;
					fin >> coursesOfStudent[i].sessionInfo->endTime.hour;
					fin >> coursesOfStudent[i].sessionInfo->endTime.minute;
					coursesOfStudent[i].sessionInfo->next = nullptr;
					tmp = coursesOfStudent[i].sessionInfo;
				}
				else
				{
					tmp->next = new SessionInfo;
					tmp = tmp->next;
					fin >> tmp->day;
					fin >> tmp->startTime.hour;
					fin >> tmp->startTime.minute;
					fin >> tmp->endTime.hour;
					fin >> tmp->endTime.minute;
					tmp->next = nullptr;
				}
			}
			fin >> coursesOfStudent[i].room;
			break;
		}
		fin.close();
		coursesOfStudent[i].semester = cur->semester;
		cur = cur->next;
	}

	// Build up the schedule.
	Course mon[4], tue[4], wed[4], thu[4], fri[4], sat[4];
	for (int t = 0; t < n; t++) {
		SessionInfo* tail = coursesOfStudent[t].sessionInfo;
		while (tail != nullptr) {
			switch (tail->day)
			{
			case 2:
			{
				if (tail->startTime.hour == 7)
				{
					mon[0].courseId = coursesOfStudent[t].courseId;
					mon[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					mon[1].courseId = coursesOfStudent[t].courseId;
					mon[1].room = coursesOfStudent->room;

				}
				else if (tail->startTime.hour == 13)
				{
					mon[2].courseId = coursesOfStudent[t].courseId;
					mon[2].room = coursesOfStudent->room;

				}
				else if (tail->startTime.hour == 15)
				{
					mon[3].courseId = coursesOfStudent[t].courseId;
					mon[3].room = coursesOfStudent->room;

				}
				break;
			}
			case 3:
			{
				if (tail->startTime.hour == 7)
				{
					tue[0].courseId = coursesOfStudent[t].courseId;
					tue[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					tue[1].courseId = coursesOfStudent[t].courseId;
					tue[1].room = coursesOfStudent->room;

				}
				else if (tail->startTime.hour == 13)
				{
					tue[2].courseId = coursesOfStudent[t].courseId;
					tue[2].room = coursesOfStudent->room;

				}
				else if (tail->startTime.hour == 15)
				{
					tue[3].courseId = coursesOfStudent[t].courseId;
					tue[3].room = coursesOfStudent->room;
				}
				break;
			}
			case 4:
			{
				if (tail->startTime.hour == 7)
				{
					wed[0].courseId = coursesOfStudent[t].courseId;
					wed[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					wed[1].courseId = coursesOfStudent[t].courseId;
					wed[1].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 13)
				{
					wed[2].courseId = coursesOfStudent[t].courseId;
					wed[2].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 15)
				{
					wed[3].courseId = coursesOfStudent[t].courseId;
					wed[3].room = coursesOfStudent->room;
				}
				break;
			}
			case 5:
			{
				if (tail->startTime.hour == 7)
				{
					thu[0].courseId = coursesOfStudent[t].courseId;
					thu[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					thu[1].courseId = coursesOfStudent[t].courseId;
					thu[1].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 13)
				{
					thu[2].courseId = coursesOfStudent[t].courseId;
					thu[2].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 15)
				{
					thu[3].courseId = coursesOfStudent[t].courseId;
					thu[3].room = coursesOfStudent->room;
				}
				break;
			}
			case 6:
			{
				if (tail->startTime.hour == 7)
				{
					fri[0].courseId = coursesOfStudent[t].courseId;
					fri[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					fri[1].courseId = coursesOfStudent[t].courseId;
					fri[1].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 13)
				{
					fri[2].courseId = coursesOfStudent[t].courseId;
					fri[2].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 15)
				{
					fri[3].courseId = coursesOfStudent[t].courseId;
					fri[3].room = coursesOfStudent->room;
				}
				break;
			}
			case 7:
			{
				if (tail->startTime.hour == 7)
				{
					sat[0].courseId = coursesOfStudent[t].courseId;
					sat[0].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 9)
				{
					sat[1].courseId = coursesOfStudent[t].courseId;
					sat[1].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 13)
				{
					sat[2].courseId = coursesOfStudent[t].courseId;
					sat[2].room = coursesOfStudent->room;
				}
				else if (tail->startTime.hour == 15)
				{
					sat[3].courseId = coursesOfStudent[t].courseId;
					sat[3].room = coursesOfStudent->room;
				}
				break;
			}
			default:
				break;
			}
			tail = tail->next;
		}
	}

	// Print out schedule.
	cout << "\t\t\t\t\t\t\tMY SCHEDULE\n\n";
	cout << "   " << "Session |" << setw(20) << "Time |" << setw(14) << "Monday |" << setw(14) << "Tuesday |" << setw(14) << "Wednesday |"
		<< setw(14) << "Thursday |" << setw(14) << "Friday |" << setw(14) << "Saturday |" << endl;
	cout << "   " << setfill('-') << setw(9) << "+" << setw(20) << "+" << setw(14) << "+" << setw(14) << "+"
		<< setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << endl;
	cout << tue[1].courseId;

	cout << "   " << setfill(' ') << setw(9) << "1 |" << setw(20) << "7h30-9h10 |" << setw(12) << mon[0].courseId << " |"
		<< setw(12) << tue[0].courseId << " |" << setw(12) << wed[0].courseId << " |" << setw(12) << thu[0].courseId
		<< " |" << setw(12) << fri[0].courseId << " |" << setw(12) << sat[0].courseId << " |" << endl;
	cout << "   " << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 7h30-9h30 |" << setw(12) << mon[0].room << " |"
		<< setw(12) << tue[0].room << " |" << setw(12) << wed[0].room << " |" << setw(12) << thu[0].room
		<< " |" << setw(12) << fri[0].room << " |" << setw(12) << sat[0].room << " |" << endl;
	cout << "   " << setfill('-') << setw(9) << "+" << setw(20) << "+" << setw(14) << "+" << setw(14) << "+"
		<< setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << endl;

	cout << "   " << setfill(' ') << setw(9) << "2 |" << setw(20) << "9h30-11h10 |" << setw(12) << mon[1].courseId << " |"
		<< setw(12) << tue[1].courseId << " |" << setw(12) << wed[1].courseId << " |" << setw(12) << thu[1].courseId
		<< " |" << setw(12) << fri[1].courseId << " |" << setw(12) << sat[1].courseId << " |" << endl;
	cout << "   " << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 9h30-11h30 |" << setw(12) << mon[1].room << " |"
		<< setw(12) << tue[1].room << " |" << setw(12) << wed[1].room << " |" << setw(12) << thu[1].room
		<< " |" << setw(12) << fri[1].room << " |" << setw(12) << sat[1].room << " |" << endl;
	cout << "   " << setfill('-') << setw(9) << "+" << setw(20) << "+" << setw(14) << "+" << setw(14) << "+"
		<< setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << endl;

	cout << "   " << setfill(' ') << setw(9) << "3 |" << setw(20) << "13h30-15h10 |" << setw(12) << mon[2].courseId << " |"
		<< setw(12) << tue[2].courseId << " |" << setw(12) << wed[2].courseId << " |" << setw(12) << thu[2].courseId
		<< " |" << setw(12) << fri[2].courseId << " |" << setw(12) << sat[2].courseId << " |" << endl;
	cout << "   " << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 13h30-15h30 |" << setw(12) << mon[2].room << " |"
		<< setw(12) << tue[2].room << " |" << setw(12) << wed[2].room << " |" << setw(12) << thu[2].room
		<< " |" << setw(12) << fri[2].room << " |" << setw(12) << sat[2].room << " |" << endl;
	cout << "   " << setfill('-') << setw(9) << "+" << setw(20) << "+" << setw(14) << "+" << setw(14) << "+"
		<< setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << endl;

	cout << "   " << setfill(' ') << setw(9) << "4 |" << setw(20) << "15h30-17h10 |" << setw(12) << mon[3].courseId << " |"
		<< setw(12) << tue[3].courseId << " |" << setw(12) << wed[3].courseId << " |" << setw(12) << thu[3].courseId
		<< " |" << setw(12) << fri[3].courseId << " |" << setw(12) << sat[3].courseId << " |" << endl;
    cout << "   " << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 15h30-17h30 |" << setw(12) << mon[3].room << " |"
		<< setw(12) << tue[3].room << " |" << setw(12) << wed[3].room << " |" << setw(12) << thu[3].room
		<< " |" << setw(12) << fri[3].room << " |" << setw(12) << sat[3].room << " |" << endl;

	cout << "\n";

	// Deallocated.
	for (int l = 0; l < n; l++) {
		if (coursesOfStudent[l].sessionInfo != nullptr)
			deleteSessionInfo(coursesOfStudent[l].sessionInfo);

	}
	deleteCourseInfo(newTurn.myCourse);
}

// 7.4
void viewScoresOfACourse(string studentUsername) {
	Student newTurn;
	getInfoOfStudent(newTurn, studentUsername);
	int  n = newTurn.numberOfCourse;
	if (n == 0) {
		cout << "You are currently not taking any course." << endl;
		cout << endl;
		return;
	}
	int choice; 
	CourseInfo* check = newTurn.myCourse;

	//print out courses for student to choose
	printCourseListTable(newTurn.myCourse);
	cout << "\tEnter no. of course to view scores: ";
	cin >> choice;
	for (int m = 0; m < choice-1; m++) {
		newTurn.myCourse = newTurn.myCourse->next;
	}

	// Get the student's scores of the course which be chosen.
	string checkName;
	StudentCourseInfo scoresOfNewTurn;
	ifstream(fin);
	fin.open("Database/" + to_string(newTurn.myCourse->academicYear) + "-" + to_string(newTurn.myCourse->academicYear + 1) + "/"
		+ newTurn.myCourse->semester + "/" + newTurn.myCourse->courseName + "-" + newTurn.myCourse->defaultClass + ".txt");
	if (!fin) cout << "Cannot open the course file now, please try later." << endl;
	while (fin) {
		getline(fin, checkName);
		if (checkName == studentUsername) {
			for (int n= 0; n < 5; n++) getline(fin,checkName);
			fin >> scoresOfNewTurn.midterm >> scoresOfNewTurn.final >> scoresOfNewTurn.lab >> scoresOfNewTurn.bonus;
			break;
		}
	}

	cout << "\n\t" << setw(5) << "No.|" << setw(15) << "Score type |" << setw(10) << "Score\n";
	cout << "\t" << setfill('-') << setw(5) << "+" << setw(15) << "+" << setw(10) << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 1 << "|" << setw(15) << "Midterm |" << setw(9) << scoresOfNewTurn.midterm << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 2 << "|" << setw(15) << "  Final |" << setw(9) << scoresOfNewTurn.final << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 3 << "|" << setw(15) << "    Lab |" << setw(9) << scoresOfNewTurn.lab << "\n";
	cout << "\t" << setfill(' ') << setw(4) << 4 << "|" << setw(15) << "  Bonus |" << setw(9) << scoresOfNewTurn.bonus << "\n\n";

	deleteCourseInfo(newTurn.myCourse);
}

