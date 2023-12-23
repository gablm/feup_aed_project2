#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include "Graph.h"
#include "Airline.h"
#include "Airport.h"

class Manager {
	private:
		Graph<Airport, std::string> single_connections;
		Graph<Airport, Airline> available_flights;
		std::list<Airport> airports;
		std::list<Airline> airlines;
	public:
		Manager();
		~Manager();
		void load_airports();
		void load_airlines();
		void load_flights();
};

#endif