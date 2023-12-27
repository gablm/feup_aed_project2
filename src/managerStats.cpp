#include "headers/manager.h"
#include <set>

//i
int Manager::airportCount() {
	return airports.size();
}

int Manager::flightCount() {
	int count = 0;
	for (auto i : available_flights.getVertexSet())
		count += i->getAdj().size();
	return count;
}

int Manager::airlineCount() {
	return airlines.size();
}

//ii
std::vector<size_t> Manager::airportStats(std::string code) {
	auto vtx = available_flights.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};
	
	std::set<Airline> airlines;

	for (auto i : vtx->getAdj()) {
		auto w = i.getInfo();
		airlines.insert(i.getInfo());
	}
	return {vtx->getAdj().size(), airlines.size()};
}

//v
std::vector<size_t> Manager::destinationsFromAirport(std::string code) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};
		
	std::set<std::string> cities, countries;

	for (auto i : vtx->getAdj()) {
		auto flg = i.getDest()->getInfo();
		cities.insert(flg.getCity());
		countries.insert(flg.getCountry());
	}

	return {vtx->getAdj().size(), cities.size(), countries.size()};
}

//vi
void dfsVisit(Vertex<Airport, std::string> *vtx, size_t *cnt, std::set<std::string> &cities, std::set<std::string> &countries, int stops) {
	vtx->setVisited(true);
	auto airport = vtx->getInfo();
	cities.insert(airport.getCity());
	countries.insert(airport.getCountry());
	*cnt += 1;

	if (stops == 0)
		return;

	for (auto i : vtx->getAdj()) {
		auto w = i.getDest();
		if (!w->isVisited())
			dfsVisit(w, cnt, cities, countries, stops - 1);
	}
}

std::vector<size_t> Manager::reachableDestinationsFromAirport(std::string code, int x) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};

	if (x <= 0)
		x = __INT32_MAX__;

	std::set<std::string> cities, countries;
	size_t count = 0;
	for (auto i : connections.getVertexSet())
		i->setVisited(false);

	for (auto i : vtx->getAdj()) {
		auto w = i.getDest();
		if (!w->isVisited())
			dfsVisit(w, &count, cities, countries, x - 1);
	}
	

	return {count, cities.size(), countries.size()};
}