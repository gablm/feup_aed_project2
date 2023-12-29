#include "headers/manager.h"
#include <set>
#include <unordered_set>
#include <iostream>
#include <algorithm>

//i
int Manager::airportCount() {
	return connections.getVertexSet().size();
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

	for (auto i : vtx->getAdj())
		airlines.insert(i.getInfo());

	return {vtx->getAdj().size(), airlines.size()};
}

//iii
//returns a vector with {num of airports inside it, num of flights, num of airlines, num of destination cities, num of destination countries}
std::vector<size_t> Manager::cityStats(std::string code){
	auto airportList = cityAirportList[code];
	
	if (airportList.empty())
		return {__INT64_MAX__};

	std::set<string> airlineList;
	std::set<string> cityList;
	std::set<string> countryList;

	size_t totalFlights = 0;

	for (auto airportptr : airportList){
		for (auto i : airportptr->getAdj()) {
			airlineList.insert(i.getInfo().getName());
			cityList.insert(i.getDest()->getInfo().getCity());
			countryList.insert(i.getDest()->getInfo().getCountry());
		}
		totalFlights+=airportptr->getAdj().size();
	}
	return {airportList.size(),totalFlights, airlineList.size(), cityList.size(), countryList.size()};

}

//returns a vector with {num of flights, num of airports, num of cities, num of countries}
std::vector<size_t> Manager::airlineStats(std::string code){
	Airline airline = airlines[code];
	set<string> airportList;
	set<string> cityList;
	set<string> countryList;
	size_t totalFlights = 0;

	

	for (auto airport : available_flights.getVertexSet()){
		for(auto flight : airport->getAdj()){
			if (flight.getInfo()==airline){
				totalFlights++;
				airportList.emplace(airport->getInfo().getName());
				airportList.emplace(flight.getDest()->getInfo().getName());
				cityList.emplace(airport->getInfo().getCity());
				cityList.emplace(flight.getDest()->getInfo().getCity());
				countryList.emplace(airport->getInfo().getCountry());
				countryList.emplace(flight.getDest()->getInfo().getCountry());
			}
		}
	}

	return {totalFlights, airportList.size(), cityList.size(), countryList.size()};

}


//v
std::vector<size_t> Manager::destinationsFromAirport(std::string code) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};
		
	std::set<std::string> cities, countries;

	for (auto i : vtx->getAdj()) {
		auto flg = i.getDest()->getInfo();
		cities.insert(flg.getCity() + ", " + flg.getCountry());
		countries.insert(flg.getCountry());
	}

	return {vtx->getAdj().size(), cities.size(), countries.size()};
}

//vi
std::vector<size_t> Manager::reachableDestinationsFromAirport(std::string code, int x) {
	auto vtx = connections.findVertex(code);
	if (vtx == NULL)
		return {__INT64_MAX__};

	if (x <= 0)
		x = __INT32_MAX__;

	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
	}

	std::set<std::string> cities, countries;
	list<Vertex<Airport, std::string>*> queue;
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
					cities.insert(wa.getCity() + ", " + wa.getCountry());
					countries.insert(wa.getCountry());
					count++;
				}
			}
		}
	}
	
	return {count, cities.size(), countries.size()};
}

//viii
vector<Vertex<Airport, Airline>*> Manager::airportsWithMostTraffic(size_t x) {
	std::set<Airport> res;
	vector<Vertex<Airport, Airline>*> copy = available_flights.getVertexSet();
	std::sort(copy.begin(), copy.end(), 
		[](Vertex<Airport, Airline> *x, Vertex<Airport, Airline> *y) { return x->getAdj().size() > y->getAdj().size(); });
	if (x < copy.size())
		copy.resize(x);
	return copy;
}

//ix
void dfs_art(Vertex<Airport, std::string> *v, Airport parent, set<Airport> &l, int &i, bool start) {
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
}

void dfsTarjanVisit(Vertex<Airport, std::string> *vtx, int &time, Vertex<Airport, std::string> *last, std::set<Airport> &res) {
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

std::set<Airport> Manager::essentialAirports() {

	std::set<Airport> res;
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
void Manager::bfsFind(Vertex<Airport, std::string> *vtx, int &maxTrip, MaxTripVector &res) {
	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
	}

	list<Vertex<Airport, std::string>*> queue;
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

std::pair<MaxTripVector, int> Manager::maximumTrip() {
	MaxTripVector res;
	int maxTrip = 0;
	
	for (auto i : connections.getVertexSet()) {
		bfsFind(i, maxTrip, res);
	}

	return std::make_pair(res, maxTrip);
}
