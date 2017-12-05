#pragma once
#include "HeaderList.h"

struct Item;
class Location
{
public:
	Location(std::string name, const int itemListSize);
	~Location();

	void Intro();
	void Inspect(const std::string& input);
	void GetAreaDescription();
	const std::string& GetName() { return m_Name; }

	void SetItem(std::string& name, std::string& desc, int idx);
	void SetAreaDescription(std::string& desc);

private:
	std::string m_Name = "";
	std::string m_Description = "";
	int m_NrOfItems = 0;

	std::vector<Item> m_ItemList;
};

