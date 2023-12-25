#include "headers/airport.h"

Airport::Airport(std::string code) : code(code) {}

Airport::Airport(std::string code, std::string name, std::string city, std::string country, double latitude, double longitude) 
			: code(code), name(name), city(city), country(country), latitude(latitude), longitude(longitude)
			{}

std::string Airport::getCode() const {
	return code;
}

std::string Airport::getName() const {
	return name;
}

std::string Airport::getCity() const {
	return city;
}

std::string Airport::getCountry() const {
	return country;
}

double Airport::getLatitude() const {
	return latitude;
}

double Airport::getLongitude() const {
	return longitude;
}

bool Airport::operator==(const Airport &rhs) {
	return code == rhs.code;
}