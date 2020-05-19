#include "Function.h"

// ========= STUDENTS' FUNCTIONS DEFINITION =========

//7.3

void viewSchedules(string studentUsername) {
	Student newTurn;
	getInfoOfStudent(newTurn, studentUsername);

	//get all courses of a student
	CourseInfo* cur = newTurn.myCourse;
	int  n = newTurn.numberOfCourse;
	if (n == 0) return;
	Course* coursesOfStudent;
	coursesOfStudent = new Course[n];
	string day[4], startDay, endDay;
	for (int i = 0; i < n; i++)
	{
		ifstream(fin);
		fin.open("Database/" + to_string(cur->academicYear - 1) + "-" + to_string(cur->academicYear) + "/" + cur->semester + "/" + cur->courseName + "-" + cur->defaultClass + ".txt");
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

	//build up the schedule
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

	//print out schedule

	cout << "Session |" << setw(20) << "Time |" << setw(14) << "Monday |" << setw(14) << "Tuesday |" << setw(14) << "Wednesday |"
		<< setw(14) << "Thursday |" << setw(14) << "Friday |" << setw(14) << "Saturday |" << endl;
	cout << setfill('-') << setw(9) << "+" << setw(20) << "+" << setw(14) << "+" << setw(14) << "+"
		<< setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << setw(14) << "+" << endl;
	cout << tue[1].courseId;

	cout << setfill(' ') << setw(9) << "1 |" << setw(20) << "7h30-9h10 |" << setw(12) << mon[0].courseId << " |"
		<< setw(12) << tue[0].courseId << " |" << setw(12) << wed[0].courseId << " |" << setw(12) << thu[0].courseId
		<< " |" << setw(12) << fri[0].courseId << " |" << setw(12) << sat[0].courseId << " |" << endl;
	cout << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 7h30-9h30 |" << setw(12) << mon[0].room << " |"
		<< setw(12) << tue[0].room << " |" << setw(12) << wed[0].room << " |" << setw(12) << thu[0].room
		<< " |" << setw(12) << fri[0].room << " |" << setw(12) << sat[0].room << " |" << endl;

	cout << setfill(' ') << setw(9) << "2 |" << setw(20) << "9h30-11h10 |" << setw(12) << mon[1].courseId << " |"
		<< setw(12) << tue[1].courseId << " |" << setw(12) << wed[1].courseId << " |" << setw(12) << thu[1].courseId
		<< " |" << setw(12) << fri[1].courseId << " |" << setw(12) << sat[1].courseId << " |" << endl;
	cout << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 9h30-11h30 |" << setw(12) << mon[1].room << " |"
		<< setw(12) << tue[1].room << " |" << setw(12) << wed[1].room << " |" << setw(12) << thu[1].room
		<< " |" << setw(12) << fri[1].room << " |" << setw(12) << sat[1].room << " |" << endl;

	cout << setfill(' ') << setw(9) << "3 |" << setw(20) << "13h30-15h10 |" << setw(12) << mon[2].courseId << " |"
		<< setw(12) << tue[2].courseId << " |" << setw(12) << wed[2].courseId << " |" << setw(12) << thu[2].courseId
		<< " |" << setw(12) << fri[2].courseId << " |" << setw(12) << sat[2].courseId << " |" << endl;
	cout << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 13h30-15h30 |" << setw(12) << mon[2].room << " |"
		<< setw(12) << tue[2].room << " |" << setw(12) << wed[2].room << " |" << setw(12) << thu[2].room
		<< " |" << setw(12) << fri[2].room << " |" << setw(12) << sat[2].room << " |" << endl;

	cout << setfill(' ') << setw(9) << "4 |" << setw(20) << "7h30-9h10 |" << setw(12) << mon[3].courseId << " |"
		<< setw(12) << tue[3].courseId << " |" << setw(12) << wed[3].courseId << " |" << setw(12) << thu[3].courseId
		<< " |" << setw(12) << fri[3].courseId << " |" << setw(12) << sat[3].courseId << " |" << endl;
	cout << setfill(' ') << setw(9) << " |" << setw(20) << "LAB: 7h30-9h30 |" << setw(12) << mon[3].room << " |"
		<< setw(12) << tue[3].room << " |" << setw(12) << wed[3].room << " |" << setw(12) << thu[3].room
		<< " |" << setw(12) << fri[3].room << " |" << setw(12) << sat[3].room << " |" << endl;

	//deallocated
	for (int l = 0; l < n; l++) {
		if (coursesOfStudent[l].sessionInfo != nullptr)
			deleteSessionInfo(coursesOfStudent[l].sessionInfo);

	}
	deleteCourseInfo(newTurn.myCourse);
}