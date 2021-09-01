#pragma once

#include "EarleyItem.h"

#include <vector>

using namespace std;

class ParseTree
{
public:
	ParseTree(ParseTree* parent, const vector<ParseTree*> children, EarleyItem earleyItem, int start, int end, int length, string data);
	~ParseTree() {};

	bool isMatchingItem(EarleyItem item, int start);

	ParseTree* m_parent;
	vector<ParseTree*> m_children;
	string m_variable;
	vector<string> m_symbols;
	int m_start = 0;
	int m_end = 0;
	int m_length = 0;
	string m_data;
};

