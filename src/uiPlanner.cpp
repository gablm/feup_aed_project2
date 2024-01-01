#include "headers/ui.h"
#include <iostream>

/**
 * Shows the main menu of the trip planner, 
 * with options to select the origin, destination and filters.
*/
void UI::plannerSelected() {
	std::string str;
	allowedAirlines.clear();
	maxAirlines = 0;

	while (1) {
		CLEAR;

		std::cout << "Amadeus - Trip planner\n\n>> Origin\n\n";
		for (auto i : origin)
			std::cout << " " << i.getCode() << " - " << i.getCity() << ", " << i.getCountry() << "\n";
		if (origin.empty())
			std::cout << " (none)\n";
		
		std::cout << "\n>> Destination\n\n";
		for (auto i : destination)
			std::cout << " " << i.getCode() << " - " << i.getCity() << ", " << i.getCountry() << "\n";
		if (destination.empty())
			std::cout << " (none)\n";
		std::cout
		<< "\n"
		<< " [0] - Select origin\t\t[1] - Select destination\n"
		<< " (erasing destinations if any)\t[clear] - Reset all fields\n"
		<< "\n"
		<< " [F] - Add filters\t\t[search] - Start search\n"
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
		if (str == "search") {
			if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}
			displayFlights();
			continue;
		}
		if (str == "s2") {
			displayFlights2();
			continue;
		}
		helpMsg("Unknown option!", "[command]");
	}
}

/**
 * Shows the current filters and which filters can be changed.
*/
void UI::filterSelect() {
	while(1){
		CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Current filters:\n"
		<< "\n"
		<< " Airline limit: " << (maxAirlines > 0 ? to_string(maxAirlines) : "None") << "\n"
		<< "\n"
		<< " Selected airlines: " << (allowedAirlines.empty() ? "All\n" : "\n\n");

		for (auto i : allowedAirlines)
			std::cout << "\t" << i.getCode() << " - " << i.getName() << "\n";
		
		std::cout
		<< "\n"
		<< ">> Select filtering method:\n"
		<< "\n"
		<< " [0] Limit to number of airlines\n"
		<< " [1] Limit selection of airlines\n"
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
		if (str == "0"){
			filterSelectMax();
			break;
		}
		if (str == "1"){
			filterSelectList();
			break;
		}
		helpMsg("Command not found!", "help - shows all commands");
	}
}

