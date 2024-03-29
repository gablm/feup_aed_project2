#include "headers/ui.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <sstream>

/**
 * Shows the main menu of the trip planner, 
 * with options to select the origin, destination and filters.
*/
void UI::plannerSelected() {
	std::string str;

	while (1) {
		CLEAR;

		std::cout << "Amadeus - Trip planner\n\n>> Origin\n\n";
		for (auto i : origin)
			std::cout << " " << i->getCode() << " - " << i->getCity() << ", " << i->getCountry() << "\n";
		if (origin.empty())
			std::cout << " (none)\n";
		
		std::cout << "\n>> Destination\n\n";
		for (auto i : destination)
			std::cout << " " << i->getCode() << " - " << i->getCity() << ", " << i->getCountry() << "\n";
		if (destination.empty())
			std::cout << " (none)\n";
		std::cout
		<< "\n"
		<< " [0] - Select origin\t\t[1] - Select destination\n"
		<< " (erasing destinations if any)\t[clear] - Reset all fields\n"
		<< "\n"
		<< " [search / s] - Fast search\t[deep-search / ds] - Deep search\n"
		<< " (no filters / limited options)\t(requires filters / slower)\n"
		<< " [F] - Add filters\n"
		<< "\n"
		<< " [B] Back\t\t\t[Q] - Quit\n"
		<< "\n$> ";

		std::getline(std::cin, str);
		if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b") {
			origin.clear();
			destination.clear();
			allowedAirlines.clear();
			maxAirlines = 0;
			mainMenu();
			return;
		}
		if (str == "F" || str == "f") {
			filterSelect();
			continue;
		}
		if (str == "clear") {
			origin.clear();
			destination.clear();
			allowedAirlines.clear();
			maxAirlines = 0;
			//useRecursionLimiter = true;
			continue;
		}
		if (str == "0") {
			plannerMenu(true);
			continue;
		}
		if (str == "1") {
			if (origin.empty()) {
				helpMsg("Please select the origin first!", "0 - go to the origin selection menu");
				continue;
			}
			plannerMenu(false);
			continue;
		}
		if (str == "search" || str == "s") {
			if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}
			buildFlights(false);
			continue;
		}
		if (str == "deep-search" || str == "ds") {
			if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}
			if (allowedAirlines.empty() && maxAirlines == 0) {
				helpMsg("Using this type of search requires filter!", 
					"[f] to add filters");
				continue;
			}

			buildFlights(true);
			continue;
		}
		if (str == "fs") {
			if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}

			buildFlights(true);
			continue;
		}
		helpMsg("Unknown option!", "[command]");
	}
}

/**
 * Shows the current filters and which filters can be changed.
*/
void UI::filterSelect() {
	while(1) {
		CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Current filters:\n"
		<< "\n"
		//<< " Result limiter: " << (useRecursionLimiter ? "True" : "False") << "\n"
		//<< "\n"
		<< " Airline limit: " << (maxAirlines > 0 ? to_string(maxAirlines) : "None") << "\n"
		<< "\n"
		<< " Selected airlines: " << (allowedAirlines.empty() ? "All\n" : "\n\n");

		for (auto i : allowedAirlines)
			std::cout << "\t" << i->getCode() << " - " << i->getName() << "\n";
		
		std::cout
		<< "\n"
		<< ">> Select filtering method:\n"
		<< "\n"
		<< " [0] Limit to number of airlines\n"
		<< " [1] Limit selection of airlines\n"
		<< "\n"
		//<< " [2] Disable result limiter (WARNING)\n"
		//<< "\n"
		<< "[C] Clear filters\n"
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
		if (str == "C" || str == "c") {
			allowedAirlines.clear();
			maxAirlines = 0;
			//useRecursionLimiter = true;
			continue;
		}
		if (str == "0") {
			filterSelectMax();
			continue;
		}
		if (str == "1") {
			filterSelectList();
			continue;
		}
		/*if (str == "2") {
			filterDisableLimiter();
			continue;
		}*/
		helpMsg("Command not found!", "help - shows all commands");
	}
}

