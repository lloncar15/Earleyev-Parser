#include <iostream>
#include "EarleyItem.h"
#include "EarleyParser.h"
#include "Grammar.h"

using namespace std;

int main()
{
	Grammar grammar = Grammar();
	grammar.addVariable("Sum");
	grammar.addVariable("Product");
	grammar.addVariable("Factor");
	grammar.addVariable("Number");
	grammar.addSymbol("0");
	grammar.addSymbol("1");
	grammar.addSymbol("2");
	grammar.addSymbol("3");
	grammar.addSymbol("4");
	grammar.addSymbol("5");
	grammar.addSymbol("6");
	grammar.addSymbol("7");
	grammar.addSymbol("8");
	grammar.addSymbol("9");
	grammar.addSymbol("(");
	grammar.addSymbol(")");
	grammar.addSymbol("+");
	grammar.addSymbol("-");
	grammar.addSymbol("*");
	grammar.addSymbol("/");

	grammar.addRule("Sum", { "Sum", "+", "Product" });
	grammar.addRule("Sum", { "Sum", "-", "Product" });
	grammar.addRule("Sum", { "Product" });
	grammar.addRule("Product", { "Product", "*", "Factor" });
	grammar.addRule("Product", { "Product", "/", "Factor" });
	grammar.addRule("Product", { "Factor" });
	grammar.addRule("Factor", { "(", "Sum", ")" });
	grammar.addRule("Factor", { "Number" });
	grammar.addRule("Number", { "0" });
	grammar.addRule("Number", { "1" });
	grammar.addRule("Number", { "2" });
	grammar.addRule("Number", { "3" });
	grammar.addRule("Number", { "4" });
	grammar.addRule("Number", { "5" });
	grammar.addRule("Number", { "6" });
	grammar.addRule("Number", { "7" });
	grammar.addRule("Number", { "8" });
	grammar.addRule("Number", { "9" });

	grammar.setStartVariable("Sum");

	EarleyParser parser = EarleyParser(grammar);
	std::string input = "1+(2*3-4)";
	parser.parse(input);

	Grammar grammar2 = Grammar();
	grammar2.addVariable("A");
	grammar2.addVariable("B");
	grammar2.addSymbol("");

	grammar2.addRule("A", { "" });
	grammar2.addRule("A", { "B" });
	grammar2.addRule("B", { "A" });

	grammar2.setStartVariable("A");

	EarleyParser parser2 = EarleyParser(grammar2);
	std::string input2 = "";
	parser2.parse(input2);

	Grammar grammar3 = Grammar();
	grammar3.addVariable("S");
	grammar3.addVariable("E");
	grammar3.addSymbol("a");
	grammar3.addSymbol("+");

	grammar3.addRule("S", { "E" });
	grammar3.addRule("E", { "E", "+", "E" });
	grammar3.addRule("E", { "a" });

	grammar3.setStartVariable("S");

	EarleyParser parser3 = EarleyParser(grammar3);
	std::string input3 = "a+a+a";
	parser3.parse(input3);
}