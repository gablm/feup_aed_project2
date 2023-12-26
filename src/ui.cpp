#include "headers/ui.h"
#include <iostream>

UI::UI() : manager(Manager()) {
	loader = std::thread(loadAll, this);
	auto start = std::chrono::high_resolution_clock::now();
	
	/*int count = 0;
	while (loading) {
		CLEAR;
		std::cout << "Data is currently loading.\nPlease wait" << std::string(count, '.') << "\n";
		count = count == 3 ? 0 : count + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}*/
	
	loader.join();
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
}

void UI::setManager(Manager value) {
	manager = value;
}

void UI::setLoading(bool value) {
	loading = value;
}

void UI::loadAll(UI *ui) {
	Manager manager;
	manager.load_airports();
	manager.load_airlines();
	manager.load_flights();
	ui->setLoading(false);
	ui->setManager(manager);
}


void UI::mainMenu() {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Lookup Tool\n"
        << "\n"
        #ifdef _linux__
        << "Welcome " << getenv("USER") << "!\n"
        #else
        << "Welcome!\n"
        #endif
		<< "Select an option [0-3]:\n"
        << "\n"
        << " [0] Global statistics\n"
		<< " [1] ??\n"
		<< " [2] ??\n"
        << "\n"
		<< " [3] Plan a trip\n"
		<< "\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		getline(std::cin, str);
        if (str == "Q" || str == "q") {
			CLEAR;
            exit(0);
		}
		if (str.size() > 1) {
			HelpMsg("", "");
			continue;
		}
		switch (str[0] - '0')
		{
			case 0:
				globalStats();
				break;
			default:
				HelpMsg("", "");
				break;
		}
    }
}

void UI::globalStats() {
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Amadeus - Global statistics\n"
		<< "\n"
		<< "There are currently available:"
		<< "\n "
        << manager.airportCount() << " airports \n "
		<< manager.airlineCount() << " airlines \n "
        << manager.flightCount() << " flights\n"
		<< "\n"
		<< "Loaded in " << loadtime << "s.\n"
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
		HelpMsg("", "");
    }
}

void UI::HelpMsg(std::string error, std::string usage) {

	CLEAR;
	std::cout << "Amadeus - Lookup Tool\n\n";
	if (error != "" && usage != "") {
		std::cout << "Invalid operation!\n"
				  << "\n Problem: " << error
				  << "\n Usage: " << usage
				  << "\n\nPress ENTER to continue...";
	} else {
		std::cout << "No help available for this page."
				  << "\n\nPress ENTER to continue...";
	}
	while (std::cin.get() != '\n') { }
}