// Unused code due to the possibility of hogging all the memory and crashing the entire system

/*void UI::filterDisableLimiter() {
	CLEAR;

	std::cout 
	<< "Amadeus - Planner\n"
	<< "\n"
	<< ">> Disable the result limiter\n"
	<< "\n"
	<< " WARNING - PLEASE READ\n"
	<< "\n"
	<< "Disabling the limiter will result in a more complete result.\n"
	<< "Despite this, the memory used and the calculation time will be exponentially bigger.\n"
	<< "It is not recommended as it some cases can crash the program and cause general lag on the Computer.\n"
	<< "\n"
	<< "Are you sure you want to continue? Type \"YES\" to continue.\n"
	<< "\n"
	<< "$> ";

	std::string str;
	std::getline(std::cin, str);
	if (str != "YES")
		return;
	
	std::cout
	<< "\n"
	<< "Are you really sure you want to continue?\n"
	<< "Type \"I HAVE READ THE WARNING\" to continue.\n"
	<< "\n"
	<< "$> ";

	std::getline(std::cin, str);

	if (str != "I HAVE READ THE WARNING")
		return;

	std::cout
	<< "\n"
	<< "LAST CHANCE\n"
	<< "Are you really sure you want to continue?\n"
	<< "Type \"I AM SURE\" to continue.\n"
	<< "\n"
	<< "$> ";

	std::getline(std::cin, str);

	if (str != "I AM SURE")
		return;

	useRecursionLimiter = false;
}*/

/**
 * Prints the menu to change the maximum number of airlines a trip should use.
*/
void UI::filterSelectMax() {
	while(1) {
		CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Select filtering method: Number of airlines\n"
		<< "\n"
		<< "Input the maximum number of airlines a trip should use.\n"
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
		int num = atoi(str.c_str());
		if (num <= 0 && num != -1) {
			helpMsg("Please insert an valid integer!", "top [integer >= 1]");
			continue;
		}
		maxAirlines = num;
		break;		
	}
}

/**
 * Shows the menu to search and add airlines to the filter of what airlines to use.
*/
void UI::filterSelectList() {
	std::vector<Airline *> lst;
	size_t count = 0;
	std::string str;
	std::string search;
	int type;

	while (1)
    {
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0]->getCode() == "NULL")
			type = 2;
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Select filtering method: Specified Airlines\n"
		<< "\n"
		<< "Input the what Airlines you would like to limit the search to.\n"
		<< "Selecting a Airline currently selected will remove it\n"
		<< "\n"
		<< ">> Selecting Airline\n";
		if (type == 1) {
			std::cout << "\nThe search for \"" << search << "\" has returned:\n\n";
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				auto w = lst[i];
				if (allowedAirlines.find(w) != allowedAirlines.end())
					std::cout << "FILTERED ";
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
			if (allowedAirlines.find(lst[num]) == allowedAirlines.end())
					allowedAirlines.insert(lst[num]);
				else
					allowedAirlines.erase(lst[num]);
			continue;
		}
		if (str.size() > 1) {
			count = 0;
			search = str;
			lst = searchAirline(str);
			if (lst.size() == 1 && lst[0]->getCode() != "NULL") {
				if (allowedAirlines.find(lst[num]) == allowedAirlines.end())
					allowedAirlines.insert(lst[num]);
				else
					allowedAirlines.erase(lst[num]);
			}
			continue;
		}
		helpMsg("Search query is too small!", "[query with at least 2 characters]");
    }
}

