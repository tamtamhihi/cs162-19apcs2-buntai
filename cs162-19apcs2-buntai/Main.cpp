#include "Function.h"

int main() {
	cout << "\t\t\t=============== BUNTAI SCHOOL MANAGEMENT ===============\n\n";
	string userName;
	int userRole;
	while (!login(userName, userRole)){
		clearScreen();
	}
	clearScreen();
	showAllRoleMenu(userName, userRole);
	return 0;
}