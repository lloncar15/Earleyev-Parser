﻿#include "EarleyParser.h"

#include <iostream>
#include <queue>

using namespace std;

EarleyParser::EarleyParser(Grammar grammar) : m_grammar(grammar)
{
	m_state = { {} };
}

EarleyParser::~EarleyParser()
{
}

void EarleyParser::parse(string& input)
{
	m_input = input;
	findNullableVariables();
	buildItems();
	printState(m_state);
	finishRecogniser();
	removeUncompletedItems();
	printState(m_state);

	vector<vector<EarleyItem>> stateOrderedByStart = orderStateByStart(m_state);
	printState(stateOrderedByStart);
	ParseTree* tree = createTree(stateOrderedByStart, m_input, 0, m_input.size(), m_grammar.getStartVariable(), nullptr);
	printTree(tree, "", true);

	/*m_parseTrees = createForest(m_state, m_input);
	for (auto it = m_parseTrees.begin(); it != m_parseTrees.end(); ) {
		if ((*it)->m_end != m_input.size()) {
			it = m_parseTrees.erase(it);
		}
		else {
			++it;
		}
	}*/
	printParseTrees();
}

// __________ RECOGNISER __________

void EarleyParser::buildItems()
{
	string startVariable = m_grammar.getStartVariable();
	for (auto& rule : m_grammar.getRules()) {
		if (rule.first == startVariable) {
			addEarleyItemIfDoesntExist(EarleyItem(startVariable, rule.second, 0, 0), 0);
		}
	}

	for (int i = 0; i < m_state.size(); ++i) {
		auto& tempStateSet = m_state[i];
		for (int j = 0; j < tempStateSet.size(); ++j) {
			auto& tempEarleyItem = tempStateSet[j];
			int dotIndex = tempEarleyItem.getParsedSymbols();
			auto& tempSymbols = tempEarleyItem.getSymbols();

			if (tempEarleyItem.isCompleted()) {
				complete(tempStateSet, i, j);
			}
			else {
				string nextSymbol = tempSymbols[dotIndex];
				if (m_grammar.isStringVariable(nextSymbol)) {
					predict(tempStateSet, i, j, nextSymbol);
				}
				else if (m_grammar.isStringSymbol(nextSymbol)) {
					scan(tempStateSet, i, j, nextSymbol, m_input);
				}
				else {
					exit(1);
				}
			}
		}
	}
}

void EarleyParser::predict(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, string& symbol)
{
	for (auto& rule : m_grammar.getRules()) {
		if (symbol == rule.first) {
			addEarleyItemIfDoesntExist(EarleyItem(symbol, rule.second, stateSetIndex, 0), stateSetIndex);
			if (isVariableNullable(symbol)) {
				addEarleyItemIfDoesntExist(EarleyItem(symbol, rule.second, stateSetIndex, stateSet[desiredStateSetIndex].getParsedSymbols() + 1), stateSetIndex);
			}
		}
	}
}

void EarleyParser::scan(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex, string& symbol, string& input)
{
	if (stateSetIndex >= input.length()) {
		return;
	}
	else {
		string inputSymbol;
		inputSymbol = input.at(stateSetIndex);
		if (symbol != inputSymbol) {
			return;
		}

		EarleyItem tempItem = stateSet[desiredStateSetIndex];
		EarleyItem item = EarleyItem(tempItem.getVariable(), tempItem.getSymbols(), tempItem.getStart(), tempItem.getParsedSymbols() + 1);
		addEarleyItemIfDoesntExist(item, stateSetIndex + 1);
	}
}

void EarleyParser::complete(vector<EarleyItem>& stateSet, int stateSetIndex, int desiredStateSetIndex)
{
	EarleyItem tempItem = stateSet[desiredStateSetIndex];
	auto& tempStateSet = m_state[tempItem.getStart()];

	string desiredSymbol = tempItem.getVariable();
	for (auto& item : tempStateSet) {
		auto& symbols = item.getSymbols();
		int itemParsedSymbols = item.getParsedSymbols();
		if (symbols.size() <= itemParsedSymbols) {
			continue;
		}
		string checkingSymbol = symbols[itemParsedSymbols];

		if (desiredSymbol == checkingSymbol) {
			EarleyItem newEarleyItem = EarleyItem(item.getVariable(), item.getSymbols(), item.getStart(), item.getParsedSymbols() + 1);
			addEarleyItemIfDoesntExist(newEarleyItem, stateSetIndex);
		}
	}
}

void EarleyParser::addEarleyItemIfDoesntExist(EarleyItem item, int stateSetIndex)
{
	if (m_state.size() <= stateSetIndex) {
		vector<EarleyItem> vector = { item };
		m_state.emplace_back(vector);
		return;
	}

	auto& set = m_state[stateSetIndex];
	for (EarleyItem& setItem : set) {
		if (item == setItem) {
			return;
		}
	}
	set.emplace_back(item);
}

