#include "headers/manager.h"
#include <set>
#include <unordered_set>
#include <iostream>
#include <algorithm>

//i
/**
 * @note Complexity: O(1)
 * @return Number of airports in the Graphs
*/
int Manager::airportCount() {
	return connections.getVertexSet().size();
}

/**
 * @note Complexity: O(V)
 * @return Number of flights in the available flights Graph
*/
int Manager::flightCount() {
	int count = 0;
	for (auto i : available_flights.getVertexSet())
		count += i->getAdj().size();
	return count;
}

/**
 * @note Complexity: O(1)
 * @return Number of airlines
*/
int Manager::airlineCount() {
	return airlines.size();
}

//ii
/**
 * Calculates the stats of a specific airport.
 * The return vector contains the number of flights and number of airlines operating in an airport.
 * @note Complexity: (E)
 * @param code Airport code 
 * @return vector with size 2
 */
std::vector<size_t> Manager::airportStats(std::string code) {
	auto vtx = available_flights.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};
	
	std::set<Airline *> airlines;

	for (auto i : vtx->getAdj())
		airlines.insert(i.getInfo());

	return {vtx->getAdj().size(), airlines.size()};
}

//iii
/**
 * Calculates the stats of a specific city.
 * The return vector contains the number of airports, the number of flights,
 * the number of airlines, the number of destination cities and the number of destination countries.
 * @note Complexity: (V + E)
 * @param code City name in the format "(city name), (country name)" 
 * @return vector with size 5
 */
std::vector<size_t> Manager::cityStats(std::string code){
	auto airportList = cityAirportList[code];
	
	if (airportList.empty())
		return {__INT64_MAX__};

	std::set<string> airlineList;
	std::set<string> cityList;
	std::set<string> countryList;

	size_t totalFlights = 0;

	for (auto airportPtr : airportList){
		for (auto i : airportPtr->getAdj()) {
			airlineList.insert(i.getInfo()->getName());
			cityList.insert(i.getDest()->getInfo()->getCity());
			countryList.insert(i.getDest()->getInfo()->getCountry());
		}
		totalFlights += airportPtr->getAdj().size();
	}
	return {airportList.size(), totalFlights, airlineList.size(), cityList.size(), countryList.size()};

}

/**
 * Calculates the stats of a specific city.
 * The return vector contains the number of flights,
 * the number of airports, the number of destination cities and the number of destination countries.
 * @note Complexity: (V + E)
 * @param code Airline code 
 * @return vector with size 3
 */
std::vector<size_t> Manager::airlineStats(std::string code){
	Airline *airline = airlines[code];
	unordered_set<string> airportList, cityList, countryList;
	size_t totalFlights = 0;

	for (auto airport : available_flights.getVertexSet()) {
		for (auto flight : airport->getAdj()) {
			if (flight.getInfo() == airline) {
				auto w1 = airport->getInfo();
				auto w2 = flight.getDest()->getInfo();
				totalFlights++;
				airportList.insert(w1->getCode());
				airportList.insert(w2->getCode());
				cityList.insert(w1->getCity() + ", " + w1->getCountry());
				cityList.insert(w2->getCity() + ", " + w2->getCountry());
				countryList.insert(w1->getCountry());
				countryList.insert(w2->getCountry());
			}
		}
	}

	return {totalFlights, airportList.size(), cityList.size(), countryList.size()};

}

//v
/**
 * Calculates the number of reacheable destinations directly from an airport.
 * The return vector contains the number of flights, 
 * the number of cities and the number of countries that the flights from an airport can reach.
 * @note Complexity: O(E)
 * @param code Airport code
 * @return vector with size 3
*/
std::vector<size_t> Manager::destinationsFromAirport(std::string code) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};
		
	std::unordered_set<std::string> cities, countries;

	for (auto i : vtx->getAdj()) {
		auto flg = i.getDest()->getInfo();
		cities.insert(flg->getCity() + ", " + flg->getCountry());
		countries.insert(flg->getCountry());
	}

	return {vtx->getAdj().size(), cities.size(), countries.size()};
}

//vi
/**
 * Calculates the number of reacheable destinations at X lay-overs of an airport.
 * The return vector contains the number of flights, 
 * the number of cities and the number of countries that the flights from an airport can reach.
 * @note Complexity: O(V + E)
 * @param code Airport code
 * @param x Number of maximum flights (lay-overs + 1)
 * @return vector with size 3
*/
std::vector<size_t> Manager::reachableDestinationsFromAirport(std::string code, int x) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};

	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
	}

	std::set<std::string> cities, countries;
	list<Vertex<Airport *, int>*> queue;
	size_t count = 0;
	vtx->setVisited(true);
	vtx->setNum(0);
	queue.push_back(vtx);

	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (!w->isVisited()) {
				auto lvl = u->getNum() + 1;
				w->setVisited(true);
				w->setNum(lvl);
				queue.push_back(w);
				if (lvl <= x) {
					auto wa = w->getInfo();
					cities.insert(wa->getCity() + ", " + wa->getCountry());
					countries.insert(wa->getCountry());
					count++;
				}
			}
		}
	}
	return {count, cities.size(), countries.size()};
}

