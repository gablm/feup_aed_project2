#include "headers/ui.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

/**
 * Shows the Airport selection menu to present 
 * the statistics of a specific airport
*/
void UI::statsAirportSelect() {
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
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Selecting Airport\n";
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
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n(nothing)\n";
		}

		std::cout
		<< "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Search for a term" << (type != 1 ? "" : ", select a airport using a number") << " or use one of the commands above.\n"
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
			showAirport(lst[num]->getCode());
			continue;
		}
		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchAirport(str, true);
			if (lst.size() == 1 && lst[0] != 0)
				showAirport(lst[num]->getCode());
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

/**
 * Shows the City selection menu to present 
 * the statistics of a specific city
*/
void UI::statsCitySelect() {
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
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Selecting City\n";
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
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n(nothing)\n";
		}

		std::cout
		<< "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Search for a term" << (type != 1 ? "" : ", select a city using a number") << " or use one of the commands above.\n"
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
			showCity(*it);
			continue;
		}

		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchCity(str, true);
			if (lst.size() == 1 && *lst.begin() != "NULL")
				showCity(*lst.begin());
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

/**
 * Shows the Airline selection menu to present 
 * the statistics of a specific airline
*/
void UI::statsAirlineSelect() {
	std::vector<Airline *> lst;
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
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Selecting Airline\n";
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
			std::cout << "\nThe search for \"" << str << "\" has returned:\n\n(nothing)\n";
		}

		std::cout
		<< "\n"
		<< (type != 1 ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (type != 1 ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "Search for a term" << (type != 1 ? "" : ", select a airline using a number") << " or use one of the commands above.\n"
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
			showAirline(lst[num]->getCode());
			continue;
		}
		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchAirline(str);
			if (lst.size() == 1 && lst[0] != 0)
				showAirline(lst[num]->getCode());
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

/**
 * Searches the airline map for matches with the query.
 * It matches the query with a airline code, its name or its call sign.
 * @param query String with the content to search
 * @return Vector of matches
*/
std::vector<Airline *> UI::searchAirline(std::string query) {
	std::vector<Airline *> res;
	std::transform(query.begin(), query.end(), query.begin(), ::toupper);

	for (auto i : manager.getAirlines()) {
		auto w = i.second;
		if (strFind(w->getCode(), query) || strFind(w->getName(), query) 
				|| strFind(w->getCallSign(), query))
			res.push_back(w);
	}

	if (res.empty())
		res.push_back(0);

	return res;
}