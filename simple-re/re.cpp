#include "re.h"
using namespace std;

/*
* 定义运算符和优先级
*/
std::unordered_map<char, re::Operator> priorityMap = {
	{'*', re::Operator('*', RIGHTUNARY, 0)},
	{'+', re::Operator('+', RIGHTUNARY, 0)},
	{'?', re::Operator('?', RIGHTUNARY, 0)},
	{'&', re::Operator('&', BINARY, 1)},
	{'|', re::Operator('|', BINARY, 2)},
	{'(', re::Operator('(', LEFTUNARY, 3)},
	{')', re::Operator(')', RIGHTUNARY, 4)}
};


bool is_equal_or_unprior(char ch1, char ch2) {
	//优先级：'*' > '&' > '|' > '('
	if (priorityMap.find(ch1) != priorityMap.end()
		&& priorityMap.find(ch2) != priorityMap.end()
		&& priorityMap[ch1].priority >= priorityMap[ch2].priority) {
		return true;
	}

	return false;
}


bool is_operator(char ch) {
	if (priorityMap.find(ch) != priorityMap.end()) {
		return true;
	}
	else {
		return false;
	}
}

/*
*  填充连接符&
*/
string fill_connector(string originExp) {
	string filledExp;

	for (auto it = originExp.begin(); it != originExp.end(); it++) {
		filledExp.push_back(*it);
		if (it < originExp.end() - 1) {
			if ((!is_operator(*it) || priorityMap[*it].type == RIGHTUNARY)
				&& (!is_operator(*(it + 1)) || priorityMap[*(it + 1)].type == LEFTUNARY)) {
				filledExp.push_back('&');
			}
		}
	}

	return filledExp;
}


/*
* 中缀 -> 后缀
*/
string re::infix2postfix(string pattern) {
	string infixExp = fill_connector(pattern);
	string postfixExp;
	stack<char> operatorStack;
	for (char ch : infixExp) {
		if (!is_operator(ch)) {
			if (ch == '.') {
				postfixExp.push_back(DOT);
			}
			else {
				postfixExp.push_back(ch);
			}
			continue;
		}
		if (ch == '(') {
			operatorStack.push(ch);
			continue;
		}
		if (ch == ')') {
			while (!operatorStack.empty()) {
				char temp = operatorStack.top();
				operatorStack.pop();
				if (temp != '(') {
					postfixExp.push_back(temp);
				}
				else {
					break;
				}
			}
			continue;
		}
		while (!operatorStack.empty() && is_equal_or_unprior(ch, operatorStack.top())) {
			postfixExp.push_back(operatorStack.top());
			operatorStack.pop();
		}
		operatorStack.push(ch);
	}

	while (!operatorStack.empty()) {
		postfixExp.push_back(operatorStack.top());
		operatorStack.pop();
	}

	return postfixExp;
}


