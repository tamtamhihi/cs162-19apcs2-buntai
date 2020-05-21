#include "Struct.h"


// ========= ALL FUNCTION PROTOTYPES =========


// ====== ALL-ROLES ======
void login(string& userName, int& userRole); // 1.1
void showMenu(int& userRole); // 1.2
void viewProfileInfo(string& userName, int& userRole); // 1.3
void changePassword(string& userName); //1.4
void logout(string& userName, int& userRole); //1.5

// ====== STAFF - CLASS ======
void importStudentFromCsv(); // 2.1
void manuallyAddStudent(); // 2.2
void editExistingStudent(); // 2.3
void removeStudent(); // 2.4
void changeStudentClass(); // 2.5
void viewListOfClasses(); // 2.6
void viewListOfStudentInAClass(); // 2.7


// ====== STAFF - COURSE ======
void manipulateAcademicYearsAndSemester(); // 3.1
void importCourseFromCsv(); // 3.2
void manuallyAddCourse(); // 3.3
void editExistingCourse(); // 3.4
void removeCourse(); // 3.5
void removeStudentFromCourse(); // 3.6
void addAStudentToCourse(); // 3.7
void viewListOfCourses(); // 3.8
void viewListOfStudentsOfCourse(); // 3.9
void viewAttendanceListOfCourse(); // 3.10
void manipulateAllLecturers(); // 3.11


// ====== STAFF - SCOREBOARD ======
void searchAndViewScoreboard(); // 4.1
void exportScoreboardToCsv(); // 4.2


// ====== STAFF - ATTENDANCE ======
void searchAndViewAttendance(); // 5.1
void exportAttendanceListToCsv(); // 5.2

// ====== LECTURER ======
void viewCoursesInSemester(string lecturerUsername); // 6.1
void viewStudentListOfCourse(string lecturerUsername); // 6.2
void viewAttendanceListOfCourseByLecturer(string lecturerUsername); // 6.3
void importScoreboardFromCsv(string lecturerUsername); // 6.5
void editGradeOfStudent(string lecturerUsername); // 6.6
void viewScoreboardOfCourse(string lecturerUsername); // 6.7


// ====== STUDENT ======
void viewSchedules(string studentUsername); // 7.3


// ====== SUPPORTING FUNCTION ======
void toUpper(string& text);
void toLower(string& text);
string toFormalCase(string name);

string toUsername(string fullName);
string toPassword(Date date);
string toPasswordGeneral(string name);

bool isDateStringSuitable(string date);
Date getDate(string date);
string dateToString(Date date);
string timeToString(Time time);

int dayToNumber(string dayOfWeek);
int getDayOfWeek(Date date);
bool isLeap(int year);
int daysInMonth(int month, int year);
int calculateDaysBetDates(Date startDate, Date endDate);
int calculateTotalSessions(Course* course);
Date dateAfterDays(Date startDate, int days);
Time getTime(string time);
string numToDay(int day);

bool isClassExist(string className);
bool isLecturerCourse(CourseInfo* courseInfo, string lecturerUsername);
bool isCourseExist(CourseInfo* courseInfo);
bool isCourseInCourseList(int academicYear, string semester, string courseId, CourseInfo*& courseList);
bool isLecturerExist(string lecturerAccount);
bool isStudentIdExist(string studentId);
bool isAcademicYearExist(int academicYear);
bool isSemesterExist(int academicYear, string mySemester);
bool isPresent(Attendance* attendance);

void removeSemesterDirectory(int academicYear, string semester);


// ====== APP-RELATED ======
int getVersion(string existent, string username);
string getValidUsername(string name);
void addUser(string username, string password, int type);
void addLecturerFromNewCourse(Lecturer lecturer, CourseInfo* courseInfo);
void addLecturer(Lecturer*& lecturer);
void addStudentUsers(Student*& studentList);
void addClass(string className);
void addAcademicYear(int academicYear);
void addSemester(int academicYear, string mySemester);

string findPasswordFromUsername(string username);
bool findLecturerFromUsername(string username, Lecturer& lecturer);
bool findStudentInfoFromFile(Student& newTurn, string userName);
void findAttendanceDateOfCourse(Attendance*& attendanceDate, CourseInfo*& courseInfo);
bool findStudentInfoFromId(Student& student, string studentId);
void getInfoOfStudent(Student& newTurn, string studentUsername);

void printStudentInfo(Student*& student);
void printStudentListTable(Student*& students);
void printScoreboardTable(Course* course);
void printScoreboardOfStudent(StudentCourseInfo* student);
void printLecturerInfo(Lecturer*& lecturer);
int printAllLecturersTable(Lecturer*& lecturers);
int printCourseListTable(CourseInfo* courseList);
void printAllSessionsTable(Attendance*& attendanceDate);
void printAttendanceListOfCourse(Course* course);
void printCourseInfo(Course* course);

void readCourseListFromFile(CourseInfo*& courseList, int academicYear, string semester);
void readClassFromFile(string className, Student*& studentList);
void readCourseFromFile(CourseInfo* courseInfo, Course*& course);
void readAcademicYearsFromFile(AcademicYear*& academicYears);
void readLecturersFromFile(Lecturer*& lecturers);
void readAttendanceList(Attendance*& attendance, CourseInfo* courseInfo, Student student);
void readAllCourseInfo(CourseInfo*& courseList);

void writeClassToFile(Student*& students, string className);
void writeCourseToFile(Course*& course);
void writeCourseListToFile(CourseInfo* courseList, int academicYear, string semester);
void writeAcademicYearsToFile(AcademicYear*& academicYears);
void writeLecturersToFile(Lecturer*& lecturers);

void deleteStudentList(Student*& studentList);
void deleteCourseInfo(CourseInfo*& courseInfo);
void deleteSessionInfo(SessionInfo*& sessionInfo);
void deleteStudent(Student*& students);
void deleteAttendance(Attendance*& attendance);
void deleteStudentCourseInfo(StudentCourseInfo*& studentCourseInfo);
void deleteCourse(Course*& myCourse);
void deleteAcademicYears(AcademicYear*& academicYears);
void deleteSemester(int academicYear, string semester);
void deleteLecturers(Lecturer*& lecturers);
void deleteAcademicYear(int academicYear);

void registerCourseForStudentList(Student*& students, CourseInfo*& courseInfo);
void unregisterCourseForStudent(Student*& student, CourseInfo*& courseInfo);

void showStaffMenu();
void showLecturerMenu();
void showStudentMenu();
