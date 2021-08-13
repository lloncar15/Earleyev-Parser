#include "Grammar.h"

Grammar::Grammar() {

}

Grammar::~Grammar() {

}

void Grammar::addRule(std::string leftHandVariable, std::vector<std::string> rightHandSymbols)
{
	m_rules.emplace(leftHandVariable, rightHandSymbols);
}

void Grammar::addVariable(std::string variable)
{
	m_variables.emplace(variable);
}

void Grammar::addSymbol(std::string symbol)
{
	m_symbols.emplace(symbol);
}

bool Grammar::isStringVariable(std::string& string)
{
	return m_variables.find(string) != m_variables.end();
}

bool Grammar::isStringSymbol(std::string& string)
{
	return  m_symbols.find(string) != m_symbols.end();
}
