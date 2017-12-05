#include "stdafx.h"
#include "NpcManager.h"
#include "Objects.h"
using namespace std;

//CONSTRUCTOR
//****************************************************************************************************************************************
NpcManager::NpcManager(const std::string& filePath)
{
	m_FilePathStart = filePath;
	CreateNPCFromList(filePath + string("Scenario.txt"));
}

//DESTRUCTOR
//****************************************************************************************************************************************
NpcManager::~NpcManager()
{
	m_CurrentNpc = nullptr;
	m_KeyList.clear();
	m_NpcArr.clear();
	m_ChangeArr.clear();
}

//CREATE NPCs FROM LIST
//****************************************************************************************************************************************
void NpcManager::CreateNPCFromList(const std::string& filePath)
{
	string fileData = Lamb_GetDataFromFile(filePath);
	string newfilePath = filePath;
	string remove = string("Scenario.txt");
	newfilePath.replace(newfilePath.find(remove), remove.length(), "");

	//CREATE WHILE LIST IS NOT EMPTY
	//-------------------------------------------------------------
	while (fileData.empty() == false)
	{
		string npcName = Lamb_SubstractString(fileData, string("<"), string(">"));

		if (npcName.length() > 0)
		{
			//CREATE NPC
			//-------------------------------------------------------------
			CreateNPC(npcName, newfilePath + npcName + string(".txt"));
		}
	}
	//CREATE KEY LIST
	//-------------------------------------------------------------
	CreateKeys(newfilePath + string("Keys") + string(".txt"));
	fileData.clear();
}

//CREATE NPC
//*******************************************************************************************************************************
void NpcManager::CreateNPC(const std::string& npcName, const std::string& filePath)
{
	//CREATE NPC FROM FILE
	//-------------------------------------------------------------
	string fileData = Lamb_GetDataFromFile(filePath);

	NPC* npc = new NPC(npcName, false);
	m_NpcArr.insert({ npcName , npc });
	ReadNpcDialogue(fileData, npc);
}

//READ NPC DIALOGUE + INFO
//*******************************************************************************************************************************
void NpcManager::ReadNpcDialogue(std::string& fileData, NPC* const npc)
{
	//READ DIALOGUE FROM FILE
	//-------------------------------------------------------------
	bool foundEnd = false;
	int countQuestion = 0;
	auto npcDialogue = npc->GetDialogue();

	string result;
	string line;
	vector<string> answerArr;
	string findPart;

	//NPC INFO
	//======================================================================================================
	//INTEROGATABLE
	//-------------------------------------------------------------
	findPart = string("[") + npc->GetName() + string(":questionable=");
	if (Lamb_GetboolFromString(fileData, findPart, string("]")))
		npc->SetInterogatble(true);
	else
		npc->SetInterogatble(false);

	//LOCATION
	//-------------------------------------------------------------
	result = Lamb_SubstractString(fileData, string("<location:"), string("/end>"));
	npc->SetLocation(result);

	//VERSION
	//-------------------------------------------------------------
	int version = Lamb_GetIntFromString(fileData, string("<version:"), string("/end>"));
	npc->SetVersion(version);

	//QUESTIONS
	//-------------------------------------------------------------
	int questionAmount = Lamb_GetIntFromString(fileData, string("<amount:"), string("/end>"));
	npcDialogue->SetQuestionAmount(questionAmount);

	//ENDING
	//-------------------------------------------------------------
	bool isEnd = Lamb_GetboolFromString(fileData, string("<ending:"), string("/end>"));
	if (isEnd)
	{
		m_EndKeys.insert({ Lamb_GetKey(npc, false) });
	}

	//DIALOGUE
	//======================================================================================================
	//INTRO
	//-------------------------------------------------------------
	result = Lamb_SubstractString(fileData, string("<INTRO:"), string("/end>"));
	npcDialogue->SetIntro(result);
	
	if (npc->GetInterogatble() == false)
	{
		fileData.clear();
		return;
	}

	//REMAINING
	//-------------------------------------------------------------
	while (fileData.empty() == false)
	{
		answerArr.clear();
		if (countQuestion < npcDialogue->GetQuestionAmount())
		{
			//Questions
			//-------------------------------------------------------------
			findPart = string("<Q") + std::to_string(countQuestion + 1) + string(":");
			result = Lamb_SubstractString(fileData, findPart, string("/end>"));
			npcDialogue->SetQuestion(countQuestion, result);

			//Answers
			//-------------------------------------------------------------
			for (int countOption = 0; countOption < MAX_ANSWERS; countOption++)
			{
				findPart = string("<Q") + std::to_string(countQuestion + 1) + string("-") + std::to_string(countOption + 1) + string(":");
				result = Lamb_SubstractString(fileData, findPart, string("/end>"));
				if(result.length() > 1)
					answerArr.push_back(result);
			}
			npcDialogue->SetAnswers(countQuestion, answerArr);
		}
		else if (countQuestion > npcDialogue->GetQuestionAmount())
		{
			//Outro
			//-------------------------------------------------------------
			result = Lamb_SubstractString(fileData, string("<OUTRO:"), string("/end>"));
			npcDialogue->SetOutro(result);

			//Clear
			//-------------------------------------------------------------
			fileData.clear();
		}
		countQuestion++;
	}

	fileData.clear();
}

