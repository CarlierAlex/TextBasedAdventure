#include "stdafx.h"
#include "LocationManager.h"
#include "Location.h"
#include "Objects.h"
using namespace std;

//CONSTRUCTOR
//****************************************************************************************************************************************
LocationManager::LocationManager(const std::string& filePath)
{
	CreateLocationsFromFile(filePath);
}

//DESTRUCTOR
//****************************************************************************************************************************************
LocationManager::~LocationManager()
{
	m_CurrentLocationPtr = nullptr;
}

//CHANGE LOCATION
//****************************************************************************************************************************************
int  LocationManager::GoToLocation(const std::string& input)
{
	int idx = input.find("go");

	//---------------------------------------------------------------------------------------------------------------------------
	string name = input.substr(input.find(" ", idx) + 1, string::npos);
	auto it = m_LocationPtrArr.find(name);
	Location * pLoc = NULL;
	if (it != m_LocationPtrArr.end())
	{
		pLoc = ((*it).second);
		pLoc->Intro();
		m_CurrentLocationPtr = pLoc;
		return 0;
	}
	else
		cout << "Location not found" << endl;

	return 1;
}

//SHOW ALL LOCATIONS
//****************************************************************************************************************************************
int LocationManager::ShowAllLocations()
{
	//---------------------------------------------------------------------------------------------------------------------------
	if (m_LocationPtrArr.size() <= 0)
	{
		return 1;
		cout << "No locations exist" << endl;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	for each (auto it in m_LocationPtrArr)
		cout << it.first << endl;
	cout << endl;

	return 0;
}

//CREATE LOCATIONS FROM FILE
//****************************************************************************************************************************************
void LocationManager::CreateLocationsFromFile(const std::string& filePath)
{
	ifstream file(filePath);
	assert(file.is_open()); //breaks when not true
	string line;

	string name = "";
	int nrOfItems = 0;
	string desc = "";

	size_t idxArr = -1;
	size_t idxItem = 0;
	size_t idxString = 0;

	//LOCATIONS FROM FILE
	//===============================================================================================================
	while (!file.eof())
	{
		line.clear();
		getline(file, line);

		//LOCATION
		//-------------------------------------------------------------------------------------------
		if (line.find("loc:") != string::npos)
		{
			idxItem = 0;
			idxArr++;

			//Name
			//-------------------------------------------------------------
			name = Lamb_SubstractString(line, string("loc: "), string("  "));

			//Number 0f items
			//-------------------------------------------------------------
			line.clear();
			getline(file, line);
			string nr = Lamb_SubstractString(line, string("nrOfItems: "), string("  "));

			//Create location
			//-------------------------------------------------------------
			nrOfItems = atoi(nr.c_str());
			if (name.length() > 0 && nrOfItems >= 0)
			{
				m_LocationPtrArr.insert({ name, new Location(name, nrOfItems) });
				m_LocationNameList.push_back(name);
			}
		}

		//ITEMS AT CURRENT LOCATION
		//-------------------------------------------------------------------------------------------
		else if (line.find("item: ") != string::npos && idxArr >= 0)
		{
			//Name
			//-------------------------------------------------------------
			name = Lamb_SubstractString(line, string("item: "), string("|"));

			//Desc
			//-------------------------------------------------------------
			desc = Lamb_SubstractString(line, string(":"), string(";"));

			//Give to array
			//-------------------------------------------------------------
			auto it = m_LocationPtrArr.find(m_LocationNameList[idxArr]);
			if (it != m_LocationPtrArr.end() && name.length() > 0 && desc.length() > 0)
			{
				auto pLoc = ((*it).second);
				pLoc->SetItem(name, desc, idxItem);
			}

			//Next
			//-------------------------------------------------------------
			idxItem++;
		}

		//DESCRIPTION OF CURRENT LOCATION
		//-------------------------------------------------------------------------------------------
		else if (line.find("desc:") != string::npos)
		{
			//Description
			//-------------------------------------------------------------
			string info = line;
			while (info.find("]") == string::npos)
			{
				getline(file, line);
				info += line;
				info += '\n';
			}
			desc = Lamb_SubstractString(info, string("["), string("]"));

			//Set description
			//-------------------------------------------------------------
			auto it = m_LocationPtrArr.find(m_LocationNameList[idxArr]);
			if (it != m_LocationPtrArr.end() && desc.length() > 0)
			{
				auto pLoc = ((*it).second);
				pLoc->SetAreaDescription(desc);
			}
		}

		//Clear
		//-------------------------------------------------------------
		name.clear();
		desc.clear();
		nrOfItems = -1;
	}
	file.close();
}

//INSPECT SPECIFIC PART OF THE LOCATION
//****************************************************************************************************************************************
int LocationManager::Inspect(const std::string& input)
{
	//---------------------------------------------------------------------------------------------------------------------------
	if (m_CurrentLocationPtr != nullptr)
	{
		m_CurrentLocationPtr->Inspect(input);
		return 1;
	}
	return 1;
}

//GET GENERAL DESCRIPTION OF THE AREA
//****************************************************************************************************************************************
int LocationManager::InspectArea()
{
	//---------------------------------------------------------------------------------------------------------------------------
	if (m_CurrentLocationPtr != nullptr)
	{
		m_CurrentLocationPtr->GetAreaDescription();
		return 1;
	}
	return 1;
}

//CHECK IF IS END CONVERSATION
//****************************************************************************************************************************************
std::string LocationManager::GetCurrentLocation()
{ 
	if (m_CurrentLocationPtr != nullptr)
		return m_CurrentLocationPtr->GetName();
	return string("");
}

//GET LIST OF ALL LOCATIONS
//****************************************************************************************************************************************
std::vector<std::string> LocationManager::GetAllLocations()
{
	return m_LocationNameList;
}