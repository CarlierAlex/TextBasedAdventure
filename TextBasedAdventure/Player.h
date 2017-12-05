#pragma once
#include "HeaderList.h"

class LocationManager;
class NpcManager;

enum PlayerState 
{ 
	IDLE = 0,
	TALKING = 1,
	END = 2
};

class Player
{
public:
	Player(std::string scenario);
	~Player();

	void Start();
	int Update();

private:
	void Intro();
	void Help();
	void CreateCommandList();
	int TalkToNpc(std::string& input);
	int PlayerAction(std::string& input);
	int GetCommand(std::string input);

	PlayerState m_PlayerState = PlayerState::IDLE;
	LocationManager* m_LocationManagerPtr = nullptr;
	NpcManager* m_NpcManagerPtr = nullptr;
	std::string m_FilePathStart = "";

	std::map<std::string, short> m_CommandList;
};

