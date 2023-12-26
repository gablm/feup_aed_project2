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
		std::thread loader;
		double loadtime;
		bool loading = true;
	public:
		UI();
		void setManager(Manager value);
		void setLoading(bool value);
		static void loadAll(UI *ui);

		void mainMenu();
		void globalStats();

		void HelpMsg(std::string error, std::string usage);
};

#endif