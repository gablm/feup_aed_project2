#ifndef AIRLINE_H_
#define AIRLINE_H_

#include <string>

class Airline {
	private:
		std::string code;
		std::string name;
		std::string callsign;
		std::string country;
	public:
		Airline() : code("") {}
		Airline(std::string code) : code(code) {}
		Airline(std::string code, std::string name, std::string callsign, std::string country) :
			code(code), name(name), callsign(callsign), country(country) {}
		std::string getCode() const { return code; }
		std::string getName() const { return name; }
		std::string getCallSign() const { return callsign; }
		std::string getCountry() const { return country; }
		bool operator==(const Airline &cmp) {
			return code == cmp.code && name == cmp.name 
				&& callsign == cmp.callsign && country == cmp.country;
		}
		bool operator<(const Airline &cmp) const {
			return code != cmp.code ? code < cmp.code : 
				name != cmp.name ? name < cmp.name :
				callsign != cmp.callsign ? callsign < cmp.callsign :
				country < cmp.country; 
		}
};

#endif