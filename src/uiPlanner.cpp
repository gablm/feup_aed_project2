#include "headers/ui.h"
#include <iostream>

void UI::plannerSelected() {
	std::string str;

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
		<< " (erasing destinations if any)\t[clear] - Clear both fields\n"
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
		if (str == "clear") {
			origin.clear();
			destination.clear();
			continue;
		}
		if (str == "0") {
			plannerInMenu();
			continue;
		}
		if (str == "1") {
			if (origin.empty()) {
				helpMsg("Please select the origin first!", "0 - go to the origin selection menu");
				continue;
			}
			plannerOutMenu();
			continue;
		}
		if (str == "search") {
			/*if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}*/
			displayFlights();
			continue;
		}
		helpMsg("Unknown option!", "[command]");
	}
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

	std::cout << "ends size " << alter.size() << "\n";

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

void UI::displayFlights() {
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
