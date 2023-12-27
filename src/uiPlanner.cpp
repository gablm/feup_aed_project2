#include "headers/ui.h"
#include <iostream>
#include <algorithm>

void UI::plannerMenu() {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< "How do you want to search the source location?\n"
		<< "\n"
		<< " [0] Airport code/name\n"
		<< " [1] City name\n"
		<< " [2] Coordinates\n"
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
		if (str.size() > 1) {
			helpMsg("Command not found!", "help - shows all commands");
			continue;
		}
		switch (str[0] - '0') {
			case 0:
				plannerAirportSelect();
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
		}
    }
}

void UI::plannerAirportSelect() {
	std::vector<Airport> lst;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Source location\n";
		if (!lst.empty()) {
			std::cout << "\nYour previous search has returned:\n";
			for (auto i : lst)
				std::cout << i.getCode() << " - " << i.getName() << "\n";
		}
		std::cout
        << "\n"
		<< "[B] Back\n"
		<< "[Q] Exit\n"
		<< "\n"
		<< "Please enter the Airport code" << (lst.empty() ? " or a name to search:\n" : ":\n")
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;
		if (str.size() > 1) {
			lst = searchAirport(str);
			if (lst.size() == 1) {

				break;
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

bool str_find(const std::string &one, const std::string &two) {
	auto it = std::search(one.begin(), one.end(), two.begin(), two.end(),
    	[](unsigned char a, unsigned char b) { return std::toupper(a) == std::toupper(b);}
  		);
  	return (it != one.end() );
}

vector<Airport> UI::searchAirport(std::string query) {
	auto conns = manager.getConnections();
	std::transform(query.begin(), query.end(), query.begin(), ::toupper);
	auto vtx = conns.findVertex(query);
	if (vtx != NULL)
		return { vtx->getInfo() };
	
	vector<Airport> res;
	for (auto i : conns.getVertexSet()) {
		auto w = i->getInfo();
		if (str_find(w.getCode(), query) || str_find(w.getName(), query))
			res.push_back(w);
	}

	return res;
}