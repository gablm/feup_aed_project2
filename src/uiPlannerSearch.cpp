#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

bool str_find(const std::string &one, const std::string &two) {
	auto it = std::search(one.begin(), one.end(), two.begin(), two.end(),
    	[](unsigned char a, unsigned char b) { return std::toupper(a) == std::toupper(b);}
  		);
  	return (it != one.end() );
}

void UI::plannerMenu(bool in) {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< "How do you want to search the " << (in ? "source" : "destination") << " location?\n"
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
				plannerAirportSelect(in);
				break;
			case 1:
				plannerCitySelect(in);
				break;
			case 2:
				plannerCoordsSelect(in);
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
		}
    }
}

void UI::plannerAirportSelect(bool in) {
	std::vector<Airport> lst;
	size_t count = 0;
	std::string str;
	std::string search;
	int type;

	while (1)
    {
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0].getCode() == "NULL")
			type = 2;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting " << (in ? "source" : "destination") << " location by Airport\n";
		if (type == 1) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << i << ". " << w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ")\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		if (type == 2) {
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n(nothing)\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (type != 1 ? ":\n" : " or select a airport using a number:\n")
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
			if (num >= lst.size() || type == 2) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			vector<Airport> res;
			res.push_back(lst[num]);
			if (in) {
				origin = res;
				destination.clear();
			} else
				destination = res;
			plannerSelected();
			break;
		}
		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchInAirport(str);
			if (lst.size() == 1 && lst[0].getCode() != "NULL") {
				if (in) {
					origin = lst;
					destination.clear();
				} else
					destination = lst;
				plannerSelected();
				break;
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

vector<Airport> UI::searchInAirport(std::string query) {
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

	if (res.empty())
		res.push_back(Airport("NULL"));

	return res;
}

vector<Airport> UI::searchOutAirport(std::string query) {
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

	if (res.empty())
		res.push_back(Airport("NULL"));

	return res;
}

void UI::plannerCitySelect(bool in) {
	std::set<std::string> lst;
	size_t count = 0;
	std::string str;
	std::string search;
	int type;

	while (1)
    {
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && *lst.begin() == "NULL")
			type = 2;
        
		CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting " << (in ? "source" : "destination") << " location by City\n";
		if (type == 1) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n";
			auto iter = lst.begin();
			std::advance(iter, count);
			for (size_t i = count; i < min(count + 10, lst.size()); i++)
				std::cout << i << ". " << *(iter++) << "\n";
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		if (type == 2) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n(nothing)\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (type != 1 ? ":\n" : " or select a city using a number:\n")
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
			if (num >= lst.size() || type == 2) {
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
			if (in) {
				origin = res;
				destination.clear();
			} else
				destination = res;
			plannerSelected();
			break;
		}

		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchInCity(str);
			if (lst.size() == 1 && *lst.begin() != "NULL") {
				vector<Airport> res;
				for (auto i : manager.getConnections().getVertexSet()) {
					auto w = i->getInfo();
					if (w.getCity() + ", " + w.getCountry() == *lst.begin())
						res.push_back(w);
				}
				if (in) {
					origin = res;
					destination.clear();
				} else
					destination = res;
				plannerSelected();
				break;
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

set<std::string> UI::searchInCity(std::string query) {	
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

	if (res.empty())
		res.insert("NULL");

	return res;
}

set<std::string> UI::searchOutCity(std::string query) {	
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

	if (res.empty())
		res.insert("NULL");

	return res;
}

void UI::plannerCoordsSelect(bool mode) {
	std::vector<Airport> lst;
	std::string str;
	size_t count = 0;
	double lat = 0, lon = 0;
	int type;

	while (1)
    { 
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0].getCode() == "NULL")
			type = 2;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting " << (mode ? "source" : "destination") << " location by coordinates\n";
		if (type == 1) {
			std::cout << "\nThe closest airports to " << lat << " La, " << lon << " Lo are:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << std::fixed << std::setprecision(2) << Manager::distance(lat, lon, w.getLatitude(), w.getLongitude()) << "\tkm | "
					<< w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ") \n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< (lst.size() + 9 - (lst.size() - 1) % 10) / 10 << "\n";
		}

		if (type == 2) {
			std::cout << "\nThere are no airport within a 300 km radius of " << lat << " La, " << lon << " Lo\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter coordinates to search" << (type != 1 ? ":\n" : " or enter the number of closest airports to consider:\n")
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
			if (num > lst.size() || type == 2) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			lst.resize(num);
			if (mode) {
				origin = lst;
				destination.clear();
			} else
				destination = lst;
			plannerSelected();
			break;
		}
		if (str.size() > 1) {
			count = 0;
			lst = searchInCoords(lat, lon);
			if (lst.size() == 1 && lst[0].getCode() != "NULL") {
				if (in) {
					origin = lst;
					destination.clear();
				} else
					destination = lst;
				plannerSelected();
				break;
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

std::vector<Airport> UI::searchInCoords(double lat, double lon) {
	std::vector<Airport> res;

	for (auto i : manager.getConnections().getVertexSet()) {
		auto w = i->getInfo();
		double dist = Manager::distance(lat, lon, w.getLatitude(), w.getLongitude());
		if (dist < 5) {
			res.clear();
			res.push_back(w);
			return res;
		}
		if (dist <= 300)
			res.push_back(w);
	}
	std::sort(res.begin(), res.end(), [lat, lon](Airport &a, Airport &b) {
		return Manager::distance(lat, lon, a.getLatitude(), a.getLongitude()) < 
			Manager::distance(lat, lon, b.getLatitude(), b.getLongitude());
	});

	if (res.empty())
		res.push_back(Airport("NULL"));

	return res;
}

std::vector<Airport> UI::searchOutCoords(double lat, double lon) {
	std::vector<Airport> res;

	for (auto i : manager.getConnections().getVertexSet()) {
		auto w = i->getInfo();
		double dist = Manager::distance(lat, lon, w.getLatitude(), w.getLongitude());
		if (dist < 5) {
			res.clear();
			res.push_back(w);
			return res;
		}
		if (dist <= 300)
			res.push_back(w);
	}
	std::sort(res.begin(), res.end(), [lat, lon](Airport &a, Airport &b) {
		return Manager::distance(lat, lon, a.getLatitude(), a.getLongitude()) < 
			Manager::distance(lat, lon, b.getLatitude(), b.getLongitude());
	});

	if (res.empty())
		res.push_back(Airport("NULL"));

	return res;
}