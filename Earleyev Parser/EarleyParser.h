#pragma once

#include "EarleyItem.h"
#include "Grammar.h"

#include <vector>

class EarleyParser
{
public:
	EarleyParser();
	~EarleyParser();

	void parse(Grammar& grammar, std::string& input);
private:
	void buildItems(Grammar& grammar);
	void predict(std::vector<EarleyItem>& stateSet, int stateSetIndex, std::string& symbol, Grammar& grammar);
	void scan(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol, std::string& input, Grammar& grammar);
	void complete(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, Grammar& grammar);
	void createParseTree();

	// helper functions
	void addEarleyItemIfDoesntExist(EarleyItem item, std::vector<EarleyItem>& set);
	void printState();
	void printStateSet(int i);

	std::vector<std::vector<EarleyItem>> m_state;
	std::string m_input;
};