/**
 * Displays the flights found for the current search.
*/
void UI::displayFlights(vector<Trip> &lst) {
	
	size_t count = 0;
	std::string str;

	while (1)
    {
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Statistics\n"
		<< "\n"
		<< ">> Paths between the selection \n\n";
		if (!lst.empty()) {
			for (size_t i = count; i < min(count + 10, lst.size()); i++) {
				printPath(lst[i]);
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		} else
			std::cout << " No path between the airports was found with the current filters.\n";

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< (lst.empty() ? "Use" : "Select a airport by its number or use") << " one of the commands above.\n"
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
		helpMsg("Invalid command!", "[next/back/b/q/(integer)]");
	}
}

/**
 * Finds paths between two vectors of origins and destinations.
 * If an airport was already reached, it won't count twice for other routes even 
 * if the airports in between them are different and they have the same lenght.
 * Such consideration will only happen at the final node.
 * The result will be saved in a vector inside the UI class.
 * A trip is defined as a pair of vectors <Airport *> and <Airline *>,
 * representing all the airports and airlines in a trip option. 
 * @param start Vector of vertexes of starting airports
 * @param end vector of vertexes of ending airports
 * @note Complexity: O(V + E)
*/
void UI::fastFindPath(vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end) {
	auto graph = manager.getFlights();
	for (auto i : graph.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
		i->clearLast();
	}

	vector<Trip> res;
	list<Vertex<Airport *, Airline *>*> queue;
	for (auto i : start) {
		i->setVisited(true);
		i->setNum(0);
		queue.push_back(i);
	}
	int minStops = __INT32_MAX__;
	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		if (u->getNum() + 1 > minStops)
			continue;
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (!w->isVisited()) {
				auto found = std::find(end.begin(), end.end(), w);
				if (found != end.end() && u->getNum() + 1 <= minStops) {
					minStops = u->getNum() + 1;
					(*found)->addLast(u, i.getInfo());
					continue;
				}
				w->setVisited(true);
				w->clearLast();
				w->addLast(u, i.getInfo());
				w->setNum(u->getNum() + 1);
				queue.push_back(w);
			}
		}
	}

	for (auto opt : end) {
		for (auto i : opt->getLasts()) {
			list<Airport *> a;
			list<Airline *> b;
			a.push_front(i.first->getInfo());
			b.push_front(i.second);
			Vertex <Airport *, Airline *> *rn = i.first;
			while (std::find(start.begin(), start.end(), rn) == start.end()) {
				auto vt = rn->getLasts().back();
				a.push_front(vt.first->getInfo());
				b.push_front(vt.second);
				rn = vt.first;
			}
			a.push_back(opt->getInfo());
			res.push_back(make_pair(a, b));
		}
	}
	plannerResult = res;
}

/**
 * Prints a path in the format: AIRPORT -> AIRPORT AIRLINE
 * @param path A trip (pair of vectors <Airport *> and <Airline *>)
*/
void UI::printPath(Trip path) {
	auto i = path.first.end();
	auto j = path.second.end();

	while (1) {
		i--;
		std::cout << (*i)->getCode();
		if (i == path.first.begin()) {
			std::cout << "\n";
			break;
		}
		std::cout << " --> ";
	}
	std::cout << "    ";
	while (1) {
		j--;
		std::cout << (*j)->getCode() << "     ";
		if (j == path.second.begin()) {
			std::cout << "\n";
			break;
		}
	}
}

/**
 * Used to delegate a calculation to another thread.
*/
void UI::findFilter(UI *who, vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end, bool *loading) {
	who->findPathFilter(start, end);
	*loading = false;
}