//viii
/**
 * Copies the vertexSet from the flights Graph and orders it by descending outedge.
 * After, this copy gets resized to the size asked by the user.
 * @param x Size of the final amount. If less than size, it is ignored
 * @return Reduce, ordered copy of vertexSet
*/
vector<Vertex<Airport *, Airline *> *> Manager::airportsWithMostTraffic(size_t x) {
	vector<Vertex<Airport *, Airline *> *> copy = available_flights.getVertexSet();
	std::sort(copy.begin(), copy.end(), 
		[](Vertex<Airport *, Airline *> *x, Vertex<Airport *, Airline *> *y) { return x->getAdj().size() > y->getAdj().size(); });
	if (x < copy.size())
		copy.resize(x);
	return copy;
}

//ix
/*void dfs_art(Vertex<Airport, std::string> *v, Airport parent, set<Airport> &l, int &i, bool start) {
	v->setLow(i);
	v->setNum(i);
	i++;
	int children = 0;
	for (auto x : v->getAdj()) {
		auto w = x.getDest();
		if (w->getNum() == 0) {
			children++;
			dfs_art(w, v->getInfo(), l, i, false);
			v->setLow(min(v->getLow(), w->getLow()));
			if (!start && w->getLow() >= v->getNum())
				l.insert(v->getInfo());
		} else if (v->getInfo() != parent)
			v->setLow(min(v->getLow(), w->getNum()));
	}

	if (start && children > 1)
		l.insert(v->getInfo());
}*/

/**
 * Visits each node using DFS 
 * and the Tarjan algorithm to find the articulation points of the graph.
 * @note Complexity: O(V + E)
 * @param vtx Starting vector
 * @param time Integer reference keeping the order of visit
 * @param last Vector visited before. NULL if it's the first
 * @param res Saves the visit result
*/
void dfsTarjanVisit(Vertex<Airport *, int> *vtx, int &time, Vertex<Airport *, int> *last, std::set<Airport *> &res) {
	int children = 0;
	vtx->setVisited(true);
	vtx->setLow(++time);
	vtx->setNum(time);

	for (auto i : vtx->getAdj()) {
		auto w = i.getDest();
		if (!w->isVisited()) {
			children++;

			dfsTarjanVisit(w, time, vtx, res);
			vtx->setLow(min(vtx->getLow(), w->getLow()));

			if (last != NULL && w->getLow() >= vtx->getNum())
				res.insert(vtx->getInfo());
		} else if (w != last)
			vtx->setLow(min(vtx->getLow(), w->getNum()));
	}

	if (last == NULL && children > 1)
		res.insert(vtx->getInfo());
}

/**
 * Calculates what Airports are essential to the network, or by other words,
 * the articulation points of the graph representing the network
 * @note Complexity: O(V + E)
 * @return Set of Airports
*/
std::set<Airport *> Manager::essentialAirports() {

	std::set<Airport *> res;
	int time = 0;

	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setLow(0);
		i->setNum(0);
	}

	for (auto i : connections.getVertexSet())
		if (!i->isVisited())
			dfsTarjanVisit(i, time, NULL, res);
			//dfs_art(i, Airport(""), res, time, true);
	
	return res;
}

//vii
/**
 * Uses bfs to find the maximum trip that can be done 
 * without having the possibility of making it by a faster way.
 * @param vtx Starting vector
 * @param maxTrip Reference to the number keeping count of the maximum trip length
 * @param res Vector saving the pair <start, end> of the trips found.
*/
void Manager::bfsFind(Vertex<Airport *, int> *vtx, int &maxTrip, MaxTripVector &res) {
	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
	}

	list<Vertex<Airport *, int> *> queue;
	vtx->setVisited(true);
	vtx->setNum(0);
	queue.push_back(vtx);

	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (!w->isVisited()) {
				w->setVisited(true);
				w->setNum(u->getNum() + 1);
				queue.push_back(w);
				if (w->getNum() > maxTrip) {
					maxTrip = w->getNum();
					res.clear();
				}
				if (w->getNum() == maxTrip)
					res.push_back(std::make_pair(vtx->getInfo(), w->getInfo()));
			}
		}
	}
}

/**
 * Calculates the maximum trip within the network.
 * A trip can be considered maximum if it is not possible 
 * to be done faster using routes with less stops/flights.
 * @note Complexity: O(V * (V + E))
 * @return Pair of a vector of pairs <start airport, end airport> and how many flights the biggest trip takes.
*/
std::pair<MaxTripVector, int> Manager::maximumTrip() {
	MaxTripVector res;
	int maxTrip = 0;
	
	for (auto i : connections.getVertexSet()) {
		bfsFind(i, maxTrip, res);
	}

	return std::make_pair(res, maxTrip);
}
