#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

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
			case 2:
				plannerCoordsSelect();
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
		}
    }
}

void UI::plannerAirportSelect() {
	std::vector<Airport> lst;
	size_t count = 0;
	std::string str;

	while (1)
    { 
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting source location by Airport\n";
		if (!lst.empty()) {
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << i << ". " << w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ")\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		std::cout
        << "\n"
		<< (lst.empty() ? "" : "next - Next page | back - Last page\n")
		<< "b - Back | q - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (lst.empty() ? ":\n" : " or select a city using a number:\n")
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

		size_t num = atol(str.c_str());
		if (str == "0" || num != 0) {
			if (num >= lst.size()) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			vector<Airport> res;
			res.push_back(lst[num]);
			origin = res;
			plannerSelected();
			break;
		}
		if (str.size() > 1) {
			count = 0;
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
	std::string str;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting source location by City\n";
		if (!lst.empty()) {
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n";
			auto iter = lst.begin();
			std::advance(iter, count);
			for (size_t i = count; i < min(count + 10, lst.size()); i++)
				std::cout << i << ". " << *(iter++) << "\n";
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		std::cout
        << "\n"
		<< (lst.empty() ? "" : "next - Next page | back - Last page\n")
		<< "b - Back | q - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (lst.empty() ? ":\n" : " or select a city using a number:\n")
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

		size_t num = atol(str.c_str());
		if (str == "0" || num != 0) {
			if (num >= lst.size()) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
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

void UI::plannerCoordsSelect() {
	std::vector<Airport> lst;
	std::string str;
	size_t count = 0;
	double lat = 0, lon = 0;

	while (1)
    { 
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting source location by coordinates\n";
		if (!lst.empty()) {
			std::cout << "\nThe closest airports to (" << lat << " La, " << lon << " Lo) are:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << std::fixed << std::setprecision(2) << Manager::distance(lat, lon, w.getLatitude(), w.getLongitude()) << "\tkm | "
					<< w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ") \n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		std::cout
        << "\n"
		<< (lst.empty() ? "" : "next - Next page | back - Last page\n")
		<< "b - Back | q - Exit\n"
		<< "\n"
		<< "Please enter coordinates to search" << (lst.empty() ? ":\n" : " or enter the number of closest airports to consider:\n")
        << "$> ";

		getline(std::cin, str);

		std::istringstream in(str);
		std::string la, lo;
		in >> la >> lo;

		try {
			lat = stod(la);
			lon = stod(lo);
		} catch (const exception &e) {}

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
		if (str.find(' ') == str.npos && num != 0) {
			if (num > lst.size()) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			lst.resize(num);
			origin = lst;
			plannerSelected();
			break;
		}
		if (str.size() > 1) {
			count = 0;
			lst = searchCoords(lat, lon);
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

std::vector<Airport> UI::searchCoords(double lat, double lon) {
	std::vector<Airport> res;

	for (auto i : manager.getConnections().getVertexSet()) {
		auto w = i->getInfo();
		double dist = Manager::distance(lat, lon, w.getLatitude(), w.getLongitude());
		if (dist < 5) {
			res.clear();
			res.push_back(w);
			return res;
		}
		if (dist <= 100)
			res.push_back(w);
	}
	std::sort(res.begin(), res.end(), [lat, lon](Airport &a, Airport &b) {
		return Manager::distance(lat, lon, a.getLatitude(), a.getLongitude()) < 
			Manager::distance(lat, lon, b.getLatitude(), b.getLongitude());
	});	
	return res;
}