/**
 * Builds the vectors of vertexes used in a search 
 * and starts it according to the parameter.
 * @param way True -> Slow search / False -> Fast Search
*/
void UI::buildFlights(bool way) {
	plannerResult.clear();
	auto graph = manager.getFlights();

	vector<Vertex<Airport *, Airline *> *> start;
	vector<Vertex<Airport *, Airline *> *> end;
	
	for (auto i : origin)
		start.push_back(graph.findVertex(i->getCode()));
				
	for (auto i : destination)
		end.push_back(graph.findVertex(i->getCode()));
	
	if (way) {
		bool loading = true;
		std::thread th = std::thread(UI::findFilter, this, start, end, &loading);

		int count = 0;
		double time_sec = 0;
		while (loading) {
			CLEAR;
			std::cout << "Amadeus - Global statistics\n"
				  	<< "\n"
				  	<< "Paths are currently being calculated.\n"
				  	<< "Please wait" << std::string(count, '.') << "\n"
					<< "\n"
					<< "Time elapsed: " << (int)time_sec << "s\n";
			count = count == 3 ? 0 : count + 1;
			time_sec += 0.5;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		th.join();
	} else
		fastFindPath(start, end);

	displayFlights(plannerResult);
}

/**
 * Returns the amount of recursions allowed after maxL recursion.
 * Used to prevent slow searches and hogging of resources.
 * @param maxL Current recursion count.
*/
int UI::getNextMax(int maxL) {
	//if (!useRecursionLimiter)
	//	return 1000000;
	switch (maxL) {
		case 1:
			return 40;
		case 2: 
			return 16;
		case 3: 
			return 10;
		case 4:
			return 6;
		case 5:
			return 3;
		default: 
			return 2;
	}
	//return (int)(39.9579019304262 * std::exp(-0.4069703510623 * maxL));
}

/**
 * Stores the trips returned by deep-search using recursion.
 * As there could be millions of combinations between two airports,
 * a recursion limiter is used.
 * This process starts at the ending vertex and ends at a starting vertex from the vector provided.
 * @param ports List of airports for a trip
 * @param lines List of airlines for a trip
 * @param curr Current Vertex being added to a trip
 * @param start Vector of starting vertexes.
 * @param time Recursion counter
 * @param airlineSet Used when the amount of airlines is limited by a filter
 * @note Complexity: O(2 ^ F - 1), F being the number of flights found
*/
void UI::storeResult(list<Airport *> ports, list<Airline *> lines, Vertex<Airport *, Airline *> *curr, vector<Vertex<Airport *, Airline *> *> start, int time, set<Airline *> airlineSet) {

	if (std::find(start.begin(), start.end(), curr) != start.end()) {
		plannerResult.push_back(make_pair(ports, lines));
		return;
	}

	int maxLoops = max(getNextMax(time), 1);
	int count = 0;
	for (auto i : curr->getLasts()) {
		auto cairset = airlineSet;
		if (maxLoops == count)
			break;
		if (maxAirlines > 0) {
			cairset.insert(i.second);
			if (cairset.size() > maxAirlines)
				continue;
		}
		auto cports = ports;
		cports.push_back(i.first->getInfo());
		auto clines = lines;
		clines.push_back(i.second);
		storeResult(cports, clines, i.first, start, time + 1, cairset);
		count++;
	}	
}

/**
 * Finds paths between two vectors of origins and destinations.
 * This includes all options that have the same length, that is
 * if the airports in between them are different but the route has similar distance.
 * The result will be saved in a vector inside the UI class.
 * A trip is defined as a pair of vectors <Airport *> and <Airline *>,
 * representing all the airports and airlines in a trip option. 
 * @param start Vector of vertexes of starting airports
 * @param end vector of vertexes of ending airports
 * @note Complexity: O(V + E)
*/
void UI::findPathFilter(vector<Vertex<Airport *, Airline *> *> start, vector<Vertex<Airport *, Airline *> *> end) {
	auto graph = manager.getFlights();
	for (auto i : graph.getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
		i->clearLast();
	}

	list<Vertex<Airport *, Airline *>*> queue;
	for (auto i : start) {
		i->setVisited(true);
		i->setNum(0);
		queue.push_back(i);
	}
	int minStops = __INT32_MAX__;
	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		if (u->getNum() + 1 > minStops)
			continue;
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (w->getNum() == __INT32_MAX__ || u->getNum() + 1 <= w->getNum()) {
				if (!allowedAirlines.empty()) {
					if (std::find(allowedAirlines.begin(), allowedAirlines.end(), i.getInfo()) == allowedAirlines.end())
						continue;
				}
				w->addLast(u, i.getInfo());
				if (std::find(end.begin(), end.end(), w) != end.end() && u->getNum() + 1 <= minStops) {
					minStops = u->getNum() + 1;
					continue;
				}
				w->setNum(u->getNum() + 1);
				queue.push_back(w);
			}
		}
	}
	std::cout << "bop\n";
	for (auto i : end) {
		list<Airport *> ports {i->getInfo()};
		storeResult(ports, list<Airline *> {}, i, start, 1, set<Airline *> {});
	}
}