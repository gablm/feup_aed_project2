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
			if (origin.empty() || destination.empty()) {
				helpMsg("Please select both the origin and destination first!", 
					"[0] to select the origin and [1] to select the destination");
				continue;
			}
			displayFlights();
			continue;
		}
		helpMsg("Unknown option!", "[command]");
	}
}

template <class T, class W>
vector<pair<T, W>> findPath(Graph<T, W> *g, Vertex<T, W> *start, Vertex<T, W> *end) {

}

void UI::displayFlights() {
	
}
