#include "headers/manager.h"
#include <algorithm>
#include <set>
#include <sstream>

CitySearchResult Manager::searchCity(std::string type, std::string query) {
	CitySearchResult res;
	for (auto i : cityAirportList) {
		std::string w = i.first;
		std::string c = i.second.front()->getInfo().getCountry();
		if (type == "city" && w.find(query) != w.npos)
			res.push_back(std::make_pair(w, c));
		if (type == "country" && c.find(query) != c.npos)
			res.push_back(std::make_pair(w, c));
	}
	return res;
}

std::vector<Airport> Manager::searchAirport(std::string type, std::string query) {
	std::vector<Airport> res;

	auto add = [&res, query](std::string tmp, Airport w) {
		if (tmp.find(query) != tmp.npos) {
			res.push_back(w);
		}
	};


	for (auto i : connections.getVertexSet()) {
		Airport w = i->getInfo();
		if (type == "code")
			add(w.getCode(), w);
		if (type == "name")
			add(w.getName(), w);
		if (type == "city")
			add(w.getName(), w);
		if (type == "country")
			add(w.getName(), w);
		if (type == "coords")
			res.push_back(w);
	}

	istringstream in(query);
	std::string xs, ys;
	in >> xs >> ys;
	double x = atoi(xs.c_str()), y = atoi(ys.c_str());

	if (type == "coords")
		std::sort(res.begin(), res.end(), 
			[x, y, this](const Airport &a, const Airport &b) { 
				return distance(x, y, a.getLatitude(), a.getLongitude()) < distance(x, y, b.getLatitude(), b.getLongitude());  
			});
			
	return res;
}

