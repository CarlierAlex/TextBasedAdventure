#pragma once
#include "HeaderList.h"
#include <set>
class NPC;

struct Key
{
	std::string m_FilePath;
	NPC* m_NpcTarget;

	Key(std::string filePath, NPC* target = nullptr) :
		m_FilePath(filePath), m_NpcTarget(target){}

	Key() : m_FilePath(), m_NpcTarget(){}
};


class NpcManager
{
public:
	NpcManager(const std::string& filePath);
	~NpcManager();

	bool GetIntro(std::string& input, const std::string& location);
	bool GetAnswers(std::string& input);
	int GetNpcAtLocation(std::string& input);
	bool GetInterogatable();
	bool GetEnd();
	void CheckKey();
	void Reset();

private:
	bool GetLocation(NPC* const npc, const std::string& location);
	void GetQuestion();
	void GetOutro();

	void CreateNPCFromList(const std::string& filePath);
	void CreateNPC(const std::string& npcName, const std::string& filePath);
	void CreateKeys(const std::string& filePath);
	void ReadNpcDialogue(std::string& fileData, NPC* const npc);

	typedef std::map<std::string, NPC*> NpcList;
	typedef std::map<std::string, std::vector<Key>> UpdateKeys;

	UpdateKeys m_KeyList;
	std::set<std::string> m_EndKeys;
	std::string m_FilePathStart = "";
	NpcList m_NpcArr;
	NpcList m_ChangeArr;
	NPC* m_CurrentNpc;
	int m_CurrQuestion = 0;
	int m_CurrAnswer = 0;
};

