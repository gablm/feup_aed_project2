#include "headers/manager.h"

int main() {
	Manager manager;
	manager.load_airports();
	manager.load_airlines();
	manager.load_flights();
	manager.test_flights();
	return 0;
}