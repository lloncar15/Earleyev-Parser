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

Rules Grammar::getRulesWithVariableInRHS(std::string symbol)
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
