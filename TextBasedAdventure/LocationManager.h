#pragma once
#include "HeaderList.h"

class Location;
class LocationManager
{
public:
	LocationManager(const std::string& filePath);
	~LocationManager();

	int GoToLocation(const std::string& filePath);
	int Inspect(const std::string& input);
	int InspectArea();
	int ShowAllLocations();
	std::string GetCurrentLocation();
	std::vector<std::string> GetAllLocations();

private:
	void CreateLocationsFromFile(const std::string& filePath);
	//std::string Substract(std::string& line, const std::string& part1, const std::string& part2);

	Location* m_CurrentLocationPtr = nullptr;

	typedef std::map<std::string, Location*> LocationList;
	//typedef std::pair<std::string, Location*> LocationPair;
	LocationList m_LocationPtrArr;
	std::vector<std::string> m_LocationNameList;
};

