#include "ParseTree.h"

#include <iostream>

using namespace std;

ParseTree::ParseTree(ParseTree* parent, const vector<ParseTree*> children, EarleyItem earleyItem, int start, int end, int length, string data) : m_parent(parent), m_children(children), m_start(start), m_end(end), m_length(length), m_data(data)
{
	// not saving EarleyItem as variable to access variable and right hand side symbols easier
	m_variable = earleyItem.getVariable();
	m_symbols = earleyItem.getSymbols();
}

void ParseTree::printParseTree(ParseTree* tree)
{
	if (tree == nullptr) {
		return;
	}
	cout << endl;
	printSubTree(tree, "");
	cout << endl;
}

void ParseTree::printSubTree(ParseTree* tree, const string& prefix)
{
	if (tree == nullptr) {
		return;
	}
	bool hasChild = !tree->m_children.empty();
	int childrenCount = tree->m_children.size();
	bool hasChildren = childrenCount > 1;

	if (!hasChild) {
		return;
	}

	bool counter = 0;
	for (int i = 0; i < childrenCount; ++i) {
		cout << prefix;
		string newPrefix = prefix;

		if (counter == childrenCount - 1) {
			cout << "└── ";
		}
		else {
			cout << "├── ";
		}

		printSubTree(tree->m_children[i], newPrefix);
	}
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