#ifndef AIRPORT_H_
#define AIRPORT_H_

#include <string>

class Airport {
	private:
		std::string code;
		std::string name;
		std::string city;
		std::string country;
		float latitude;
		float longitude;
	public:
		Airport(std::string code, std::string name, 
			std::string city, std::string country, 
			float latitude, float longitude);
		std::string getCode() const;
		std::string getName() const;
		std::string getCity() const;
		std::string getCountry() const;
		float getLatitude() const;
		float getLongitude() const;
};

Airport::Airport(std::string code, std::string name, std::string city, std::string country, float latitude, float longitude) 
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

float Airport::getLatitude() const {
	return latitude;
}

float Airport::getLongitude() const {
	return longitude;
}

#endif