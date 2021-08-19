#pragma once

#include "EarleyItem.h"
#include "Grammar.h"

#include <vector>
#include <set>

class EarleyParser
{
public:
	EarleyParser(Grammar grammar);
	~EarleyParser();

	void parse(std::string& input);
private:
	void buildItems();
	void predict(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol);
	void scan(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol, std::string& input);
	void complete(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex);

	// helper functions
	void addEarleyItemIfDoesntExist(EarleyItem item, int stateSetIndex);
	void printState();
	void printStateSet(int i);

	std::vector<std::vector<EarleyItem>> m_state;
	std::string m_input;
	std::unordered_set<std::string> m_nullableVariables;
	Grammar m_grammar;
};

