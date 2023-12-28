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
			case 1:
				plannerCitySelect();
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
				origin = lst;
				plannerSelected();
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
	vector<Airport> res;

	if (vtx != NULL) {
		res.push_back(vtx->getInfo());
		return res;
	}
	
	for (auto i : conns.getVertexSet()) {
		auto w = i->getInfo();
		if (str_find(w.getCode(), query) || str_find(w.getName(), query))
			res.push_back(w);
	}

	return res;
}

void UI::plannerCitySelect() {
	std::set<std::string> lst;
	size_t count = 0;
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting source location\n";
		if (!lst.empty()) {
			std::cout << "\nThe search has returned:\n\n";
			auto iter = lst.begin();
			std::advance(iter, count);
			for (size_t i = count; i < min(count + 10, lst.size()); i++)
				std::cout << i << ". " << *(iter++) << "\n";
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << " total\n";
		}
		std::cout
        << "\n"
		<< (lst.empty() ? "" : "next - Next page | back - Last page\n")
		<< "b - Back | q - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (lst.empty() ? ":\n" : " or select a city using a number:\n")
        << "$> ";
        std::string str;
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

		size_t num = atol(str.c_str());
		if (str == "0" || num != 0) {
			auto it = lst.begin();
			std::advance(it, num);
			vector<Airport> res;
			for (auto i : manager.getConnections().getVertexSet()) {
				auto w = i->getInfo();
				if (w.getCity() + ", " + w.getCountry() == *it)
					res.push_back(w);
			}
			origin = res;
			plannerSelected();
			break;
		}

		if (str.size() > 1) {
			count = 0;
			lst = searchCity(str);
			if (lst.size() == 1) {
				vector<Airport> res;
				for (auto i : manager.getConnections().getVertexSet()) {
					auto w = i->getInfo();
					if (w.getCity() + ", " + w.getCountry() == *lst.begin())
						res.push_back(w);
				}
				origin = res;
				plannerSelected();
				break;
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

set<std::string> UI::searchCity(std::string query) {	
	auto conns = manager.getConnections();
	set<std::string> res;
	std::transform(query.begin(), query.end(), query.begin(), ::tolower);
	for (auto i : conns.getVertexSet()) {
		auto w = i->getInfo();
		std::string fullName = w.getCity() + ", " + w.getCountry();
		if (str_find(w.getCity(), query) || str_find(w.getCountry(), query))
			res.insert(fullName);
		std::string fl2 = fullName;
		std::transform(fullName.begin(), fullName.end(), fl2.begin(), ::tolower);
		if (fl2 == query) {
			res.clear();
			res.insert(fullName);
			return res;
		}
	}

	return res;
}

void UI::plannerSelected() {
	CLEAR;
	std::cout << "Origin Selected:\n\n";
	for (auto i : origin) {
		std::cout << i.getCode() << " - " << i.getCity() << ", " << i.getCountry() << "\n";
	}
	while (std::cin.get() != '\n') { }
}