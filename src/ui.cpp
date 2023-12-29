#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>

int main() {
	UI ui;
	ui.mainMenu();
	return 0;
}

UI::UI() : manager(Manager()) {
	auto start = std::chrono::high_resolution_clock::now();
    manager.loadAirports();
	manager.loadAirlines();
	manager.loadFlights();
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
}

void UI::mainMenu() {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Lookup Tool\n"
        << "\n"
        #ifdef _linux__
        << "Welcome " << getenv("USER") << "!\n"
        #else
        << "Welcome!\n"
        #endif
		<< "Select an option:\n"
        << "\n"
        << " [1] Statistics\n"
		<< " [2] Plan a trip\n"
        << "\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "t") {
			test();
			continue;
		}
		if (str.size() > 1) {
			helpMsg("", "");
			continue;
		}
		switch (str[0] - '0')
		{
			case 1:
				statsMenu();
				break;
			case 2:
				plannerSelected();
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
		}
    }
}

void UI::statsMenu(){
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Statistics Menu\n"
        << "\n"
		<< ">> Search and display statistics\n"
		<< "\n"
		<< " [0] - Global\n"
		<< "\n"
		<< " [1] - Airports\n"
		<< " [2] - Cities\n"
		<< " [3] - Airlines\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
		if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b") {
			mainMenu();
			break;
		}
		if (str.size() > 1) {
			helpMsg("Command not found!", "help - shows all commands");
			continue;
		}
		switch (str[0] - '0') {
			case 0:
				globalStats();
				break;
			case 1:
				statsAirportSelect();
				break;
			case 2:
				statsCitySelect();
				break;
			case 3:
				statsAirlineSelect();
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
		}
    }
}

void UI::globalStats() {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Global statistics\n"
		<< "\n"
		<< "There are currently available:\n"
		<< "\n "
        << manager.airportCount() << " airports \n "
		<< manager.airlineCount() << " airlines \n "
        << manager.flightCount() << " flights\n"
		<< "\n"
		<< "To get information about the maximum possible trips input 'max'\n"
		<< "(This might take a few seconds)\n"
		<< "\n"
		<< "Loaded in " << loadtime << "s.\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
		if (str == "max"){
			showMax();
			continue;
		}
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		helpMsg("Command not found!", "help - shows all commands");
    }
}

void UI::showMax() {
	auto dataPair = manager.maximumTrip();
	int tripSize = dataPair.second;
	std::vector<std::pair<Airport, Airport>> airportVector = dataPair.first;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Global statistics - Maximum trip\n"
		<< "\n"
		<< "The biggest trips you can take require "<<tripSize<<" stops\n"
		<< "\n"
		<< "There are several places you can start and end a trip of that size. These are:"
		<< "\n";
		for(auto i : airportVector){
			std::cout << "    From "<<i.first.getCode()<<" to "<<i.second.getCode()<<"\n";
		}
		std::cout
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		helpMsg("Command not found!", "help - shows all commands");
    }
}

void UI::showAirport(std::string str) {
	bool numberInputed = false;
	int stops;
	size_t nonDigits;
	std::vector<size_t> destinationDataVector;

	std::istringstream is(str);
	std::string airportName;
	is >> airportName >> airportName;

	std::vector<size_t> dataVector1 = manager.airportStats(airportName);
	if (dataVector1[0]==__INT64_MAX__){
		helpMsg("Invalid airport name! Airport doesn't belong to the network!","Choose a valid city name.");
		return;
	}
	std::vector<size_t> dataVector2 = manager.destinationsFromAirport(airportName);

	int totalFlights = dataVector1[0];
	int totalAirlines = dataVector1[1];
	int totalAirports = dataVector2[0];
	int totalCities = dataVector2[1];
	int totalCountries = dataVector2[2];
	
	int destinationAirports;
	int destinationCities;
	int destinationCountries;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Airport statistics\n"
		<< "\n"
		<< airportName << " airport has:\n\n"
		<< " " << totalFlights << " flights departing from it\n"
		<< " from a total of " << totalAirlines << " airlines\n"
		<< " which go to " << totalAirports << " different airports\n"
		<< " in " << totalCities << " different cities\n"
		<< " in " << totalCountries <<" different countries.\n "
		<< "\n";
		if (numberInputed) {
			destinationAirports = destinationDataVector[0];
			destinationCities = destinationDataVector[1];
			destinationCountries = destinationDataVector[2];
			std::cout
			<< "From this airport, you can reach:\n\n "
			<< destinationAirports <<" airports\n "
			<< destinationCities <<" cities\n "
			<< destinationCountries<<" countries\n"
			<<"\nin the maximum of "<< stops <<" lay-overs (" << stops + 1 << " flights)\n\n";			
		}

		std::cout
		<< "Input an integer to receive the ammount of possible destinations (airports, cities and countries) in that amount of layovers or less.\n"
		<< "Inputting -1 corresponde to the max amount of lay-overs.\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		

		try{
			stops = stoi(str,&nonDigits);

			if (nonDigits != str.size()){
				throw std::invalid_argument("Invalid argument: command not found!");
			}
			if (stops < 0)
				stops = __INT32_MAX__ - 1;
			destinationDataVector = manager.reachableDestinationsFromAirport(airportName, stops + 1);
			numberInputed = true;
			continue;
			}
			catch(const std::invalid_argument& ia){
				helpMsg("Command not found!", "help - shows all commands");
			}

    }
}

