#include "Grammar.h"

using namespace std;

Grammar::Grammar() {

}

Grammar::~Grammar() {

}

void Grammar::addRule(string leftHandVariable, vector<string> rightHandSymbols)
{
	m_rules.emplace(leftHandVariable, rightHandSymbols);
}

void Grammar::addVariable(string variable)
{
	m_variables.emplace(variable);
}

void Grammar::addSymbol(string symbol)
{
	m_symbols.emplace(symbol);
}

bool Grammar::isStringVariable(string& string)
{
	return m_variables.find(string) != m_variables.end();
}

bool Grammar::isStringSymbol(string& string)
{
	return  m_symbols.find(string) != m_symbols.end();
}

Rules Grammar::getRulesWithVariableInRHS(string symbol)
{
	Rules rules;

	// add each rule which contains symbol to rules
	for (auto& rulePair : m_rules) {
		for (auto& symbolRHS : rulePair.second) {
			if (symbol == symbolRHS) {
				rules.emplace(rulePair);
				continue;
			}
		}
	}
	return rules;
}

Rules Grammar::getEmptyRules()
{
	Rules rules;

	// add each empty rule to rules
	for (auto& rulePair : m_rules) {
		if (rulePair.second.size() == 1 && rulePair.second[0] == "") {
			rules.emplace(rulePair);
		}
	}
	return rules;
}
