#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

/**
 * Searches a string in another string.
 * This search is case-insensitive.
 * @param one The string to search
 * @param two The string to be searched
 * @return If there are any matches
*/
bool UI::strFind(const std::string &one, const std::string &two) {
	auto it = std::search(one.begin(), one.end(), two.begin(), two.end(),
    	[](unsigned char a, unsigned char b) { return std::toupper(a) == std::toupper(b);}
  		);
  	return (it != one.end());
}

/**
 * Shows the menu for selecting how the search the source/destination airports.
 * @param in True -> Source / False -> Destination
*/
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

/**
 * Shows the menu for selecting source/destination 
 * airports using the airport code/name.
 * @param in True -> Source / False -> Destination
*/
void UI::plannerAirportSelect(bool in) {
	std::vector<Airport *> lst;
	size_t count = 0;
	std::string str;
	std::string search;
	int type;

	while (1)
    {
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0] == 0)
			type = 2;
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting " << (in ? "source" : "destination") << " location by Airport\n";
		if (type == 1) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << i << ". " << w->getCode() << " - " << w->getName() << "  (" << w->getCountry() << ")\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
		}

		if (type == 2) {
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n(nothing / already selected)\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (type != 1 ? ":\n" : " or select a airport using a number.\n")
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
			if (num >= lst.size() || type == 2) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			vector<Airport *> res;
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
			lst = searchAirport(str, in);
			if (lst.size() == 1 && lst[0] != 0) {
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

/**
 * Shows the menu for selecting source/destination 
 * airports using the city they are located in.
 * @param in True -> Source / False -> Destination
*/
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
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;
        
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
						<< totalPages << "\n";
		}

		if (type == 2) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n(nothing / already selected)\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter a term to search" << (type != 1 ? ":\n" : " or select a city using a number.\n")
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
			if (num >= lst.size() || type == 2) {
				helpMsg("Please enter a valid option!", "[number]");
				continue;
			}
			auto it = lst.begin();
			std::advance(it, num);
			vector<Airport *> res;
			for (auto i : manager.getConnections().getVertexSet()) {
				auto w = i->getInfo();
				if (w->getCity() + ", " + w->getCountry() == *it)
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
			lst = searchCity(str, in);
			if (lst.size() == 1 && *lst.begin() != "NULL") {
				vector<Airport *> res;
				for (auto i : manager.getConnections().getVertexSet()) {
					auto w = i->getInfo();
					if (w->getCity() + ", " + w->getCountry() == *lst.begin())
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

/**
 * Shows the menu for selecting source/destination 
 * airports using coordinates.
 * It will display all the airports within 300 km 
 * and ask for how many to save for the search.
 * @param mode True -> Source / False -> Destination
*/
void UI::plannerCoordsSelect(bool mode) {
	std::vector<Airport *> lst;
	std::string str;
	size_t count = 0;
	double lat = 0, lon = 0;
	int type;

	while (1)
    { 
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0] == 0)
			type = 2;
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Selecting " << (mode ? "source" : "destination") << " location by coordinates\n";
		if (type == 1) {
			std::cout << "\nThe closest airports to " << lat << " La, " << lon << " Lo are:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				std::cout << std::fixed << std::setprecision(2) << Manager::distance(lat, lon, w->getLatitude(), w->getLongitude()) << "\tkm | "
					<< w->getCode() << " - " << w->getName() << "  (" << w->getCountry() << ") \n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
		}

		if (type == 2) {
			std::cout 
			<< "\n" 
			<< "There are no airport within a 300 km radius of " << lat << " La, " << lon << " Lo\n"
			<< "Or all of them were already selected.\n";
		}

		std::cout
        << "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Please enter coordinates to search" << (type != 1 ? ":\n" : " or enter the number of closest airports to consider.\n")
		<< "Enter the coordinates in the format LATITUDE LONGITUDE (ex: \'40 40\').\n"
		<< "\n"
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
			lst = searchCoords(lat, lon, mode);
			if (lst.size() == 1 && lst[0] != 0) {
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

/**
 * Searches the connection graph for matches with the query.
 * It matches the query with a airport code or its name.
 * @param query String with the content to search
 * @param in If false, it will check if the airport is already in the origin list.
 * @return Vector of matches
*/
vector<Airport *> UI::searchAirport(std::string query, bool in) {
	auto conns = manager.getConnections();
	std::transform(query.begin(), query.end(), query.begin(), ::toupper);
	auto vtx = conns.findVertex(query);
	vector<Airport *> res;

	if (vtx != NULL) {
		if (in || std::find(origin.begin(), origin.end(), vtx->getInfo()) == origin.end()) {
			res.push_back(vtx->getInfo());
			return res;
		}
	}
	
	for (auto i : conns.getVertexSet()) {
		auto w = i->getInfo();
		if (!in && std::find(origin.begin(), origin.end(), w) != origin.end())
			continue;
		if (strFind(w->getCode(), query) || strFind(w->getName(), query))
			res.push_back(w);
	}

	if (res.empty())
		res.push_back(0);
	
	return res;
}

/**
 * Searches the connection graph for matches with the query.
 * It matches the query with a airport if their city or country matches the query.
 * @param query String with the content to search
 * @param in If false, it will check if the airport is already in the origin list.
 * @return Vector of matches
*/
std::set<std::string> UI::searchCity(std::string query, bool in) {	
	auto conns = manager.getConnections();
	std::set<std::string> res;
	std::transform(query.begin(), query.end(), query.begin(), ::tolower);
	for (auto i : conns.getVertexSet()) {
		auto w = i->getInfo();
		if (!in && std::find(origin.begin(), origin.end(), w) != origin.end())
			continue;
		std::string fullName = w->getCity() + ", " + w->getCountry();
		if (strFind(w->getCity(), query) || strFind(w->getCountry(), query))
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

	(void)in;

	return res;
}

/**
 * Searches the connections graph for airports near the coordinates provided.
 * An airport is considered near if the distance if less or equal to 300 km.
 * If the distance to an airport is less than 5 km, 
 * it is considered to be the coordinates of that airport.
 * @param lat Latitude to search
 * @param lon Longitude to search
 * @param in Search mode, origin or destination
 * @return Vector of matches
*/
std::vector<Airport *> UI::searchCoords(double lat, double lon, bool in) {
	std::vector<Airport *> res;

	for (auto i : manager.getConnections().getVertexSet()) {
		auto w = i->getInfo();
		if (in && std::find(origin.begin(), origin.end(), w) != origin.end())
			continue;
		double dist = Manager::distance(lat, lon, w->getLatitude(), w->getLongitude());
		if (dist < 5) {
			res.clear();
			res.push_back(w);
			return res;
		}
		if (dist <= 300)
			res.push_back(w);
	}
	std::sort(res.begin(), res.end(), [lat, lon](Airport *a, Airport *b) {
		return Manager::distance(lat, lon, a->getLatitude(), a->getLongitude()) < 
			Manager::distance(lat, lon, b->getLatitude(), b->getLongitude());
	});

	if (res.empty())
		res.push_back(0);

	return res;
}