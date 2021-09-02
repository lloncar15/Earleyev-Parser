#pragma once

#include "EarleyItem.h"
#include "Grammar.h"
#include "ParseTree.h"

#include <vector>
#include <set>

using namespace std;

class EarleyParser
{
public:
	EarleyParser(Grammar grammar, string input);
	~EarleyParser();

	void parse();
private:
	void buildItems();
	void predict(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, string& symbol);
	void scan(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, string& symbol, string& input);
	void complete(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex);
	void finishBuilding();

	void addEarleyItem(EarleyItem item, int stateSetIndex);
	void printState(const vector<vector<EarleyItem>>& state);
	void printStateSet(const vector<EarleyItem>& set, int i);

	void findNullableVariables();
	bool isVariableNullable(const string& variable);

	void removeIncompleteItems();
	vector<ParseTree*> createTrees(const vector<vector<EarleyItem>>& state, const string& input, unordered_set<ParseTree*>& cachedTrees, int end, string token, ParseTree* parent);
	void printParseTrees();
	void printTree(ParseTree* tree, string indent, bool isLast);

	ParseTree* createTree(const vector<vector<EarleyItem>>& state, const string& input, int start, const string& token, ParseTree* parent);
	vector<vector<EarleyItem>> orderStateByStart(const vector<vector<EarleyItem>>& state);

	vector<vector<EarleyItem>> m_state;
	string m_input;
	Grammar m_grammar;

	unordered_set<string> m_nullableVariables;

	vector<ParseTree*> m_parseTrees;
};

