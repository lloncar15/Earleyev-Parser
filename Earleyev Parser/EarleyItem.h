#pragma once

#include <string>
#include <vector>

class EarleyItem {
public:
	EarleyItem(std::string m_variable, std::vector<std::string> symbols, int start, int parsedSymbols);
	EarleyItem() {};

	bool operator==(EarleyItem& b);
	void printItem() const;
	bool isCompleted();

	std::string& getVariable() { return m_variable; }
	std::vector<std::string>& getSymbols() { return m_symbols; }
	int getStart() { return m_start; }
	void setStart(int start) { m_start = start; }
	int getParsedSymbols() { return m_parsedSymbols; }
	void setParsedSymbols(int parsed) { m_parsedSymbols = parsed; }
private:
	// m_start represents where the EarleyItem begins, m_start = 0 means that it starts at the begining of the input string
	// m_parsedSymbols represents where the dot is, how many symbols are parsed until now
	int m_start = 0;
	int m_parsedSymbols = 0;
	std::string m_variable;
	std::vector<std::string> m_symbols;
};