#pragma once

#include <map>
#include <unordered_set>

typedef std::multimap<std::string, std::vector<std::string>> Rules;

class Grammar
{
public:
	Grammar();
	~Grammar();
	void addRule(std::string leftHandVariable, std::vector<std::string> rightHandSymbols);
	void addVariable(std::string variable);
	void addSymbol(std::string symbol);

	const std::string& getStartVariable() { return m_startVariable; }
	void setStartVariable(std::string variable) { m_startVariable = variable; }
	const Rules& getRules() { return m_rules; }
	const std::unordered_set<std::string>& getSymbols() { return m_symbols; }
	const std::unordered_set<std::string>& getVariables() { return m_variables; }

	Rules getRulesWithVariableInRHS(std::string symbol);
	Rules getEmptyRules();

	bool isStringVariable(std::string& string);
	bool isStringSymbol(std::string& string);
private:
	std::unordered_set<std::string> m_variables;
	std::unordered_set<std::string> m_symbols;
	std::string m_startVariable;
	Rules m_rules;
};

