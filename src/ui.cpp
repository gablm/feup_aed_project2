#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <set>

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
		<< "Select an option [0-3]:\n"
        << "\n"
        << " [0] Global statistics\n"
		<< " [1] ??\n"
		<< " [2] ??\n"
        << "\n"
		<< " [3] Plan a trip\n"
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
			case 0:
				statsMenu();
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
        << "\n"
		<< "For the global statistics input 'global'\n"
        << "\n"
		<< "For the statistics about a specific airport input 'airport (airport code)'\n"
		<< "For the statistics about a specific city input 'city (city code)'\n"
		<< "For the statistics about a specific airline input 'airline (airline code)'\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
		if (str == "global"){
			globalStats();
			continue;
		}
		else if (str.substr(0,8) == "airport "){
			showAirport(str);
		}
		else if (str.substr(0,5) == "city "){
			showCity(str);
		}
		else if (str.substr(0,8) == "airline "){
			showAirline(str);
		}
        else if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		else if (str == "B" || str == "b")
			break;
		else {
			helpMsg("Command not found!", "help - shows all commands");
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
		<< "Loaded in " << loadtime << "s.\n"
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

void UI::showAirport(std::string str){
	str = str;
	return;
}

void UI::showCity(std::string str){
	str = str;
	return;
}

void UI::showAirline(std::string str){
	str = str;
	return;
}


void UI::helpMsg(std::string error, std::string usage) {
	CLEAR;
	std::cout << "Amadeus - Lookup Tool\n\n";
	if (error != "" && usage != "") {
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
	std::string str;
	getline(std::cin, str);
	if (str == "b")
		return;
// -
	auto res = manager.maximumTrip();
	//std::set<Airport> res = manager.essentialAirports();
	for (auto i : res) {
		std::cout << i.first.getCode() << " - " << i.second.getCode() << "\n";
	}

	manager.minPath();

	std::cout << "Count: " << res.size() << "\n"; 
// -
	std::cout << "\n\nPress ENTER to continue...";
	while (std::cin.get() != '\n') { }
	test();
}