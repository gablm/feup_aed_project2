#ifndef AIRLINE_H_
#define AIRLINE_H_

#include <string>

/** 
 * An object that represents an Airline. 
 * It is composed by a code, name, callsign and country.
*/
class Airline {
	private:
		std::string code;
		std::string name;
		std::string callsign;
		std::string country;
	public:
		Airline(std::string code, std::string name, std::string callsign, std::string country);
		std::string getCode() const;
		std::string getName() const;
		std::string getCallSign() const;
		std::string getCountry() const;
};

/**
 * Default constructor for an Airline
 * @param code Airline code
 * @param name Airline name
 * @param callsign Airline Call Sign
 * @param country Country where the city and Airline are located
*/
inline Airline::Airline(std::string code, std::string name, std::string callsign, std::string country) :
		code(code), name(name), callsign(callsign), country(country) {}

/** @return Airline code */
inline std::string Airline::getCode() const {
	return code;
}

/** @return Airline name */
inline std::string Airline::getName() const { 
	return name; 
}

/** @return Airline callsign */
inline std::string Airline::getCallSign() const { 
	return callsign; 
}

/** @return Airline country */
inline std::string Airline::getCountry() const { 
	return country;
}

#endif