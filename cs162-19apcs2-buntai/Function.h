#include "Struct.h"


// ========= ALL FUNCTION PROTOTYPES =========


// ====== ALL-ROLES ======
void login(string& username, int& userrole); // 1.1
void showMenu(int& userrole); // 1.2
void staffMenu();
void lecturerMenu();
void studentMenu();

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
void removeCourse(); // 3.5


// ====== STAFF - SCOREBOARD ======


// ====== STAFF - ATTENDANCE ======


// ====== LECTURER ======


// ====== STUDENT ======


// ====== SUPPORTING FUNCTION ======
void toUpper(string& text);
void toLower(string& text);
string toUsername(string fullName);
string toPassword(Date date);
Date getDate(string date);
string toFormalCase(string name);
bool isClassExist(string className);
int dayToNumber(string dayOfWeek);
int getDayOfWeek(Date date);
bool isLeap(int year);
int daysInMonth(int month, int year);
int calculateDaysBetDates(Date startDate, Date endDate);
int calculateTotalSessions(Course* course);
Date dateAfterDays(Date startDate, int days);
Time getTime(string time);
string numToDay(int day);
bool isCourseExist(CourseInfo* courseInfo);
bool isLecturerExist(string lecturerAccount);
bool isStudentIdExist(string studentId);
bool isAcademicYearExist(int academicYear);
bool isSemesterExist(int academicYear, string mySemester);


// ====== APP-RELATED ======
void writeClassToFile(Student*& students, string className);
int getVersion(string existent, string username);
string getValidUsername(string name);
void addUser(string username, string password, int type);
void addStudentUsers(Student*& studentList);
void addClass(string className);
void deleteStudentList(Student*& studentList);
void deleteCourseInfo(CourseInfo*& courseInfo);
string findPasswordFromUsername(string username);
Lecturer findLecturerFromUsername(string username);
void printStudentInfo(Student*& student);
void readClassFromFile(string className, Student*& studentList);
void readCourseFromFile(CourseInfo* courseInfo, Course*& course);
void writeCourseToFile(Course*& course);
void deleteSessionInfo(SessionInfo*& sessionInfo);
void deleteStudent(Student*& students);
void deleteAttendance(Attendance*& attendance);
void deleteStudentCourseInfo(StudentCourseInfo*& studentCourseInfo);
void deleteCourse(Course*& myCourse);
void readCourseListFromFile(CourseInfo*& courseList, string academicYear, string semester);
void writeCourseListToFile(CourseInfo* courseList, string academicYear, string semester);
void addAcademicYear(int academicYear);
void addSemester(int academicYear, string mySemester);
void readAcademicYearsFromFile(AcademicYear*& academicYears);
void deleteAcademicYears(AcademicYear*& academicYears);
void writeAcademicYearsToFile(AcademicYear*& academicYears);
void deleteSemester(int academicYear, string semester);
void readLecturersFromFile(Lecturer*& lecturers);
void writeLecturersToFile(Lecturer*& lecturers);
void deleteLecturers(Lecturer*& lecturers);
void deleteAcademicYear(int academicYear);
