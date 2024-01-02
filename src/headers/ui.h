#ifndef UI_H_
#define UI_H_

#include "manager.h"
#include <thread>

#ifdef __linux__
# define CLEAR int retv = system("clear"); if (retv == -1) {}
#else
# define CLEAR int retv = system("cls"); if (retv == -1) {}
#endif

#define Trip std::pair<list<Airport*>, list<Airline *>>
class UI {
	private:
		Manager manager;
		double loadtime;

		vector<Airport *> origin;
		vector<Airport *> destination;
		set<Airline *> allowedAirlines;
		vector<Trip> plannerResult;
		size_t maxAirlines;
		//bool useRecursionLimiter = true;

		void statsMenu();
		void globalStats();
		void showMax();
		void showTop(int x);
		void showEssential();

		void plannerMenu(bool in);
		void plannerAirportSelect(bool in);
		std::vector<Airport *> searchAirport(std::string query, bool in);
		void plannerCitySelect(bool in);
		std::set<std::string> searchCity(std::string query, bool in);
		void plannerCoordsSelect(bool in);
		std::vector<Airport *> searchCoords(double lat, double lon, bool in);
		void plannerSelected();
		void filterSelect();
		void filterSelectMax();
		void filterSelectList();
		//void filterDisableLimiter();

		void displayFlights(vector<Trip> &lst);
		void buildFlights(bool way);
		void printPath(Trip path);

		void findPathFilter(vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end);
		void fastFindPath(vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end);
		void storeResult(list<Airport *> ports, list<Airline *> lines, Vertex<Airport *, Airline *> *curr, vector<Vertex<Airport *, Airline *> *> start, int time, set<Airline *> airlineSet);
		int getNextMax(int maxL);

		void static findFilter(UI *who, vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end, bool *loading);
		bool strFind(const std::string &one, const std::string &two);

		void statsAirportSelect();
		void statsCitySelect();
		void statsAirlineSelect();
		std::vector<Airline *> searchAirline(std::string query);

		void showAirport(std::string str);
		void showCity(std::string str);
		void showAirline(std::string str);

		void helpMsg(std::string error, std::string usage);
	public:
		UI();
		void mainMenu();
};

#endif