void UI::showCity(std::string str) {
	std::vector<size_t> dataVector = manager.cityStats(str);

	if (dataVector[0] == __INT64_MAX__) {
		helpMsg("Invalid city name! City doesn't belong to the network!","Choose a valid city name.");
		return;
	}

	int internalAirports = dataVector[0];
	int totalFlights = dataVector[1];
	int totalAirlines = dataVector[2];
	int totalCities = dataVector[3];
	int totalCountries = dataVector[4];

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - City statistics\n"
		<< "\n"
		<< " " << str << "contains " << internalAirports << " airports,\n"
		<< " with " << totalFlights << " flights departing from it\n"
		<< " belonging to a total of " << totalAirlines << " airlines\n"
		<< " and go to " << totalCities << " different cities\n"
		<< " in " << totalCountries << " different countries."
		<< "\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		helpMsg("Command not found!", "help - shows all commands");
    }
}

void UI::showAirline(std::string str) {
	std::istringstream is(str);
	std::string airlineName;
	is >> airlineName >> airlineName;

	//{num of flights, num of departure airports, num of destination airports, num of cities, num of countries}
	std::vector<size_t> dataVector = manager.airlineStats(airlineName);
	if (dataVector[0]==__INT64_MAX__){
		helpMsg("Invalid airline name! Airline doesn't belong to the network!","Choose a valid airline name.");
		return;
	}

	int totalFlights = dataVector[0];
	int totalAirports = dataVector[1];
	int totalCities = dataVector[2];
	int totalCountries = dataVector[3];

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Airline statistics - "+ airlineName + "\n"
		<< "\n"
		<< "This airline has "<<totalFlights << " scheduled flights.\n"
		<< "Operates in:\n"
		<< totalAirports << " Airports\n"
		<< totalCities << " Cities\n"
		<< totalCountries << " Countries\n"
		<< "\n "
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		helpMsg("Command not found!", "help - shows all commands");
    }
}


void UI::helpMsg(std::string error, std::string usage) {
	CLEAR;
	std::cout << "Amadeus - Lookup Tool\n\n";
	if (error == "Invalid city name! City doesn't belong to the network!"){
		std::cout << "Invalid operation!\n"
				  << "\n Problem: " << error
				  << "\n Usage: " << usage
				  << "\n\nPress ENTER to continue...";
	}
	else if (error != "" && usage != "") {
		std::cout << "Invalid operation!\n"
				  << "\n Problem: " << error
				  << "\n Usage: " << usage
				  << "\n\nPress ENTER to continue...";
	} else {
		std::cout << "No help available for this page."
				  << "\n\nPress ENTER to continue...";
	}
	while (std::cin.get() != '\n') { }
}

void UI::test() {
	CLEAR;
	std::cout << "Amadeus - Lookup Tool\n\n";
	std::string str, x;
	getline(std::cin, str);
	if (str == "b")
		return;
	auto start = std::chrono::high_resolution_clock::now();
// -
	auto res = manager.essentialAirports();

	for (auto i : res) {
		std::cout << i.getCode() << "\n";
	}

	std::cout << "\nCount: " << res.size() << "\n";
// -
	auto end = std::chrono::high_resolution_clock::now();
	double l_time = std::chrono::duration<double>(end - start).count();
	std::cout << "\nTime elapsed: " << l_time << "s\n\nPress ENTER to continue...";
	while (std::cin.get() != '\n') { }
	test();
}

/**
 *	Test for vii

	auto res = manager.maximumTrip();
	for (auto i : res.first) {
		std::cout << i.first.getCode() << " - " << i.second.getCode() << "\n";
	}
	std::cout << "\nCount: " << res.first.size() << " | Distance: " << res.second << "\n";

	Expected -> 18 count, 12 distance

 *	Test for viii

	auto res = manager.airportsWithMostTraffic(std::atoi(str.c_str()));

	for (auto i : res) {
		std::cout << i->getInfo().getCode() << " - " << i->getAdj().size() << "\n";
	}

	Expected -> ATL - 909, ORD - 556, PEK - 526, etc.

 *	Test for ix

	auto res = manager.essentialAirports();

	for (auto i : res) {
		std::cout << i.getCode() << "\n";
	}

	std::cout << "\nCount: " << res.size() << "\n";
	
	Expected -> 309 - net / 310 - teoricas
*/