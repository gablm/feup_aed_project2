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
		set<Airline> allowedAirlines;
		int maxAirlines;

		void statsMenu();
		void globalStats();
		void showMax();
		void showTop(int x);
		void showEssential();

		void plannerMenu(bool in);
		void plannerAirportSelect(bool in);
		std::vector<Airport> searchAirport(std::string query, bool in);
		void plannerCitySelect(bool in);
		std::set<std::string> searchCity(std::string query, bool in);
		void plannerCoordsSelect(bool in);
		std::vector<Airport> searchCoords(double lat, double lon, bool in);
		void plannerSelected();
		void filterSelect();
		void filterSelectMax();
		void filterSelectList();

		bool isValid(list<std::pair<Airport, Airline>> path);

		void displayFlights();

		void statsAirportSelect();
		void statsCitySelect();
		void statsAirlineSelect();
		std::vector<Airline> searchAirline(std::string query);

		void showAirport(std::string str);
		void showCity(std::string str);
		void showAirline(std::string str);
		
		void helpMsg(std::string error, std::string usage);
	public:
		UI();
		void mainMenu();
};

#endif