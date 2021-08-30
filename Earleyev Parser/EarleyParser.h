#pragma once

#include "EarleyItem.h"
#include "Grammar.h"
#include "ParseTree.h"

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
	void finishRecogniser();

	void addEarleyItemIfDoesntExist(EarleyItem item, int stateSetIndex);
	void printState(const std::vector<std::vector<EarleyItem>>& state);
	void printStateSet(const std::vector<EarleyItem>& set, int i);

	void findNullableVariables();
	bool isVariableNullable(const std::string& variable);

	void removeUncompletedItems();
	std::vector<ParseTree*> createTrees(const std::vector<std::vector<EarleyItem>>& state, const std::string& input, std::unordered_set<ParseTree*>& cachedTrees, int end, std::string token, ParseTree* parent);
	void printParseTrees();
	void printTree(ParseTree* tree, std::string indent, bool isLast);

	ParseTree* createTree(const std::vector<std::vector<EarleyItem>>& state, const std::string& input, int start, const std::string& token, ParseTree* parent);
	std::vector<std::vector<EarleyItem>> orderStateByStart(const std::vector<std::vector<EarleyItem>>& state);

	std::vector<std::vector<EarleyItem>> m_state;
	std::string m_input;
	Grammar m_grammar;

	std::unordered_set<std::string> m_nullableVariables;

	std::vector<ParseTree*> m_parseTrees;
};

