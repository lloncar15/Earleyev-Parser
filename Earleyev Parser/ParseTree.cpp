#include "ParseTree.h"

#include <iostream>

using namespace std;

ParseTree::ParseTree(ParseTree* parent, const vector<ParseTree*> children, EarleyItem earleyItem, int start, int end, int length, string data) : m_parent(parent), m_children(children), m_start(start), m_end(end), m_length(length), m_data(data)
{
	// not saving EarleyItem as variable to access variable and right hand side symbols easier
	m_variable = earleyItem.getVariable();
	m_symbols = earleyItem.getSymbols();
}

bool ParseTree::isMatchingItem(EarleyItem item, int start)
{
	bool isSame = true;
	isSame &= m_variable == item.getVariable();
	isSame &= m_symbols == item.getSymbols();
	isSame &= m_start == start;
	isSame &= m_end == item.getStart();
	return isSame;
}

/* Primjer ispisa stabla parsiranja za 1+(2*3-4)
+- Sum
   +- Sum
   |  +- Product
   |     +- Factor
   |        +- Number
   |           +- 1
   +- +
   +- Product
      +- Factor
	     +- (
		 +- Sum
		 |  +- Sum
		 |  |  +- Product
		 |  |     +- Product
		 |  |     |  +- Factor
		 |  |     |     +- Number
		 |  |     |        +- 2
		 |  |     +- *
		 |  |     +- Factor
		 |  |        +- Number
		 |  |           +- 3
		 |  +- -
		 |  +- Product
		 |     +- Factor
		 |        +- Number
		 |           +- 4
		 +- )
*/