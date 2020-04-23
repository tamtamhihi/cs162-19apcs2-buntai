#include "Struct.h"

// ========= ALL FUNCTION PROTOTYPES =========


// ====== ALL-ROLES ======


// ====== STAFF - CLASS ======
void importStudentFromCsv();


// ====== STAFF - COURSE ======


// ====== STAFF - SCOREBOARD ======


// ====== STAFF - ATTENDANCE ======


// ====== LECTURER ======


// ====== STUDENT ======


// ====== SUPPORTING FUNCTION ======
void toUpper(string& text);
void toLower(string& text);
string toUsername(string fullName);
string toPassword(Date dob);
Date getDob(string dob);


// ====== APP-RELATED ======
void writeClassToFile(Student*& students, string className);
int getVersion(string existent, string username);
string getValidUsername(string name);
void addUser(string username, string password, int type);
void addStudentUsers(Student*& studentList);
void addClass(string className);
void deleteStudentList(Student*& studentList);