void EarleyParser::printState(const vector<vector<EarleyItem>>& state)
{
	cout << "Printing the whole state: " << endl;
	for (unsigned int i = 0; i < state.size(); ++i) {
		printStateSet(state[i], i);
	}
	cout << endl;
}

void EarleyParser::printStateSet(const vector<EarleyItem>& set, int i)
{
	cout << string("=== ") + to_string(i) + string(" ===") << endl;
	for (auto& earleyItem : set) {
		earleyItem.printItem();
	}
}

void EarleyParser::finishRecogniser()
{
	int inputSize = m_input.size();
	if (m_state.size() - 1 != inputSize) {
		exit(1);
	}
	else {
		string startVar = m_grammar.getStartVariable();
		bool isFinished = false;
		for (auto& item : m_state[inputSize]) {
			bool tempBool = true;
			tempBool &= item.getParsedSymbols() == item.getSymbols().size();
			tempBool &= item.getStart() == 0;
			tempBool &= item.getVariable() == startVar;
			if (tempBool) {
				isFinished = true;
				break;
			}
		}
		if (!isFinished) {
			exit(1);
		}
	}
}

// __________ EMPTY RULES __________

void EarleyParser::findNullableVariables()
{
	map<string, Rules> variableRulesRHS;
	for (auto& variable : m_grammar.getVariables()) {
		Rules rules = m_grammar.getRulesWithVariableInRHS(variable);
		variableRulesRHS.emplace(variable, rules);
	}

	for (auto& rule : m_grammar.getEmptyRules()) {
		m_nullableVariables.emplace(rule.first);
	}

	queue<string> workQueue;
	for (auto variable : m_nullableVariables) {
		workQueue.push(variable);
	}

	while (workQueue.size() > 0) {
		string workVariable = workQueue.front();
		workQueue.pop();

		for (auto& rule : variableRulesRHS[workVariable]) {
			if (isVariableNullable(rule.first)) {
				continue;
			}

			bool isRuleNullable = true;
			for (auto& symbol : rule.second) {
				if (!isVariableNullable(symbol)) {
					isRuleNullable = false;
					break;
				}
			}
			if (isRuleNullable) {
				m_nullableVariables.emplace(rule.first);
				workQueue.push(rule.first);
			}
		}
	}
}

bool EarleyParser::isVariableNullable(const string& symbol)
{
	return m_nullableVariables.find(symbol) != m_nullableVariables.end();
}

// __________ PARSER __________

