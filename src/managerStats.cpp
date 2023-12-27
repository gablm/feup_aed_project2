#include "headers/manager.h"
#include <set>
#include <unordered_set>
#include <iostream>
#include <algorithm>

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

//ix
void dfs_art(Vertex<Airport, std::string> *v, vector<Airport> &s, set<Airport> &l, int &i, bool start) {
	v->setLow(i);
	v->setNum(i);
	s.push_back(v->getInfo());
	i++;
	int children = 0;
	for (auto x : v->getAdj()) {
		auto w = x.getDest();
		if (w->getNum() == 0) {
			children++;
			dfs_art(w, s, l, i, false);
			v->setLow(min(v->getLow(), w->getLow()));
			if (!start && w->getLow() >= v->getNum())
				l.insert(v->getInfo());
		} else if (std::find(s.begin(), s.end(), w->getInfo()) != s.end())
			v->setLow(min(v->getLow(), w->getNum()));
	}
	if (start && children > 1)
		l.insert(v->getInfo());

	s.pop_back();
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
	std::vector<Airport> s;
	int time = 0;

	for (auto i : connections.getVertexSet()) {
		i->setVisited(false);
		i->setLow(0);
		i->setNum(0);
	}

	for (auto i : connections.getVertexSet())
		if (!i->isVisited()) {
			dfsTarjanVisit(i, time, NULL, res);
			//dfs_art(i, s, res, time, true);
		}

	return res;
}

void Manager::printConns() {
	for (auto i : connections.getVertexSet()) {
		std::cout << i->getInfo().getCode() << ":\n";
		for (auto j : i->getAdj()) {
			auto w = j.getDest();
			std::cout << w->getInfo().getCode() << " ";
		}
		std::cout << "\n\n";
	}
}