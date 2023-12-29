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

		void statsMenu();
		void globalStats();
		void showMax();
		void showTop(std::string str);

		void plannerInMenu();
		void plannerAirportInSelect();
		std::vector<Airport> searchInAirport(std::string query);
		void plannerCityInSelect();
		std::set<std::string> searchInCity(std::string query);
		void plannerCoordsInSelect();
		std::vector<Airport> searchInCoords(double lat, double lon);
		
		void plannerSelected();

		void plannerOutMenu();
		void plannerAirportOutSelect();
		std::vector<Airport> searchOutAirport(std::string query);
		void plannerCityOutSelect();
		std::set<std::string> searchOutCity(std::string query);
		void plannerCoordsOutSelect();
		std::vector<Airport> searchOutCoords(double lat, double lon);

		void displayFlights();

		void statsAirportSelect();
		void statsCitySelect();
		void statsAirlineSelect();
		std::vector<Airline> searchAirline(std::string query);

		void showAirport(std::string str);
		void showCity(std::string str);
		void showAirline(std::string str);
		
		void helpMsg(std::string error, std::string usage);

		void test();
	public:
		UI();
		void mainMenu();
};

#endif