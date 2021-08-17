#include "EarleyParser.h"
#include <iostream>

EarleyParser::EarleyParser()
{
	m_state = { {} };
}

EarleyParser::~EarleyParser()
{
}

void EarleyParser::parse(Grammar& grammar, std::string& input)
{
	m_input = input;
	buildItems(grammar);
	printState();
	createParseTree();
}

void EarleyParser::buildItems(Grammar& grammar)
{
	// populate the first state set with rules that have the start variable on the left hand side
	std::string startVariable = grammar.getStartVariable();
	for (auto& rule : grammar.getRules()) {
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
				complete(tempStateSet, i, j, grammar);
			}
			else {
				std::string nextSymbol = tempSymbols[dotIndex];
				if (grammar.isStringVariable(nextSymbol)) {
					predict(tempStateSet, i, nextSymbol, grammar);
				}
				else if (grammar.isStringSymbol(nextSymbol)) {
					scan(tempStateSet, i, j, nextSymbol, m_input, grammar);
				}
				else {
					// handle error
				}
			}
		}
	}
}

void EarleyParser::predict(std::vector<EarleyItem>& stateSet, int stateSetIndex, std::string& symbol, Grammar& grammar)
{
	for (auto& rule : grammar.getRules()) {
		if (symbol == rule.first) {
			addEarleyItemIfDoesntExist(EarleyItem(symbol, rule.second, stateSetIndex, 0), stateSetIndex);
		}
	}
}

void EarleyParser::scan(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, std::string& symbol, std::string& input, Grammar& grammar)
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

void EarleyParser::complete(std::vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, Grammar& grammar)
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
