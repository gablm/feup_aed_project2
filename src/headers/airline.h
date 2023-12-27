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
		Airline() {}
		Airline(std::string code, std::string name, std::string callsign, std::string country);
		std::string getCode() const;
		std::string getName() const;
		std::string getCallSign() const;
		std::string getCountry() const;
		bool operator==(const Airline &cmp);
		bool operator<(const Airline &cmp) const;
};

#endif