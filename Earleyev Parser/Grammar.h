#pragma once

#include <map>
#include <unordered_set>

using namespace std;

typedef multimap<string, vector<string>> Rules;

class Grammar
{
public:
	Grammar();
	~Grammar();
	void addRule(string leftHandVariable, vector<string> rightHandSymbols);
	void addVariable(string variable);
	void addSymbol(string symbol);

	bool isStringVariable(string& string);
	bool isStringSymbol(string& string);

	Rules getRulesWithVariableInRHS(string symbol);
	Rules getEmptyRules();

	const string& getStartVariable() { return m_startVariable; }
	void setStartVariable(string variable) { m_startVariable = variable; }
	const Rules& getRules() { return m_rules; }
	const unordered_set<string>& getSymbols() { return m_symbols; }
	const unordered_set<string>& getVariables() { return m_variables; }
private:
	unordered_set<string> m_variables;
	unordered_set<string> m_symbols;
	string m_startVariable;
	Rules m_rules;
};

