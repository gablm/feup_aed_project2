#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <map>
#include "airline.h"
#include "airport.h"
#include "graph.h"

class Manager {
	private:
		Graph<Airport, std::string> connections;
		Graph<Airport, Airline> available_flights;
		std::map<std::string, Airline> airlines;
		std::map<std::string, Airport> airports;
        std::map<std::string,std::list<Vertex<Airport, Airline>*>> cityAirportList;
	public:
		Manager() {}
		void loadAirports();
		void testAirports();

		void loadAirlines();
		void testAirlines();
		
		void loadFlights();
		void testFlights();

		//i
		int airportCount();
		int flightCount();
		int airlineCount();
};

#endif