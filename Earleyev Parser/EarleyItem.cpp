#include "EarleyItem.h"

#include <string>
#include <iostream>

using namespace std;

EarleyItem::EarleyItem(string variable, vector<string> symbols, int start, int parsedSymbols) : m_variable(variable), m_symbols(symbols), m_start(start), m_parsedSymbols(parsedSymbols)
{
}

bool EarleyItem::operator==(EarleyItem& b)
{
	bool equals = true;

	equals &= m_variable == b.getVariable();
	equals &= m_start == b.getStart();
	equals &= m_parsedSymbols == b.getParsedSymbols();
	equals &= m_symbols == b.getSymbols();

	return equals;
}

void EarleyItem::printItem() const
{
	string outputString = m_variable + " -> ";
	for (unsigned int i = 0; i < m_symbols.size(); ++i) {
		if (m_parsedSymbols == i) {
			outputString += " . ";
		}
		outputString += m_symbols[i] + " ";
	}
	if (m_parsedSymbols == m_symbols.size()) {
		outputString += " . ";
	}
	outputString += string(" (") + to_string(m_start) + ")";

	cout << outputString << endl;
}

bool EarleyItem::isCompleted()
{
	return m_symbols.size() == m_parsedSymbols;
}
