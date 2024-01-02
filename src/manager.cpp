#include "headers/manager.h"
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

Manager::~Manager() {
	for (auto i : airlines)
		delete i.second;
	for (auto i : connections.getVertexSet()) {
		delete i->getInfo();
		delete i;
	}
	for (auto i : available_flights.getVertexSet())
		delete i;
}

/**
 * Loads the airports into the graphs.
 * As our graph class has an addicional map to improve access times to the graph,
 * the complexity is equal to number of airports * (log V)
 * @note Complexity: O(N * log V)
*/
void Manager::loadAirports() {

	std::string line, code, name, city, country, latitudeStr, longitudeStr;
	double latitude, longitude;

	ifstream file;
	file.open("./data/airports.csv");

	if (file.fail()) {
		std::cout << "Invalid File - Please insert a valid airports.csv in the data folder.";
		exit(0);
	}

	getline(file, line);

	while (true) {

		if (file.eof())
			break;

		getline(file, line);

		if (line.size() < 5)
			continue;

		istringstream ss(line);

		try {

			std::getline(ss, code, ',');
			std::getline(ss, name, ',');
			std::getline(ss, city, ',');
			std::getline(ss, country, ',');
			std::getline(ss, latitudeStr, ',');
			latitude = stod(latitudeStr);
			std::getline(ss, longitudeStr, '\r');
			longitude = stod(longitudeStr);

		} catch (const std::exception& e) {

			std::cout << "The file airports.csv is badly formatted. Please fix the file and reopen the program." << std::endl;
			exit(0);

		}

		Airport *newData = new Airport(code, name, city, country, latitude, longitude);

		connections.addVertex(newData, code);
		if (available_flights.addVertex(newData, code)){
			Vertex<Airport *, Airline *> *vertexPointer = available_flights.findVertex(code);
        	cityAirportList[city + ", " + country].push_back(vertexPointer);
		}
	}

	file.close();
}

/**
 * Loads the airlines into a map.
 * A map provides fast access to the airlines by their code name.
 * @note Complexity: O(N * log N)
*/
void Manager::loadAirlines() {

	std::string line, code, name, callsign, country;

	ifstream file;
	file.open("./data/airlines.csv");

	if (file.fail()) {
		std::cout << "Invalid File - Please insert a valid airlines.csv in the data folder.";
		exit(0);
	}

	getline(file, line);

	while (true) {

		if (file.eof())
			break;

		getline(file, line);

		if (line.size() < 5)
			continue;

		istringstream ss(line);

		try {

			std::getline(ss, code, ',');
			std::getline(ss, name, ',');
			std::getline(ss, callsign, ',');
			std::getline(ss, country, '\r');

		} catch (const std::exception& e) {

			std::cout << "The file airlines.csv is badly formatted. Please fix the file and reopen the program." << std::endl;
			exit(0);

		}

		airlines[code] = new Airline(code, name, callsign, country);
	}

	file.close();
}

/**
 * Used to calculate the distance between two coordinates,
 * using the Haversine formula.
 * As the sqrt function is used, the complexity is O(log N)
 * @note Complexity: O(log N)
 * @return Distance in kilometers
*/
double Manager::distance(double la1, double lo1, double la2, double lo2) {
	double la = (la2 - la1) * M_PI / 180.0;
	double lo = (lo2 - lo1) * M_PI / 180.0;

	la1 *= M_PI / 180.0;
	la2 *= M_PI / 180.0;

	double formula = pow(sin(la / 2), 2) + pow(sin(lo / 2), 2) * cos(la1) * cos(la2);
	return abs(6371 * 2 * asin(sqrt(formula)));
}

/**
 * Loads flights into the graphs.
 * Finds the Vertex of each airport and adds an Edge 
 * between them with an Airline field inside of them.
 * @note Complexity: O(N * log V)
*/
void Manager::loadFlights() {

	std::string line, source, oldSource, target, airline;

	Vertex<Airport *, int> *cn_src = nullptr;
	std::vector<Edge<Airport *, int>> cn_edge;

	Vertex<Airport *, Airline *> *av_src = nullptr;
	std::vector<Edge<Airport *, Airline *>> av_edge;
	

	ifstream file;
	file.open("./data/flights.csv");

	if (file.fail()) {
		std::cout << "Invalid File - Please insert a valid airlines.csv in the data folder.";
		exit(0);
	}

	getline(file, line);

	while (true) {

		if (file.eof())
			break;

		getline(file, line);

		if (line.size() < 5)
			continue;

		istringstream ss(line);

		try {

			std::getline(ss, source, ',');
			std::getline(ss, target, ',');
			std::getline(ss, airline, '\r');

			Airline *company = airlines[airline];

			if (av_src == nullptr || cn_src == nullptr || oldSource != source) {
				if (av_src != nullptr && cn_src != nullptr) {
					av_src->setAdj(av_edge);
					cn_src->setAdj(cn_edge);
				}
				av_src = available_flights.findVertex(source);
				av_edge = av_src->getAdj();
				cn_src = connections.findVertex(source);
				cn_edge = cn_src->getAdj();
				oldSource = source;
			}

			auto cn_dst = connections.findVertex(target);

			Airport *src = cn_src->getInfo();
			Airport *dst = cn_dst->getInfo();

			double dist = distance(src->getLatitude(), src->getLongitude(), dst->getLatitude(), dst->getLongitude());

			auto av_dst = available_flights.findVertex(target);
			av_edge.push_back(Edge<Airport *, Airline *>(av_dst, dist, company));

			int empty = 0;
			Edge<Airport *, int> edg(cn_dst, dist, empty);
			if (std::find(cn_edge.begin(), cn_edge.end(), edg) == cn_edge.end())
				cn_edge.push_back(edg);

		} catch (const std::exception& e) {

			std::cout << "The file airlines.csv is badly formatted. Please fix the file and reopen the program." << std::endl;
			exit(0);

		}
	}

	file.close();

	if (av_src != nullptr && cn_src != nullptr) {
		av_src->setAdj(av_edge);
		cn_src->setAdj(cn_edge);
	}
}
