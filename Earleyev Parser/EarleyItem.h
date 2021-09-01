#pragma once

#include <string>
#include <vector>

using namespace std;

class EarleyItem {
public:
	EarleyItem(string m_variable, vector<string> symbols, int start, int parsedSymbols);
	EarleyItem() {};

	bool operator==(EarleyItem& b);
	void printItem() const;
	bool isCompleted();

	string& getVariable() { return m_variable; }
	vector<string>& getSymbols() { return m_symbols; }
	int getStart() { return m_start; }
	void setStart(int start) { m_start = start; }
	int getParsedSymbols() { return m_parsedSymbols; }
	void setParsedSymbols(int parsed) { m_parsedSymbols = parsed; }
private:
	int m_start = 0;
	int m_parsedSymbols = 0;
	string m_variable;
	vector<string> m_symbols;
};