/**
 * Prints the menu to change the maximum number of airlines a trip should use.
*/
void UI::filterSelectMax() {
	while(1){
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
	std::vector<Airline> lst;
	size_t count = 0;
	std::string str;
	std::string search;
	int type;

	while (1)
    {
		type = lst.empty() ? 0 : 1;
		if (!lst.empty() && lst[0].getCode() == "NULL")
			type = 2;
		int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

        CLEAR;
        std::cout 
		<< "Amadeus - Planner\n"
		<< "\n"
		<< ">> Select filtering method: Specified Airlines.\n"
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
				std::cout << i << ". " << w.getCode() << " - " << w.getName() << "  (" << w.getCountry() << ")\n";
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
		<< "Search for a term" << (type != 1 ? "" : ", select a airline using a number") << " or use one of the commands above\n"
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
			if (lst.size() == 1 && lst[0].getCode() != "NULL") {
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

template <class T, class W>
vector<list<pair<T, W>>> findPath(Graph<T, W> *g, Vertex<T, W> *start, Vertex<T, W> *end);

void UI::displayFlights() {
	CLEAR;
	
	auto graph = manager.getFlights();
	auto ini = graph.findVertex(origin[0].getCode());
	auto end = graph.findVertex(destination[0].getCode());
	vector<list<pair<Airport, Airline>>> res;
	try {
		res = findPath(&graph, ini, end);
	} catch (const exception &e) {
		std::cout << e.what();
		exit(0);
	}

	vector<list<pair<Airport, Airline>>> tempRes; 
	for (auto i : res){
		if (isValid(i)){
			tempRes.push_back(i);
		}
	}
	res = tempRes;
	

	for (auto i : res) {
		size_t len = 0;
		for (auto j : i) {
			std::cout << j.first.getName() << (j.second.getCode() != "" ? " (" : "") << j.second.getName();
			if (len < i.size() - 1)
				std::cout << ") -> ";
			len++;
		}
		std::cout << "\n";
	}

	while (std::cin.get() != '\n') {}
}

template <class T, class W>
vector<list<pair<T, W>>> findPath(Graph<T, W> *g, Vertex<T, W> *start, Vertex<T, W> *end) {
	
	for (auto i : g->getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
		i->setLast(NULL, W());
	}
	vector<list<pair<T, W>>> res;
	list<Vertex<T, W>*> queue;
	list<pair<Vertex<T, W>*, W>> alter;
	start->setVisited(true);
	start->setNum(0);
	queue.push_back(start);
	int minStops = __INT32_MAX__;
	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (!w->isVisited()) {
				if (w == end && u->getNum() + 1 <= minStops) {
					minStops = u->getNum() + 1;
					alter.push_back(make_pair(u, i.getInfo()));
					continue;
				}
				w->setVisited(true);
				w->setLast(u, i.getInfo());
				w->setNum(u->getNum() + 1);
				queue.push_back(w);
			}
		}
	}
	//cout << "test111";
	std::cout << "ends size " << alter.size() << "\n";
	//cout << "test222";
	for (auto i : alter) {
		list<pair<T, W>> tmp;
		tmp.push_front(make_pair(i.first->getInfo(), i.second));
		Vertex <T, W> *rn = i.first;
		while (rn != start) {
			auto a = rn->getLast();
			tmp.push_front(make_pair(a.first->getInfo(), a.second));
			rn = a.first;
		}
		tmp.push_back(make_pair(end->getInfo(), W()));
		res.push_back(tmp);
	}
	return res;
}

//
std::vector<std::list<Airport>> findPath2(Graph<Airport, std::string> *g, Vertex<Airport, std::string> *start, Vertex<Airport, std::string> *end);

void UI::displayFlights2() {
	CLEAR;
	
	auto graph = manager.getConnections();
	vector<std::list<Airport>> res;
	try {
		auto ini = graph.findVertex(origin[0].getCode());
		auto end = graph.findVertex(destination[0].getCode());
		res = findPath2(&graph, ini, end);
	} catch (const exception &e) {
		std::cout << e.what();
		exit(0);
	}
	
	auto av = manager.getFlights();
	for (auto trp : res) {
		auto i = trp.end();
		auto li = trp.end();
		li--;
		while (true) {
			i--;
			std::cout << i->getName() << "\n";
			if (i != trp.begin()) {
				for (auto edge : av.findVertex(i->getCode())->getAdj()) {
					if (edge.getDest()->getInfo() == *li) {
						std::cout << " " << edge.getInfo().getCode() << " ";
					}
				}
				std::cout << "\n";
				li--;
			} else break;
			
		}
		std::cout << "\n";
	}

	std::cout << "total trips: " << res.size() << "\nPRESS ENTER TO CONTINUE";

	while (std::cin.get() != '\n') {}
}

void storeResult(list<Airport> ports,
	Vertex<Airport, std::string> *curr, Vertex<Airport, std::string> *start, std::vector<std::list<Airport>>& res) {
	
	if (curr == start) {
		std::cout << "beep\n";
		res.push_back(ports);
		return;
	}

	for (auto i : curr->getLasts()) {
		auto cports = ports;
		cports.push_back(i.first->getInfo());
		storeResult(cports, i.first, start, res);
	}	
}

std::vector<std::list<Airport>> findPath2(Graph<Airport, std::string> *g, Vertex<Airport, std::string> *start, Vertex<Airport, std::string> *end) {
	for (auto i : g->getVertexSet()) {
		i->setVisited(false);
		i->setNum(__INT32_MAX__);
		i->clearLast();
	}

	vector<std::list<Airport>> res;
	list<Vertex<Airport, std::string>*> queue;
	start->setVisited(true);
	start->setNum(0);
	queue.push_back(start);
	int minStops = __INT32_MAX__;
	while (!queue.empty()) {
		auto u = queue.front();
		queue.pop_front();
		if (u->getNum() + 1 > minStops)
			continue;
		for (auto i : u->getAdj()) {
			auto w = i.getDest();
			if (w->getNum() == __INT32_MAX__ || u->getNum() + 1 <= w->getNum()) {
				w->addLast(u, i.getInfo());
				if (w == end && u->getNum() + 1 <= minStops) {
					minStops = u->getNum() + 1;
					continue;
				}
				w->setNum(u->getNum() + 1);
				queue.push_back(w);
			}
		}
	}
	std::cout << "bop\n";
	list<Airport> ports {end->getInfo()};
	storeResult(ports, end, start, res);
	return res;
}

bool UI::isValid(list<std::pair<Airport, Airline>> path){
	set<Airline> usedAirlines;
	bool newAirline = false;
	int airlineNum = -1;

	if (allowedAirlines.empty() && maxAirlines == 0)
		return true;

	for (auto i : path) {
		newAirline = true;
		for (auto j : allowedAirlines){
			if (j.getName() == i.second.getName()){
				return false;
			}
		}
		//cout << i.second.getName() << "\n";
		for (auto j : usedAirlines) {
			if (j.getName() == i.second.getName() && i.second.getName() != "")
				newAirline = false;
		}

		if (newAirline == true) {
			airlineNum++;
			usedAirlines.emplace(i.second);
		}
	}

	if (airlineNum <= maxAirlines)
		return true;

	return false;
}
