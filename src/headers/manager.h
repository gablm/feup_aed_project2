#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <map>
#include <set>
#include "airline.h"
#include "airport.h"
#include "graph.h"

#define MaxTripVector std::vector<std::pair<Airport *, Airport *>>

class Manager {
	private:
		Graph<Airport *, int> connections;
		Graph<Airport *, Airline *> available_flights;
		std::map<std::string, Airline *> airlines;
        std::map<std::string, std::list<Vertex<Airport *, Airline *>*>> cityAirportList;
	public:
		Manager() {}
		~Manager();
		Graph<Airport *, int> getConnections() { return connections; }
		Graph<Airport *, Airline *> getFlights() { return available_flights; }
		std::map<std::string, Airline *> getAirlines() { return airlines; }
		
		void loadAirports();
		void loadAirlines();
		void loadFlights();

		double static distance(double la1, double lo1, double la2, double lo2);

		//i
		int airportCount();
		int flightCount();
		int airlineCount();
		//ii
		std::vector<size_t> airportStats(std::string code);
		//iii
		std::vector<size_t> cityStats(std::string code);
		std::vector<size_t> airlineStats(std::string code);
		
		//v
		std::vector<size_t> destinationsFromAirport(std::string code);
		//vi
		std::vector<size_t> reachableDestinationsFromAirport(std::string code, int x);
		//vii
		void bfsFind(Vertex<Airport *, int> *vtx, int &maxTrip, MaxTripVector &res);
		std::pair<MaxTripVector, int> maximumTrip();
		//viii
		std::vector<Vertex<Airport *, Airline *>*> airportsWithMostTraffic(size_t x);
		//ix
		std::set<Airport *> essentialAirports();
};

#endif