#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>
#include <thread>
#include <chrono>

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
		std::getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
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
		std::getline(std::cin, str);
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
		<< "Loaded in " << loadtime << "s.\n"
		<< "\n"
		<< "To get information about the maximum possible trips input 'max'\n"
		<< "(This might take a few seconds)\n"
		<< "\n"
		<< "To get information about the top-K airports with most air traffic input 'top (integer)'\n"
		<< "\n"
		<< "To show the listing of essential airports, use 'essential'\n"
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);
		if (str.substr(0, 3) == "top") {
			if (str.size() <= 4) {
				helpMsg("Please insert an valid integer!", "top [integer >= 1]");
				continue;
			}
			int num = atoi(str.substr(4).c_str());
			if (num <= 0 && num != -1) {
				helpMsg("Please insert an valid integer!", "top [integer >= 1/integer = -1 for max]");
				continue;
			}
			showTop(num);
			continue;
		}
		if (str == "max"){
			showMax();
			continue;
		}
		if (str == "essential") {
			showEssential();
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

void wait(Manager *manager, std::pair<MaxTripVector, int> *res, bool *loading) {
	*res = manager->maximumTrip();
	*loading = false;
}

void UI::showMax() {
	bool loading = true;
	std::pair<MaxTripVector, int> dataPair;

	std::thread p = std::thread(wait, &manager, &dataPair, &loading);

	int count = 0;
	while (loading) {
		CLEAR;
		std::cout << "Amadeus - Global statistics\n"
				  << "\n"
				  << "Maximum trip is currently being calculated.\n"
				  << "Please wait" << std::string(count, '.') << "\n";
		count = count == 3 ? 0 : count + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	p.join();

	int tripSize = dataPair.second;
	std::vector<std::pair<Airport, Airport>> airportVector = dataPair.first;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Global statistics - Maximum trip\n"
		<< "\n"
		<< "The biggest trip you can take requires " << tripSize <<" stops.\n"
		//<< "\n"
		<< "There are several places you can start and end a trip of that size, those being:\n"
		<< "\n";
		for(auto i : airportVector){
			std::cout << " " << i.first.getCode() << " to " << i.second.getCode() << "\n";
		}
		std::cout
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);
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
	int stops = -20;
	std::vector<size_t> destinationDataVector;

	std::vector<size_t> dataVector1 = manager.airportStats(str);
	if (dataVector1[0] == __INT64_MAX__) {
		helpMsg("Invalid airport name! Airport doesn't belong to the network!","Choose a valid city name.");
		return;
	}
	std::vector<size_t> dataVector2 = manager.destinationsFromAirport(str);

	size_t totalFlights = dataVector1[0];
	size_t totalAirlines = dataVector1[1];
	size_t totalAirports = dataVector2[0];
	size_t totalCities = dataVector2[1];
	size_t totalCountries = dataVector2[2];
	
	size_t destinationAirports = 0;
	size_t destinationCities = 0;
	size_t destinationCountries = 0;

	Airport airport = manager.getConnections().findVertex(str)->getInfo();

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Airport statistics\n"
		<< "\n"
		<< airport.getCode() << " or " << airport.getName() << " is an airport in " << airport.getCity() << ", " << airport.getCountry() << "\n"
		<< "\n"
		<< "This airport has a total of " << totalFlights << " flights departing from it, which can reach:\n"
		<< " \n"
		<< " " << totalAirports << " Airports\n"
		<< " " << totalCities << " Cities\n"
		<< " " << totalCountries << " Countries\n "
		<< "\n"
		<< "The flights are operated by a total of " << totalAirlines << " distinct airlines.\n"
		<< "\n"
		<< "To check the number of possible destinations in a maximum of a certain amount of layovers or less, input a number.\n"
		<< "Note: 0 will show direct flights, -1 will show the results with an infinite amount of lay-overs.\n";

		if (numberInputed) {
			destinationAirports = destinationDataVector[0];
			destinationCities = destinationDataVector[1];
			destinationCountries = destinationDataVector[2];

			if (stops < 0)
				stops = __INT32_MAX__ - 1;

			if (stops < __INT32_MAX__ - 1)
				std::cout << "\nIn the maximum of " << stops << " lay-overs (" << stops + 1 << " flights), you can reach:\n\n";
			else
				std::cout << "\nIn the maximum of infinite lay-overs (infinite flights), you can reach:\n\n";

			std::cout
			<< " " << destinationAirports << " airports\n"
			<< " " << destinationCities << " cities\n"
			<< " " << destinationCountries << " countries\n";		
		}

        std::cout
		<< "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		stops = atoi(str.c_str());
		if (str == "0" || stops >= -1) {

			if (stops < 0)
				stops = __INT32_MAX__ - 1;

			destinationDataVector = manager.reachableDestinationsFromAirport(airport.getCode(), stops + 1);
			numberInputed = true;
			continue;
		}
		helpMsg("Command not found!", "help - shows all commands");
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

	auto city = str.substr(0, str.find(", "));
	auto country = str.substr(str.find(", ") + 2);

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - City statistics\n"
		<< "\n"
		<< city << " is a city in " << country << " with a total of " << internalAirports << " airport" << (internalAirports != 1 ? "s" : "") << ".\n"
		<< "\n"
		<< "The " << totalFlights << " flights departing from " << (internalAirports != 1 ? "them" : "it") << " can reach:\n"
		<< "\n"
		<< " " << totalCities << " Cities\n"
		<< " " << totalCountries << " Countries\n"
        << "\n"
		<< "The flights are operated by a total of " << totalAirlines << " distinct airlines.\n"
		<< "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);
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
	//{num of flights, num of departure airports, num of destination airports, num of cities, num of countries}
	std::vector<size_t> dataVector = manager.airlineStats(str);
	if (dataVector[0] == __INT64_MAX__) {
		helpMsg("Invalid airline name! Airline doesn't belong to the network!","Choose a valid airline name.");
		return;
	}

	int totalFlights = dataVector[0];
	int totalAirports = dataVector[1];
	int totalCities = dataVector[2];
	int totalCountries = dataVector[3];

	Airline airline = manager.getAirlines()[str];

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Airline statistics\n"
		<< "\n"
		<< str << " or " << airline.getName() << " is an airline from " << airline.getCountry() <<".\n"
		<< "\n"
		<< "This airline has " << totalFlights << " scheduled flights and operates in:\n"
		<< "\n "
		<< totalAirports << " Airports\n "
		<< totalCities << " Cities\n "
		<< totalCountries << " Countries\n"
		<< "\n"
		<< "Its callsign is \"" << airline.getCallSign() << "\".\n"
		<< "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);
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

void UI::showEssential() {
	std::set<Airport> lst = manager.essentialAirports();
	size_t count = 0;
	std::string str;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Essential airports\n\n";
		if (!lst.empty()) {
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto it = lst.begin();
				std::advance(it, i);
				auto w = *it;
				std::cout << i << ". " << w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ")\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		} else
			std::cout << " There are no essential airports\nThis is most likely an error. Please go back and try again!\n";

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< (lst.empty() ? "Use" : "Select a airport by its number or use") << " one of the commands above\n"
		<< "\n"
        << "$> ";

		getline(std::cin, str);

        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;

		if (str == "next" && !lst.empty()) {
			count = count + 10 < lst.size() + lst.size() % 10 ? count + 10 : count;
			continue;
		}
		if (str == "back" && !lst.empty()) {
			count = count < 10 ? 0 : count - 10;
			continue;
		}
		if (str.substr(0, 4) == "page") {
			if (str.size() <= 5 || lst.empty()) {
				helpMsg("There is no page to change to!", "page [num] if there is results");
				continue;
			}
			int page = atoi(str.substr(5).c_str());
			if (page <= 0 || page > totalPages) {
				helpMsg("That page does not exist!", "page [num] if there is results");
				continue;
			}
			count = (page - 1) * 10;
			continue;
		}
		size_t num = atol(str.c_str());
		if (str == "0" || num != 0) {
			if (num >= lst.size() || lst.empty()) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			auto it = lst.begin();
			std::advance(it, num);
			showAirport((*it).getCode());
			continue;
		}
		helpMsg("Invalid command!", "[next/back/b/q/(integer)]");
    }
}


void UI::showTop(int x) {
	auto lst = manager.airportsWithMostTraffic(x);
	size_t count = 0;
	std::string str;

	while (1)
    {
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Airports with most traffic\n\n";
		if (!lst.empty()) {
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i]->getInfo();
				std::cout << i + 1 << ". " <<  std::fixed << lst[i]->getAdj().size() << " flights | " << w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ")\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		} else
			std::cout << " There are airports.\nThis is most likely an error. Please go back and try again!\n";

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[top (integer)] - Increase the number of results shown\n"
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< (lst.empty() ? "Use" : "Select a airport by its number or use") << " one of the commands above\n"
		<< "\n"
        << "$> ";

		getline(std::cin, str);

        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;

		if (str == "next" && !lst.empty()) {
			count = count + 10 < lst.size() + lst.size() % 10 ? count + 10 : count;
			continue;
		}
		if (str == "back" && !lst.empty()) {
			count = count < 10 ? 0 : count - 10;
			continue;
		}
		if (str.substr(0, 4) == "page") {
			if (str.size() <= 5 || lst.empty()) {
				helpMsg("There is no page to change to!", "page [num] if there is results");
				continue;
			}
			int page = atoi(str.substr(5).c_str());
			if (page <= 0 || page > totalPages) {
				helpMsg("That page does not exist!", "page [num] if there is results");
				continue;
			}
			count = (page - 1) * 10;
			continue;
		}
		if (str.substr(0, 3) == "top") {
			if (str.size() <= 4 || lst.empty()) {
				helpMsg("There is no page to change to!", "top [num]");
				continue;
			}
			int top = atoi(str.substr(4).c_str());
			if (top <= 0) {
				helpMsg("This number is too small", "top [num]");
				continue;
			}
			lst = manager.airportsWithMostTraffic(top);
			continue;
		}
		size_t num = atol(str.c_str());
		if (num != 0) {
			if (num > lst.size() || lst.empty()) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			showAirport(lst[num - 1]->getInfo().getCode());
			continue;
		}
		helpMsg("Invalid command!", "[next/back/b/q/(integer)]");
    }
}