#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "airline.h"
#include "airport.h"
#include "graph.h"

class Manager {
	private:
		Graph<Airport, std::string> connections;
		Graph<Airport, Airline> available_flights;
		std::list<Airport> airports;
		std::list<Airline> airlines;
	public:
		Manager() {}
		void load_airports();
		void test_airports();

		void load_airlines();
		void test_airlines();
		
		void load_flights();
		void test_flights();

		template <class T, class W>
		Vertex<T, W> *findAirportByCode(Graph<T, W> graph, std::string code);
};

#endif