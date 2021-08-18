#include "EarleyParser.h"
#include <iostream>
#include <queue>

EarleyParser::EarleyParser(Grammar grammar) : m_grammar(grammar)
{
	m_state = { {} };
}

EarleyParser::~EarleyParser()
{
}

void EarleyParser::parse(std::string& input)
{
	m_input = input;
	findNullableVariables();
	buildItems();
	printState();
	createParseTree();
}

void EarleyParser::buildItems()
{
	// populate the first state set with rules that have the start variable on the left hand side
	std::string startVariable = m_grammar.getStartVariable();
	for (auto& rule : m_grammar.getRules()) {
		if (rule.first == startVariable) {
			addEarleyItemIfDoesntExist(EarleyItem(startVariable, rule.second, 0, 0), 0);
		}
	}

	// populate other state sets
	
	for (int i = 0; i < m_state.size(); ++i) {
		auto& tempStateSet = m_state[i];
		for (int j = 0; j < m_state[i].size(); ++j) {
			auto& tempEarleyItem = m_state[i][j];
			int dotIndex = tempEarleyItem.getParsedSymbols();
			int startIndex = tempEarleyItem.getStart();
			auto& tempSymbols = tempEarleyItem.getSymbols();

			auto it = tempSymbols.begin() + dotIndex;

			// the dot is at the end of the rule
			if (it == tempSymbols.end()) {
				complete(tempStateSet, i, j);
			}
			else {
				std::string nextSymbol = tempSymbols[dotIndex];
				if (m_grammar.isStringVariable(nextSymbol)) {
					predict(tempStateSet, i, j, nextSymbol);
				}
				else if (m_grammar.isStringSymbol(nextSymbol)) {
					scan(tempStateSet, i, j, nextSymbol, m_input);
				}
				else {
					// handle error
				}
			}
		}
	}
}

void EarleyParser::predict(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol)
{
	for (auto& rule : m_grammar.getRules()) {
		if (symbol == rule.first) {
			addEarleyItemIfDoesntExist(EarleyItem(symbol, rule.second, stateSetIndex, 0), stateSetIndex);
			// add the magic completion for nullable variables
			if (isVariableNullable(symbol)) {
				addEarleyItemIfDoesntExist(EarleyItem(symbol, rule.second, stateSetIndex, stateSet[desiredStateSetIndex].getParsedSymbols() + 1), stateSetIndex);
			}
		}
	}
}

void EarleyParser::scan(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol, std::string& input)
{
	if (stateSetIndex >= input.length()) {
		return;
	}
	else {
		std::string inputSymbol;
		inputSymbol = input.at(stateSetIndex);

		// don't do anything if the symbol doesn't match
		if (symbol != inputSymbol) {
			return;
		}

		// stateSet exists and just add the item there
		auto& tempItem = stateSet[desiredStateSetIndex];
		EarleyItem item = EarleyItem(tempItem.getVariable(), tempItem.getSymbols(), tempItem.getStart(), tempItem.getParsedSymbols() + 1);
		addEarleyItemIfDoesntExist(item, stateSetIndex + 1);
	}
}

void EarleyParser::complete(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex)
{
	EarleyItem tempItem = stateSet[desiredStateSetIndex];
	auto& tempStateSet = m_state[tempItem.getStart()];

	// get the symbol to the left of the dot
	std::string desiredSymbol = tempItem.getVariable();

	// check EarleyItems in the desired stateSet
	for (auto& item : tempStateSet) {
		auto& symbols = item.getSymbols();
		int itemParsedSymbols = item.getParsedSymbols();
		if (symbols.size() <= itemParsedSymbols) {
			continue;
		}
		std::string checkingSymbol = symbols[itemParsedSymbols];

		if (desiredSymbol == checkingSymbol) {
			EarleyItem newEarleyItem = EarleyItem(item.getVariable(), item.getSymbols(), item.getStart(), item.getParsedSymbols() + 1);
			addEarleyItemIfDoesntExist(newEarleyItem, stateSetIndex);
		}
	}
}

void EarleyParser::createParseTree()
{

}

void EarleyParser::findNullableVariables()
{
	// create a map where the keys are variables and values are rules that contain that variable
	std::map<std::string, Rules> variableRulesRHS;
	for (auto& variable : m_grammar.getVariables()) {
		Rules rules = m_grammar.getRulesWithVariableInRHS(variable);
		variableRulesRHS.emplace(variable, rules);
	}

	// add all variables of empty rules to nullable variables
	for (auto& rule : m_grammar.getEmptyRules()) {
		m_nullableVariables.emplace(rule.first);
	}

	// init the work queue with nullable variables set
	std::queue<std::string> workQueue;
	for (auto variable : m_nullableVariables) {
		workQueue.push(variable);
	}

	while (workQueue.size() > 0) {
		std::string workVariable = workQueue.front();
		workQueue.pop();

		// iterate through the rules contain the workVariable in RHS
		for (auto& rule : variableRulesRHS[workVariable]) {
			// the LHS variable is already nullable, go to next rule
			if (isVariableNullable(rule.first)) {
				continue;
			}

			bool isRuleNullable = true;
			for (auto& symbol : rule.second) {
				// if one of the symbols is not nullable skip this rule
				if (!isVariableNullable(symbol)) {
					isRuleNullable = false;
					break;
				}
			}
			// add the LHS to nullable variables and add it to the workQueue
			if (isRuleNullable) {
				m_nullableVariables.emplace(rule.first);
				workQueue.push(rule.first);
			}
		}
	}
}

void EarleyParser::addEarleyItemIfDoesntExist(EarleyItem item, int stateSetIndex)
{
	// add the new state set if it doesn't exist
	if (m_state.size() <= stateSetIndex) {
		std::vector<EarleyItem> vector = { item };
		m_state.emplace_back(vector);
		return;
	}

	auto& set = m_state[stateSetIndex];
	// don't add the item if it already exists in the state set
	for (EarleyItem& setItem : set) {
		if (item == setItem) {
			return;
		}
	}
	set.emplace_back(item);
}

void EarleyParser::printState()
{
	std::cout << "Printing the whole state: " << std::endl;
	for (unsigned int i = 0; i < m_state.size(); ++i) {
		printStateSet(i);
	}
}

void EarleyParser::printStateSet(int i)
{
	std::cout << std::string("=== ") + std::to_string(i) + std::string(" ===") << std::endl;
	for (auto& earleyItem : m_state[i]) {
		earleyItem.printItem();
	}
}

bool EarleyParser::isVariableNullable(const std::string& symbol)
{
	return m_nullableVariables.find(symbol) != m_nullableVariables.end();
}