/*
* 后缀 -> NFA
*/
NFA re::postfix2nfa(string postfixExp) {
	struct SubNFA {
		int headId;
		int tailId;
		SubNFA(int headId, int tailId) {
			this->headId = headId;
			this->tailId = tailId;
		}
	};

	NFA nfa;
	stack<SubNFA> subNFAStack;
	int newHeadId = 0, newTailId = 0;
	for (char ch : postfixExp) {
		if (!is_operator(ch)) {
			newHeadId = nfa.get_new_state_id();
			newTailId = nfa.get_new_state_id();

			subNFAStack.push(SubNFA(newHeadId, newTailId));
			nfa.insert_edge(newHeadId, newTailId, ch);
			continue;
		}

		if (ch == '*') {
			SubNFA subNFA = subNFAStack.top();
			subNFAStack.pop();

			newHeadId = nfa.get_new_state_id();
			newTailId = nfa.get_new_state_id();
			nfa.insert_edge(newHeadId, subNFA.headId, EPSILON);
			nfa.insert_edge(subNFA.tailId, newTailId, EPSILON);
			nfa.insert_edge(subNFA.tailId, subNFA.headId, EPSILON);
			nfa.insert_edge(newHeadId, newTailId, EPSILON);
			subNFAStack.push(SubNFA(newHeadId, newTailId));
			continue;
		}

		if (ch == '+') {
			SubNFA subNFA = subNFAStack.top();
			subNFAStack.pop();

			nfa.insert_edge(subNFA.tailId, subNFA.headId, EPSILON);
			subNFAStack.push(SubNFA(newHeadId, newTailId));
			continue;
		}

		if (ch == '?') {
			SubNFA subNFA = subNFAStack.top();
			subNFAStack.pop();

			newHeadId = nfa.get_new_state_id();
			newTailId = nfa.get_new_state_id();
			nfa.insert_edge(newHeadId, subNFA.headId, EPSILON);
			nfa.insert_edge(subNFA.tailId, newTailId, EPSILON);
			nfa.insert_edge(newHeadId, newTailId, EPSILON);
			subNFAStack.push(SubNFA(newHeadId, newTailId));
			continue;
		}

		if (ch == '&') {
			SubNFA subNFA1 = subNFAStack.top();
			subNFAStack.pop();
			SubNFA subNFA2 = subNFAStack.top();
			subNFAStack.pop();

			newHeadId = subNFA2.headId;
			newTailId = subNFA1.tailId;
			nfa.insert_edge(subNFA2.tailId, subNFA1.headId, EPSILON);
			subNFAStack.push(SubNFA(newHeadId, newTailId));
			continue;
		}

		if (ch == '|') {
			SubNFA subNFA1 = subNFAStack.top();
			subNFAStack.pop();
			SubNFA subNFA2 = subNFAStack.top();
			subNFAStack.pop();

			newHeadId = nfa.get_new_state_id();
			newTailId = nfa.get_new_state_id();
			nfa.insert_edge(newHeadId, subNFA1.headId, EPSILON);
			nfa.insert_edge(newHeadId, subNFA2.headId, EPSILON);
			nfa.insert_edge(subNFA1.tailId, newTailId, EPSILON);
			nfa.insert_edge(subNFA2.tailId, newTailId, EPSILON);
			subNFAStack.push(SubNFA(newHeadId, newTailId));
			continue;
		}
	}
	nfa.set_start_state_id(newHeadId);
	nfa.set_end_state_id(newTailId);

	return nfa;
}


/*
*  NFA -> DFA
*/
DFA re::nfa2dfa(NFA& nfa) {
	DFA dfa;
	unordered_set<int> uncheckedDFAStates;

	DFAState startDFAState = DFAState(nfa.get_epsilon_closure(unordered_set<int>({ nfa.get_start_state_id() })));
	int newStateId = dfa.insert_node(startDFAState);
	dfa.set_start_state_id(newStateId);
	dfa.set_end_nfastate_id(nfa.get_end_state_id());

	uncheckedDFAStates.insert(newStateId);
	while (!uncheckedDFAStates.empty()) {
		int uncheckedStateId = *uncheckedDFAStates.begin();
		auto conditionsPaths = nfa.get_conditions_paths(dfa.get_state(uncheckedStateId).nfaStates);
		for (auto conditionPaths : conditionsPaths) {
			unordered_set<int> newEpsilonClosure = nfa.get_epsilon_closure(conditionPaths.second);
			DFAState dfaState = DFAState(newEpsilonClosure);
			if (!dfa.is_exist(dfaState)) {
				newStateId = dfa.insert_node(dfaState);
				uncheckedDFAStates.insert(newStateId);
			}

			dfa.get_state(uncheckedStateId).add_path(dfa.get_state_id(dfaState), conditionPaths.first);
		}
		uncheckedDFAStates.erase(uncheckedStateId);
	}

	return dfa;
}


bool re::is_match(DFA& dfa, string postfixExp) {
	DFAState currentState = dfa.get_state(dfa.get_start_state_id());
	for (char condition : postfixExp) {
		if (currentState.paths.find(condition) != currentState.paths.end()) {
			currentState = dfa.get_state(currentState.paths[condition]);
		}
		else if (currentState.paths.find(DOT) != currentState.paths.end()) {
			currentState = dfa.get_state(currentState.paths[DOT]);
		}
		else {
			return false;
		}
	}
	if (currentState.nfaStates.find(dfa.get_end_nfastate_id()) != currentState.nfaStates.end()) {
		return true;
	}

	return false;
}


/*
*  判断str是否匹配正则式pattern
*/
bool re::match(string str, string pattern) {
	string postfixExp = infix2postfix(pattern);
	NFA nfa = postfix2nfa(postfixExp);
	DFA dfa = nfa2dfa(nfa);
	
	return is_match(dfa, str);
}
