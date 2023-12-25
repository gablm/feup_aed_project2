#include "headers/manager.h"

int main() {
	Manager manager;
	manager.load_airports();
	manager.load_airlines();
	manager.test_airlines();
	return 0;
}