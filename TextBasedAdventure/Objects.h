#pragma once
#include "HeaderList.h"
const int MAX_QUESTIONS = 7;
const int MAX_ANSWERS = 3;

struct Item
{
	std::string m_NameItem;
	std::string m_InfoItem;
	bool m_Unlocked = true;

	Item(std::string name, std::string info, bool unlock = true) :
		m_NameItem(name), m_InfoItem(info), m_Unlocked(unlock) {}

	Item() : m_NameItem(), m_InfoItem(), m_Unlocked() {}
};

class Dialogue
{
public:
	Dialogue():
		m_Intro(""),
		m_Outro(""),
		m_Fail ("")
	{

	}
	~Dialogue()
	{
		m_QuestionArr.clear();
		for (size_t i = 0; i < m_AnswerArr.size(); i++)
		{
			m_AnswerArr[i].clear();
		}
		m_AnswerArr.clear();
	}

	void SetIntro(std::string intro) { m_Intro = intro; }
	void SetQuestionAmount(int amount) 
	{ 
		m_QuestionArr.clear();
		m_QuestionArr.resize(amount);

		m_AnswerArr.clear();
		m_AnswerArr.resize(amount);
	}
	void SetQuestion(short idx, std::string question) { if ((size_t)idx < m_QuestionArr.size()) m_QuestionArr[idx] = question; }
	void SetAnswers(short idx, std::vector<std::string> answers) { if ((size_t)idx < m_AnswerArr.size()) m_AnswerArr[idx] = answers; }
	void SetOutro(std::string outro) { m_Outro = outro; }

	const std::string& GetIntro() { return m_Intro; }
	const std::string& GetQuestion(short idx) { return m_QuestionArr[idx]; }
	const std::string& GetAnswers(short idx, short answer)
	{
		if ((size_t)idx < m_AnswerArr.size())
			if ((size_t)answer < m_AnswerArr[idx].size())
				return (m_AnswerArr[idx])[answer];
		return m_Fail;
	}
	const std::string& GetOutro() { return m_Outro; }
	const int GetQuestionAmount() { return (int)m_QuestionArr.size(); }

	void Clear()
	{
		m_QuestionArr.clear();
		m_AnswerArr.clear();
		m_Intro.clear();
		m_Outro.clear();
	}


	Dialogue(const Dialogue& other) : // Copy constructor
		m_Intro(other.m_Intro),
		m_Outro(other.m_Outro),
		m_Fail("")
	{
		m_QuestionArr.clear();
		m_AnswerArr.clear();
		m_QuestionArr = other.m_QuestionArr;
		m_AnswerArr = other.m_AnswerArr;
	}

	Dialogue& operator= (const Dialogue& other) // Copy assignment operator
	{
		m_Intro = other.m_Intro;
		m_Outro = other.m_Outro;

		m_QuestionArr.clear();
		m_AnswerArr.clear();
		m_QuestionArr = other.m_QuestionArr;
		m_AnswerArr = other.m_AnswerArr;
		return *this; //dreference
	}

private:
	std::string m_Intro;
	std::vector<std::string> m_QuestionArr;
	std::vector<std::vector<std::string>> m_AnswerArr;
	std::string m_Outro;
	const std::string m_Fail;
};

class NPC
{
public:
	NPC(std::string name, bool interrogatble = false) :
		m_NameNPC(name), m_Interrogatable(interrogatble), m_NpcDDialogue(new Dialogue())
	{
		//m_NpcDDialogue = new Dialogue();
	}
	~NPC()
	{
		delete m_NpcDDialogue;
	}

	//void SetDialogue(Dialogue* dialogue) { m_NpcDDialogue = dialogue; }
	void SetInterogatble(bool inter) { m_Interrogatable = inter; }
	void SetLocation(std::string name) { m_LocationNPC = name; }
	void SetVersion(int version) { m_Version = version; }

