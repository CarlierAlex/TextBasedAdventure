// TextBasedAdventure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Player.h"
#include "Objects.h"
#include <iostream>

int main()
{
	int scenarioNumber = -1;
	int scenarioMax = 0;
	std::string input = "";
	input = Lamb_GetDataFromFile(std::string("./Resources/ScenarioSelection.txt"));
	scenarioMax = Lamb_GetIntFromString(input, "<max:", ">");
	std::cout << input;

	
	while (scenarioNumber < 0 || scenarioNumber > scenarioMax)
	{
		std::getline(std::cin, input, '\n');
		if(input.size() > 0)
			scenarioNumber = atoi(input.c_str());
	}
	system("CLS");

	//Create	
	Player* m_Player = new Player(std::string("Scenario") + std::to_string(scenarioNumber));

	//Start
	m_Player->Start();
	// Loop
	int feedback = 0;
	while (feedback != -1) {
		//Opties
		feedback = m_Player->Update();
	}

	std::cout << std::endl << "Program ended" << std::endl;
	std::getline(std::cin, input, '\n');
    return 0;
}