void NpcManager::CreateKeys(const std::string& filePath)
{
	//KEY LIST FROM FILE
	//=========================================================================================================================
	string fileData = Lamb_GetDataFromFile(filePath);
	string findPart;
	string result;

	//EXTRACT KEY
	//-------------------------------------------------------------
	while (fileData.empty() == false)
	{
		result = Lamb_SubstractString(fileData, string("{"), string("}"));
		string keyName = Lamb_SubstractString(result, string("<:"), string(">"));

		if (keyName.length() > 0)
		{
			Key key;
			//Get + store key
			//-------------------------------------------------------------
			std::vector<Key> keyList;
			findPart = string("[");
			string name = Lamb_SubstractString(result, findPart, string("]"));
			auto it = m_NpcArr.find(name);
			if (it == m_NpcArr.end())
				continue;

			key.m_NpcTarget = it->second;

			findPart = string("</");
			key.m_FilePath = m_FilePathStart + Lamb_SubstractString(result, findPart, string("/end>"));



			auto id = m_KeyList.find(keyName);
			if (id != m_KeyList.end())
			{
				id->second.push_back(key);
			}
			else
			{
				keyList.push_back(key);
				m_KeyList.insert({ keyName , keyList });
			}
		}
		else
		{
			//Clear at finish
			//-------------------------------------------------------------
			fileData.clear();
		}
	}
}

//CHECK (possible) KEY
//****************************************************************************************************************************************
void NpcManager::CheckKey()
{
	//CHECK IN KEY LIST
	//=========================================================================================================================
	string key = Lamb_GetKey(m_CurrentNpc, m_CurrQuestion, m_CurrAnswer, true);
	auto it = m_KeyList.find(key);

	if (it != m_KeyList.end())
	{
		auto list = it->second;
		//Get event triggered by key
		//-------------------------------------------------------------
		for (auto p = 0; p < list.size(); p++)
		{
			auto keyNew = list[p];
			string filePath = keyNew.m_FilePath;
			string fileData = Lamb_GetDataFromFile(filePath);

			//Store triggered key to review after conversation
			//-------------------------------------------------------------
			m_ChangeArr.insert({ fileData, keyNew.m_NpcTarget });
		}
	}

	if (it != m_KeyList.end())
		m_KeyList.erase(it);
}

//GET INTRO
//****************************************************************************************************************************************
bool NpcManager::GetIntro(std::string& input, const std::string& location)
{
	//GET NAME
	//-------------------------------------------------------------
	auto name = Lamb_SubstractString(input, string("talk "), string(" "));

	//CHECK IF CHARACTER EXISTS
	//-------------------------------------------------------------
	auto it = m_NpcArr.find(name);
	if (it != m_NpcArr.end())
	{
		//Check location
		//-------------------------------------------------------------
		auto npc = ((*it).second);
		if (GetLocation(npc, location) == true)
		{
			//Get intro
			//-------------------------------------------------------------
			cout << npc->GetDialogue()->GetIntro();

			//Setup for conversation if possible
			//-------------------------------------------------------------
			m_CurrentNpc = npc;
			if (npc->GetInterogatble())
			{
				getchar(); //pause between parts of conversation, for readability
				GetQuestion();
				return true;
			}
		}
	}
	else
	{
		cout << "Who are u talking about" << endl;
	}
	 
	return false;
}


