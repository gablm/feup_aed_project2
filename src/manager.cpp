#include "headers/manager.h"
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

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

		Airport newData = Airport(code, name, city, country, latitude, longitude);

		connections.addVertex(newData, code);
		if (available_flights.addVertex(newData, code)){
			Vertex<Airport, Airline> *vertexPointer = available_flights.findVertex(code);
        	cityAirportList[city].push_back(vertexPointer);
		}
		airports[code] = newData;
	}

	file.close();
}

void Manager::testAirports() {
	std::cout << std::setprecision(6) << std::fixed;
	int count = 0;
	
	for (auto i : connections.getVertexSet()) {
		count++;
		Airport info = i->getInfo();
		std::cout 	<< info.getCode() << " - " << info.getCity() 
					<< "\nLA: " << info.getLatitude() << " LO: " << info.getLongitude() << "\n\n";
	}

	std::cout << "Expected: 3019\nGot: " << count << "\n\n";

	for (std::string codes : {"CGD", "OPO", "CIA"}) {
		auto res = connections.findVertex(codes)->getInfo();
		std::cout 	<< "Search for " + codes + " returns: \n\n" 
					<< res.getCode() << " - " << res.getCity() 
					<< "\nLA: " << res.getLatitude() << " LO: " << res.getLongitude() << "\n\n";
	}



}

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

		airlines[code] = Airline(code, name, callsign, country);
	}

	file.close();
}

void Manager::testAirlines() {
	int count = 0;
	
	for (auto i : airlines) {
		count++;
		Airline info = i.second;
		std::cout << info.getCode() << " - " << info.getName() << "\n\n";
	}

	std::cout << "Expected: 444\nGot: " << count << "\n\n";	
}

double distance(double la1, double lo1, double la2, double lo2) {
	double la = (la2 - la1) * M_PI / 180.0;
	double lo = (lo2 - lo1) * M_PI / 180.0;

	la1 *= M_PI / 180.0;
	la2 *= M_PI / 180.0;

	double formula = pow(sin(la / 2), 2) + pow(sin(lo / 2), 2) * cos(la1) * cos(la2);
	return 6371 * 2 * asin(sqrt(formula));
}

void Manager::loadFlights() {

	std::string line, source, oldSource, target, airline;

	Vertex<Airport, std::string> *cn_src = nullptr;
	std::vector<Edge<Airport, std::string>> cn_edge;

	Vertex<Airport, Airline> *av_src = nullptr;
	std::vector<Edge<Airport, Airline>> av_edge;
	

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

			Airline company = airlines[airline];
			std::string empty;

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

			Airport src = cn_src->getInfo();
			Airport dst = airports[target];

			double dist = distance(src.getLatitude(), src.getLongitude(), dst.getLatitude(), dst.getLongitude());

			auto av_dst = available_flights.findVertex(target);
			av_edge.push_back(Edge<Airport, Airline>(av_dst, dist, company));

			auto cn_dst = connections.findVertex(target);
			Edge <Airport, std::string> edg(cn_dst, dist, empty);
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

void Manager::testFlights() {

	int count = 0;
	for (auto i : available_flights.getVertexSet()) {
		count += i->getAdj().size();
	}

	std::cout << "Expected: 63832\nGot: " << count << "\n";

	auto i = connections.findVertex("DCY");

	std::cout 	<< "Expected: 1\nGot: " << i->getAdj().size() << "\n"
				<< i->getAdj().front().getDest()->getInfo().getCode() << "\n";
}