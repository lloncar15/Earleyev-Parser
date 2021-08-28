#pragma once

#include "EarleyItem.h"

#include <vector>

class ParseTree
{
public:
	ParseTree(ParseTree* parent, const std::vector<ParseTree*> children, EarleyItem earleyItem, int start, int end, int length, std::string data);

	void printParseTree(ParseTree* tree);
	void printSubTree(ParseTree* tree, const std::string& prefix);

	ParseTree* m_parent;
	std::vector<ParseTree*> m_children;
	std::string m_variable;
	std::vector<std::string> m_symbols;
	int m_start = 0;
	int m_end = 0;
	int m_length = 0;
	std::string m_data;
};

