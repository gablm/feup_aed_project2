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

		vector<Airport> origin;
		vector<Airport> destination;
	public:
		UI();

		void mainMenu();
		void statsMenu();
		void globalStats();

		void plannerMenu();
		void plannerAirportSelect();
		std::vector<Airport> searchAirport(std::string query);
		void plannerCitySelect();
		std::set<std::string> searchCity(std::string query);
		void plannerCoordsSelect();
		std::vector<Airport> searchCoords(double lat, double lon);
		
		void plannerSelected();

		void showAirport(std::string str);
		void showCity(std::string str);
		void showAirline(std::string str);
		

		void helpMsg(std::string error, std::string usage);

		void test();
};

#endif