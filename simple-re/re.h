#pragma once
#include "nfa.h"
#include "dfa.h"
#include <iostream>
#include <stack>

// 运算符类型
#define LEFTUNARY 0
#define RIGHTUNARY -1
#define BINARY -2


// 特殊字符类型
#define DOT 26


namespace re {

	struct Operator
	{
		char value;
		int type;
		int priority;

		Operator(char value, int type, int priority) {
			this->value = value;
			this->type = type;
			this->priority = priority;
		}

		Operator() {

		}
	};


	std::string infix2postfix(std::string originExp);

	NFA postfix2nfa(std::string postfixExp);

	DFA nfa2dfa(NFA& nfa);

	bool is_match(DFA& dfa, std::string postfixExp);

	bool match(std::string str, std::string pattern);
}