void NpcManager::GetQuestion()
{
	//GET QUESTION
	//-------------------------------------------------------------
	if (m_CurrentNpc != nullptr)
	{
		if (m_CurrentNpc->GetInterogatble())
		{
			string text = m_CurrentNpc->GetDialogue()->GetQuestion(m_CurrQuestion);
			cout << m_CurrentNpc->GetDialogue()->GetQuestion(m_CurrQuestion);
		}
	}
}

//GET LOCATION
//****************************************************************************************************************************************
bool NpcManager::GetLocation(NPC* const npc, const std::string& location)
{
	//CHECK LOCATION + FEEDBACK
	//-------------------------------------------------------------
	if (location == "")
	{
		cout << "You're nowhere" << endl;
		return false;
	}

	if (npc->GetLocation() == location)
		return true;
	else
		cout << "This person is not here" << endl;
	return false;
}

//GET ANSWER
//****************************************************************************************************************************************
bool NpcManager::GetAnswers(std::string& input)
{
	if (m_CurrentNpc != nullptr)
	{
		//Check if interogatable
		//-------------------------------------------------------------
		if (m_CurrentNpc->GetInterogatble())
		{
			//Get option number
			//-------------------------------------------------------------
			string result;
			result = Lamb_SubstractString(input, string("option "), string("  "));
			if (result.length() <= 0)
				result = Lamb_SubstractString(input, string("option"), string("  "));
			int answerIdx = atoi(result.c_str()) - 1;

			//Get corresponding answer
			//-------------------------------------------------------------
			if(m_CurrQuestion >= 0 && answerIdx  >= 0)
				result = m_CurrentNpc->GetDialogue()->GetAnswers(m_CurrQuestion, answerIdx);

			//Check if something follows
			//-------------------------------------------------------------
			if (result.empty() == false && result.size() > 0)
			{
				m_CurrAnswer = answerIdx;
				CheckKey(); // check for possible key triggers

				cout << result;
				m_CurrQuestion++;

				//Get next question OR outro/end
				//-------------------------------------------------------------
				if (m_CurrQuestion < m_CurrentNpc->GetDialogue()->GetQuestionAmount())
				{
					getchar(); //pause between parts of conversation, for readability
					GetQuestion();
					return false;
				}
				else
				{
					getchar(); //pause between parts of conversation, for readability
					GetOutro();
					Reset();
					return true;
				}
			}
			else
			{
				cout << string("What do u mean?") << endl;
				return false;
			}
		}
	}
	return false;
}

//GET OUTRO
//****************************************************************************************************************************************
void NpcManager::GetOutro()
{
	if (m_CurrentNpc != nullptr)
	{
		if (m_CurrentNpc->GetInterogatble())
			cout << m_CurrentNpc->GetDialogue()->GetOutro();
	}
}

//CHECK INTEROGATABLE
//****************************************************************************************************************************************
bool NpcManager::GetInterogatable()
{
	if (m_CurrentNpc != nullptr)
	{
		return m_CurrentNpc->GetInterogatble();
	}
	return false;
}

int NpcManager::GetNpcAtLocation(std::string& input)
{
	for each(auto npc in m_NpcArr)
	{
		if (npc.second->GetLocation() == input)
			cout << npc.first << endl;
	}
	return 0;
}

//RESET FOR (next conversation)
//****************************************************************************************************************************************
void NpcManager::Reset()
{
	m_CurrentNpc = nullptr;
	m_CurrQuestion = 0;

	for each (auto change in m_ChangeArr)
	{
		auto data = change.first;
		ReadNpcDialogue(data, change.second);
	}
	m_ChangeArr.clear();
}

//CHECK IF IS END CONVERSATION
//****************************************************************************************************************************************
bool NpcManager::GetEnd()
{
	if (m_CurrentNpc)
	{
		string endKey = m_CurrentNpc->GetName();
		endKey += std::to_string(m_CurrentNpc->GetVersion());

		if (m_EndKeys.find(endKey) != m_EndKeys.end() && endKey.length() > 0)
			return true;
	}

	return false;
}

