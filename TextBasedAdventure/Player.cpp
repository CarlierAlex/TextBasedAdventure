#include "stdafx.h"
#include "Player.h"

#include "LocationManager.h"
#include "NpcManager.h"
#include "Objects.h"
using namespace std;


//CONSTRUCTOR
//*******************************************************************************************************************************
Player::Player(std::string scenario)
{
	m_FilePathStart = string("./Resources/") + scenario + string("/");
	m_LocationManagerPtr = new LocationManager(m_FilePathStart + string("Locations.txt"));
	m_NpcManagerPtr = new NpcManager(m_FilePathStart);

	CreateCommandList();
}

//DESTRUCTOR
//*******************************************************************************************************************************
Player::~Player()
{
	delete m_LocationManagerPtr;
	delete m_NpcManagerPtr;
}

//START
//*******************************************************************************************************************************
void Player::Start()
{
	Intro();
	Help();

	string input = "";
	getline(std::cin, input, '\n'); //Prevent immediate text, getline is smoother that getchar
}

//UPDATE
//*******************************************************************************************************************************
int Player::Update()
{
	//Start of next action
	//-------------------------------------------------------------
	if (m_PlayerState == 0)
	{
		cout << endl << endl;
		cout << string("What to do next?") << endl;
	}
	int feedback = 0;

	//Get input
	//-------------------------------------------------------------
	string input = "";
	getline(std::cin, input, '\n');

	//Result input
	//-------------------------------------------------------------
	switch (m_PlayerState)
	{
	case (int)PlayerState::IDLE:
		feedback = PlayerAction(input);
		break;
	case (int)PlayerState::TALKING:
		feedback = TalkToNpc(input);
		break;
	case (int)PlayerState::END:
		feedback = TalkToNpc(input);
		break;
	default:
		cout << "Technical difficulties" << endl;
		break;
	}
	return feedback;
}

//ACTIONS
//*******************************************************************************************************************************
int Player::PlayerAction(std::string& input)
{
	//Result input
	//-------------------------------------------------------------
	short commandInt = GetCommand(input);
	int feedback = 0;

	//Act to command
	//-------------------------------------------------------------
	switch (commandInt)
	{
	case 10: //Stop game
		return -1;
		break;
	case 1: //Talk to people -> will start conversation if possible
		if (m_NpcManagerPtr->GetIntro(input, m_LocationManagerPtr->GetCurrentLocation()))
		{
			m_PlayerState = PlayerState::TALKING;
		}
		if (m_NpcManagerPtr->GetEnd())
		{
			m_PlayerState = PlayerState::END;
			if (m_NpcManagerPtr->GetInterogatable() == false)
				return -1;
		}

		break;
	case 3: //Change location
		feedback = m_LocationManagerPtr->GoToLocation(input);
		break;
	case 4: //Detailed info about part in area
		feedback = m_LocationManagerPtr->Inspect(input);
		break;
	case 5: //Area description
		feedback = m_LocationManagerPtr->InspectArea();
		break;
	case 6: //Show all possible locations
		feedback = m_LocationManagerPtr->ShowAllLocations();
		break;
	case 7: //Show all NPCs in this area
		feedback = m_NpcManagerPtr->GetNpcAtLocation(m_LocationManagerPtr->GetCurrentLocation());
		break;
	case 8: //Help
		Help();
		feedback = 0;
		break;
	default: //When faulty input is given
		cout << "No, I can't do that" << endl;
		break;
	}

	//Return feedback for program
	//-------------------------------------------------------------
	return feedback;
}

//CONVERSATION
//*******************************************************************************************************************************
int Player::TalkToNpc(std::string& input)
{
	//Get command
	//-------------------------------------------------------------
	string result = "";
	short commandInt = GetCommand(input);
	int feedback = 0;

	//Follow command
	//-------------------------------------------------------------
	switch (commandInt)
	{
	case 10: //Stop program
		return -1;
		break;
	case 2: //Question NPCs -> end of conversation, next possible question or game end occurs
		if (m_NpcManagerPtr->GetAnswers(input))
		{
			if (m_PlayerState != PlayerState::END)
			{
				m_PlayerState = PlayerState::IDLE;
			}
			else
			{
				cout << "You won!" << endl;
				feedback = -1;
			}
		}
		break;
	case 9: //Stop conversation, only if possible
		if (m_PlayerState != PlayerState::END)
		{
			m_PlayerState = PlayerState::IDLE;
			m_NpcManagerPtr->Reset();
		}
		else
		{
			cout << "You can't leave." << endl;
		}
		break;
	case 8: //Help
		Help();
		feedback = 0;
		break;
	default: //When faulty input is given
		cout << "What are u trying to do?" << endl;
		break;
	}

	//Return feedback for program
	//-------------------------------------------------------------
	return feedback;
}

//GET COMMAND ID
//*******************************************************************************************************************************
int Player::GetCommand(std::string input)
{
	string command = "";
	command = input.substr(0, input.find("  "));
	for each (auto com in m_CommandList)
	{
		string text = com.first;
		if (command.find(text) != string::npos)
			return com.second;
	}
	return -1;
}

//CREATE COMMANDS LIST
//*******************************************************************************************************************************
void Player::CreateCommandList()
{
	m_CommandList.insert({ string("end "),  10 });
	m_CommandList.insert({ string("talk "), 1 });
	m_CommandList.insert({ string("option"),  2 });
	m_CommandList.insert({ string("go "),  3 });
	m_CommandList.insert({ string("inspect"), 4 });
	m_CommandList.insert({ string("area"), 5 });
	m_CommandList.insert({ string("show locations"), 6 });
	m_CommandList.insert({ string("show people"), 7 });
	m_CommandList.insert({ string("help"), 8 });
	m_CommandList.insert({ string("stop talking"), 9 });
}

//INTRO
//*******************************************************************************************************************************
void Player::Intro()
{
	string filePath = m_FilePathStart + string("GameIntro.txt");
	ifstream file(filePath);
	assert(file.is_open()); //breaks when not true
	string line;
	string fileData;
	std::vector<string> introParts;

	//Read Intro
	//-------------------------------------------------------------
	while (!file.eof())
	{
		line.clear();
		getline(file, line);

		if (line.find("/new") != string::npos)
		{
			introParts.push_back(fileData);
			fileData = "";
		}
		else if (line.find("  ") != 0)
		{
			fileData += line;
			fileData += '\n';
		}
	}
	file.close();

	//Output Intro with pauzes
	//-------------------------------------------------------------
	for (size_t i = 0; i < introParts.size(); i++)
	{
		cout << introParts[i];
		cout << endl;
		getline(std::cin, line, '\n');
	}
}

//HELP
//*******************************************************************************************************************************
void Player::Help()
{
	string filePath = m_FilePathStart + string("Help.txt");
	string fileData = Lamb_GetDataFromFile(filePath);
	cout << fileData;
}
