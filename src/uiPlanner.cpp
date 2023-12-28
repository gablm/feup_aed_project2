#include "headers/ui.h"
#include <iostream>

void UI::plannerSelected() {
	std::string str;

	while (1) {
		CLEAR;

		std::cout << "Amadeus - Trip planner\n\nOrigin Selected:\n\n";
		for (auto i : origin)
			std::cout << i.getCode() << " - " << i.getCity() << ", " << i.getCountry() << "\n";
		if (origin.empty())
			std::cout << "(none)\n";
		
		std::cout << "\nDestination Selected:\n\n";
		for (auto i : destination)
			std::cout << i.getCode() << " - " << i.getCity() << ", " << i.getCountry() << "\n";
		if (destination.empty())
			std::cout << "(none)\n";
		std::cout
		<< "\n$> ";

		std::getline(std::cin, str);
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
		helpMsg("Unknown option", "");
	}
}