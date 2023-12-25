#include "headers/manager.h"

int main() {
	Manager manager;
	manager.load_airports();
	manager.test_airports();
	return 0;
}