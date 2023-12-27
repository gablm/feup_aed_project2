#ifndef UI_H_
#define UI_H_

#include "manager.h"
#include <thread>

#ifdef __linux__
# define CLEAR system("clear")
#else
# define CLEAR system("cls")
#endif

class UI {
	private:
		Manager manager;
		double loadtime;
	public:
		UI();

		void mainMenu();
		void globalStats();

		void helpMsg(std::string error, std::string usage);

		void test();
};

#endif