	Dialogue* const GetDialogue() { return m_NpcDDialogue; }
	bool GetInterogatble() { return m_Interrogatable; }
	const std::string& GetLocation() { return m_LocationNPC; }
	const std::string& GetName() { return m_NameNPC; }
	const int& GetVersion() { return m_Version; }

	void Clear()
	{
		//m_NpcDDialogue->Clear();
		//m_LocationNPC.clear();
		//m_NameNPC.clear();
		//m_Interrogatable = false;
		//m_Version = 0;
	}

	NPC(const NPC& other) : // Copy constructor
		m_LocationNPC(other.m_LocationNPC),
		m_NameNPC(other.m_NameNPC),
		m_Interrogatable(other.m_Interrogatable),
		m_NpcDDialogue(other.m_NpcDDialogue),
		m_Version(other.m_Version)
	{

	}

	NPC& operator= (const NPC& other) // Copy assignment operator
	{
		m_LocationNPC = other.m_LocationNPC;
		m_NameNPC = other.m_NameNPC;
		m_Interrogatable = other.m_Interrogatable;
		delete m_NpcDDialogue;
		m_NpcDDialogue = new Dialogue(*other.m_NpcDDialogue);
		m_Version = other.m_Version;
		return *this; //dreference
	}

private:
	std::string m_LocationNPC;
	std::string m_NameNPC;
	bool m_Interrogatable = true;
	Dialogue* m_NpcDDialogue;
	int m_Version = 0;
};

const auto Lamb_SubstractString = [](std::string& line, const std::string& part1, const std::string& part2)
{
	size_t idxPart1 = 0, idxPart2 = 0;
	unsigned int sizePart1 = 0;
	size_t size = 0;
	std::string sub = "";

	//Name
	idxPart1 = line.find(part1);
	sizePart1 = part1.size();

	if (idxPart1 != std::string::npos)
	{
		idxPart2 = line.find(part2, idxPart1 + sizePart1);
		size = idxPart2 - idxPart1 - sizePart1;
		if (size < 0)
			size = 0;


		sub = line.substr(idxPart1 + sizePart1, size);

		if(part2.size() > 0)
			line.replace(idxPart1, size + part1.size() + part2.size(), "");
		else
			line.replace(idxPart1, size + part1.size(), "");

		int idxNext = line.find("\n");
		if (idxNext != std::string::npos)
			line.replace(idxNext, 1, "");
	}

	return sub;
};

const auto Lamb_GetKey = [](NPC* npc, int question = 0, int answer = 0, bool specific = false)
{
	std::string key;

	if (npc != nullptr)
	{
		key += npc->GetName();
		key += std::to_string(npc->GetVersion());

		if (specific)
		{
			key += std::string(".");
			key += std::to_string(question + 1);
			key += std::string(".");
			key += std::to_string(answer + 1);
		}
	}

	return key;
};

const auto Lamb_GetIntFromString = [](std::string& fileData, const std::string& findPart1, const std::string& findPart2)
{
	int result = 0;
	std::string resultString;
	if (fileData.find(findPart1) != std::string::npos)
	{
		resultString = Lamb_SubstractString(fileData, findPart1, findPart2);
		result = atoi(resultString.c_str());
	}
	return result;
};

const auto Lamb_GetboolFromString = [](std::string& fileData, const std::string& findPart1, const std::string& findPart2)
{
	bool result = false;
	std::string resultString;
	if (fileData.find(findPart1) != std::string::npos)
	{
		resultString = Lamb_SubstractString(fileData, findPart1, findPart2);
		if (resultString.find("true") != std::string::npos)
		{
			result = true;
		}
	}
	return result;
};

const auto Lamb_GetDataFromFile = [](const std::string& filePath)
{
	std::ifstream file(filePath);
	assert(file.is_open()); //breaks when not true
	std::string line;
	std::string fileData;

	while (!file.eof())
	{
		line.clear();
		getline(file, line);
		fileData += line;
		fileData += '\n';
	}

	file.close();
	return fileData;
};