#include "headers/airline.h"

Airline::Airline(std::string code, std::string name, std::string callsign, std::string country) 
			: code(code), name(name), callsign(callsign), country(country)
			{}
	
std::string Airline::getCode() const {
	return code;
}

std::string Airline::getName() const {
	return name;
}

std::string Airline::getCallSign() const {
	return callsign;
}

std::string Airline::getCountry() const {
	return country;
}

bool Airline::operator==(const Airline &cmp) {
	return code == cmp.code && name == cmp.name 
		&& callsign == cmp.callsign && country == cmp.country;
}

bool Airline::operator<(const Airline &cmp) const {
	return code != cmp.code ? code < cmp.code : 
			name != cmp.name ? name < cmp.name :
			callsign != cmp.callsign ? callsign < cmp.callsign :
			country < cmp.country; 
}
