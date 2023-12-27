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
		void statsMenu();
		void globalStats();

		void plannerMenu();
		void plannerAirportSelect();
		vector<Airport> searchAirport(std::string query);

		void showAirport(std::string str);
		void showCity(std::string str);
		void showAirline(std::string str);
		

		void helpMsg(std::string error, std::string usage);

		void test();
};

#endif