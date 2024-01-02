#ifndef AIRPORT_H_
#define AIRPORT_H_

#include <string>

/** 
 * An object that represents an Airport. 
 * It is composed by a code, name, city, country and coordinates.
 * The coordinates in it self are composed by two doubles: the latitude and the longitude.
*/
class Airport {
	private:
		std::string code;
		std::string name;
		std::string city;
		std::string country;
		double latitude;
		double longitude;
	public:
		Airport(std::string code, std::string name, std::string city, 
			std::string country, double latitude, double longitude);
		std::string getCode() const;
		std::string getName() const;
		std::string getCity() const;
		std::string getCountry() const;
		double getLatitude() const;
		double getLongitude() const;
};

/**
 * Default constructor for an Airport
 * @param code Airport code
 * @param name Airport name
 * @param city City where the airport is located
 * @param country Country where the city and airport are located
 * @param latitude Latitude of the airport
 * @param longitude Longitude of the airport
*/
inline Airport::Airport(std::string code, std::string name, std::string city, std::string country, double latitude, double longitude) : 
	code(code), name(name), city(city), country(country), latitude(latitude), longitude(longitude) {}

/** Returns the Airport code */
inline std::string Airport::getCode() const {
	return code;
}

/** Returns the Airport name */
inline std::string Airport::getName() const { 
	return name; 
}

/** Returns the Airport city */
inline std::string Airport::getCity() const { 
	return city; 
}

/** Returns the Airport country */
inline std::string Airport::getCountry() const { 
	return country;
}

/** Returns the Airport latitude */
inline double Airport::getLatitude() const {
	return latitude;
}

/** Returns the Airport longitude */
inline double Airport::getLongitude() const {
	return longitude;
}

#endif