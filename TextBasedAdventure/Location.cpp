#include "stdafx.h"
#include "Location.h"
#include "Objects.h"

//CONSTRUCTOR
//****************************************************************************************************************************************
Location::Location(std::string name, const int itemListSize)
{
	m_Name = name;
	m_NrOfItems = itemListSize;
}

//DESTRUCTOR
//****************************************************************************************************************************************
Location::~Location()
{
	m_ItemList.clear();
}

//INTRO
//****************************************************************************************************************************************
void Location::Intro()
{
	std::cout << "U arrived at the " << m_Name.c_str() << std::endl;
	GetAreaDescription();
}

//INSPECT PART OF THE AREA
//****************************************************************************************************************************************
void Location::Inspect(const std::string& input)
{
	int idx = input.find("inspect ");
	std::string name = input.substr(input.find(" ", idx) + 1, std::string::npos);

	for (size_t i = 0; i < m_ItemList.size(); i++)
	{
		if (m_ItemList[i].m_NameItem == name && m_ItemList[i].m_Unlocked == true)
			std::cout << m_ItemList[i].m_InfoItem << std::endl;
	}
}

//GET GENERAL AREA DESCRIPTION
//****************************************************************************************************************************************
void Location::GetAreaDescription()
{
	std::cout << m_Description.c_str() << std::endl;
}

//ADD ITEM
//****************************************************************************************************************************************
void Location::SetItem(std::string& name, std::string& desc, int idx)
{
	Item item (name , desc);
	m_ItemList.push_back(item);
}

//SET GENERAL DESCRIPTION
//****************************************************************************************************************************************
void Location::SetAreaDescription(std::string& desc)
{
	m_Description = desc;
}