void EarleyParser::removeUncompletedItems()
{
	for (auto& set : m_state) {
		auto it = set.begin();
		while (it != set.end()) {
			if (!(*it).isCompleted()) {
				it = set.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

vector<vector<EarleyItem>> EarleyParser::orderStateByStart(const vector<vector<EarleyItem>>& state)
{
	vector<vector<EarleyItem>> newState(state.size());
	for (int i = 0; i < state.size(); ++i) {
		for (auto item : state[i]) {
			EarleyItem newItem = EarleyItem(item.getVariable(), item.getSymbols(), i, item.getParsedSymbols());
			newState[item.getStart()].emplace_back(newItem);
		}
	}
	return newState;
}

vector<ParseTree*> EarleyParser::createForest(const vector<vector<EarleyItem>>& state, const string& input)
{
	vector<vector<EarleyItem>> stateOrderedByStart = orderStateByStart(m_state);
	printState(stateOrderedByStart);

	vector<ParseTree*> memo;
	return createTrees(stateOrderedByStart, input, 0, m_grammar.getStartVariable(), 0, memo);
}

vector<ParseTree*> EarleyParser::createTrees(const vector<vector<EarleyItem>>& state, const string& input, int start, string token, ParseTree* parent, vector<ParseTree*>& memo)
{
	vector<ParseTree*> returnTrees;
	for (int i = 0; i < state[start].size(); i++) {
		vector<ParseTree*> possibleTrees;
		EarleyItem item = state[start][i];
		if (item.getVariable() != token) {
			continue;
		}

		int itemEnd = item.getStart();
		if (parent != 0 && itemEnd > parent->m_end) {
			continue;
		}

		bool found = false;
		for (ParseTree* tree = parent; tree != 0; tree = tree->m_parent) {
			bool isSame = true;
			isSame &= tree->m_variable == item.getVariable();
			isSame &= tree->m_symbols == item.getSymbols();
			isSame &= tree->m_start == start;
			isSame &= tree->m_end == itemEnd;
			if (isSame) {
				found = true;
				break;
			}
		}
		if (found) {
			continue;
		}

		found = false;
		for (int j = 0; j < memo.size(); ++j) {
			ParseTree* tree = memo[j];

			bool isSame = true;
			isSame &= tree->m_variable == item.getVariable();
			isSame &= tree->m_symbols == item.getSymbols();
			isSame &= tree->m_start == start;
			isSame &= tree->m_end == itemEnd;
			if (isSame) {
				returnTrees.push_back(tree);
				found = true;
			}
		}
		if (found) {
			continue;
		}

		ParseTree* tree = new ParseTree(parent, vector<ParseTree*>(), item, start, itemEnd, 0, token);
		possibleTrees.push_back(tree);

		for (int j = 0; j < possibleTrees.size(); ++j) {
			tree = possibleTrees[j];

			int numOfTokens = tree->m_symbols.size();
			while (tree->m_children.size() < numOfTokens) {
				int index = tree->m_children.size();

				string inputChar;
				if (!input.empty()) {
					inputChar += input[start + tree->m_length];
				}
				if (tree->m_symbols[index] == inputChar) {
					int charLength = inputChar.empty() ? 0 : 1;
					ParseTree* leafTree = new ParseTree(tree, vector<ParseTree*>(), EarleyItem(), start + tree->m_length, start + tree->m_length + charLength, charLength, inputChar);
					tree->m_children.push_back(leafTree);
					tree->m_length += charLength;
					continue;
				}

				vector<ParseTree*> children = createTrees(state, input, start + tree->m_length, tree->m_symbols[index], tree, memo);

				if (children.size() == 0) {
					break;
				}
				else {
					ParseTree* child = children[0];
					int length = tree->m_length;
					tree->m_length += child->m_length;
					tree->m_children.push_back(child);

					for (int k = 1; k < children.size(); k++) {
						child = children[k];
						ParseTree* newTree = new ParseTree(tree->m_parent, tree->m_children, item, tree->m_start, tree->m_end, length + child->m_length, tree->m_data);
						newTree->m_children.back() = child;
						possibleTrees.push_back(newTree);
					}
				}
			}

			if (tree->m_children.size() == numOfTokens && tree->m_start + tree->m_length == tree->m_end) {
				returnTrees.push_back(tree);
				memo.push_back(tree);
			}
		}
	}
	return returnTrees;
}

void EarleyParser::printParseTrees()
{
	int parseTreeNumber = 1;
	for (auto& tree : m_parseTrees) {
		cout << "Printing parse tree number " + to_string(parseTreeNumber) + string(":") << endl;
		printTree(tree, "", true);
		++parseTreeNumber;
	}
}

void EarleyParser::printTree(ParseTree* tree, string indent, bool isLast)
{
	if (tree == nullptr) {
		return;
	}

	cout << indent + string("#- ") + tree->m_data << endl;
	indent += isLast ? "   " : "|  ";

	for (int i = 0; i < tree->m_children.size(); ++i) {
		printTree(tree->m_children[i], indent, i == tree->m_children.size() - 1);
	}
}

ParseTree* EarleyParser::createTree(const std::vector<std::vector<EarleyItem>>& state, const std::string& input, int start, int end, const std::string& token, ParseTree* parent)
{
	for (int i = 0; i < state[start].size(); ++i) {
		EarleyItem item = state[start][i];
		if (item.getVariable() != token) {
			continue;
		}

		int itemEnd = item.getStart();
		if (parent != 0 && itemEnd > parent->m_end) {
			continue;
		}

		bool found = false;
		for (ParseTree* tree = parent; tree != 0; tree = tree->m_parent) {
			bool isSame = true;
			isSame &= tree->m_variable == item.getVariable();
			isSame &= tree->m_symbols == item.getSymbols();
			isSame &= tree->m_start == start;
			isSame &= tree->m_end == itemEnd;
			if (isSame) {
				found = true;
				break;
			}
		}
		if (found) {
			continue;
		}

		ParseTree* tree = new ParseTree(parent, vector<ParseTree*>(), item, start, itemEnd, 0, token);

		int numOfTokens = tree->m_symbols.size();
		while (tree->m_children.size() < numOfTokens) {
			int index = tree->m_children.size();

			string inputChar;
			if (!input.empty()) {
				inputChar += input[start + tree->m_length];
			}

			if (tree->m_symbols[index] == inputChar) {
				int charLength = inputChar.empty() ? 0 : 1;
				ParseTree* leafTree = new ParseTree(tree, vector<ParseTree*>(), EarleyItem(), start + tree->m_length, start + tree->m_length + charLength, charLength, inputChar);
				tree->m_children.push_back(leafTree);
				tree->m_length += charLength;
				continue;
			}

			ParseTree* child = createTree(state, input, start + tree->m_length, itemEnd, tree->m_symbols[index], tree);
			if (child == nullptr) {
				break;
			}
			else {
				tree->m_length += child->m_length;
				tree->m_children.push_back(child);
			}
		}

		if (tree->m_children.size() == numOfTokens && tree->m_start + tree->m_length == tree->m_end) {
			return tree;
		}
	}
	return nullptr;
}
