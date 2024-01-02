#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <map>
#include <set>
#include "airline.h"
#include "airport.h"
#include "graph.h"

#define MaxTripVector std::vector<std::pair<Airport *, Airport *>>

/**
 * Object that stores all the base data for the program.
 * Contains 2 graphs representing the flight network, 
 * a two maps, one with all the airlines and one with the airports for each city.
 * Additionally, this class is responsible for the calculation of simple statistics on its own data. 
*/
class Manager {
	private:
		Graph<Airport *, int> connections;
		Graph<Airport *, Airline *> available_flights;
		std::map<std::string, Airline *> airlines;
        std::map<std::string, std::list<Vertex<Airport *, Airline *>*>> cityAirportList;
	public:
		Manager() {}
		~Manager();
		/** @return Graph of connections, without repeating edges */
		Graph<Airport *, int> getConnections();
		Graph<Airport *, Airline *> getFlights();
		std::map<std::string, Airline *> getAirlines();
		
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

/** @return Graph of connections, without repeating edges */
inline Graph<Airport *, int> Manager::getConnections() { 
	return connections; 
}
/** 
 * @return Graph of flights, with repeating edges 
 * if there is more than one airline that operated the route. 
 */
inline Graph<Airport *, Airline *> Manager::getFlights() { 
	return available_flights; 
}

/** @return Map of Airlines */
inline std::map<std::string, Airline *> Manager::getAirlines() { 
	return airlines;
}

#endif