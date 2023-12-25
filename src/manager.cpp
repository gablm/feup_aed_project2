#include "headers/manager.h"
#include <iomanip>
#include <chrono>
#include <thread>

void Manager::load_airports() {

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

		connections.addVertex(newData);
		available_flights.addVertex(newData);

	}

	file.close();
}

void Manager::test_airports() {
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
		auto res = findAirportByCode(connections, codes)->getInfo();
		std::cout 	<< "Search for " + codes + " returns: \n\n" 
					<< res.getCode() << " - " << res.getCity() 
					<< "\nLA: " << res.getLatitude() << " LO: " << res.getLongitude() << "\n\n";
	}	
}

template <class T, class W>
Vertex<T, W> *Manager::findAirportByCode(Graph<T, W> graph, std::string code) {
	return graph.findVertex(Airport(code, "", "", "", 0, 0));
}

void Manager::load_airlines() {

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

void Manager::test_airlines() {
	int count = 0;
	
	for (auto i : airlines) {
		count++;
		Airline info = i.second;
		std::cout << info.getCode() << " - " << info.getName() << "\n\n";
	}

	std::cout << "Expected: 444\nGot: " << count << "\n\n";	
}

void Manager::load_flights() {

	auto start = std::chrono::high_resolution_clock::now();

	std::string line, source, target, airline;

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
			Airport src = Airport(source);
			Airport dest = Airport(source);
			std::string empty = "";
			
			connections.addEdge(src, dest, 0, empty);
			available_flights.addEdge(src, dest, 0, company); 

		} catch (const std::exception& e) {

			std::cout << "The file airlines.csv is badly formatted. Please fix the file and reopen the program." << std::endl;
			exit(0);

		}
	}

	file.close();

	auto end = std::chrono::high_resolution_clock::now();
	std::cout 	<< "Load Time: " << std::chrono::duration<double>(end - start).count() << "s\n\n";
}

void Manager::test_flights() {

	int count = 0;
	for (auto i : available_flights.getVertexSet()) {
		count += i->getAdj().size();
	}

	std::cout << "Expected: 63832\nGot: " << count;
}