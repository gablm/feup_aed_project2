#ifndef AIRPORT_H_
#define AIRPORT_H_

#include <string>

class Airport {
	private:
		std::string code;
		std::string name;
		std::string city;
		std::string country;
		double latitude;
		double longitude;
	public:
		Airport() {}
		Airport(std::string code) : code(code) {}
		Airport(std::string code, std::string name, std::string city, 
			std::string country, double latitude, double longitude) : 
			code(code), name(name), city(city), country(country), 
			latitude(latitude), longitude(longitude) {}
		std::string getCode() const {
			return code;
		}
		std::string getName() const {
			return name;
		}
		std::string getCity() const {
			return city;
		}
		std::string getCountry() const {
			return country;
		}
		double getLatitude() const {
			return latitude;
		}
		double getLongitude() const {
			return longitude;
		}
		bool operator==(const Airport &rhs) const {
			return code == rhs.code;
		}
		bool operator!=(const Airport &rhs) const {
			return code != rhs.code;
		}
		bool operator<(const Airport &cmp) const {
			return code != cmp.code ? code < cmp.code : name < cmp.name;
		}
};

#endif