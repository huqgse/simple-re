#pragma once
#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#define EPSILON 0


struct EdgeNode
{
	int stateId;
	int condition;
	EdgeNode* next = nullptr;

	EdgeNode(int stateId, int condition) {
		this->stateId = stateId;
		this->condition = condition;
	}

};


struct NFANode
{
	EdgeNode* first;
};


class NFA {

private:
	std::vector<NFANode> states;
	int stateNum = 0;
	int startStateId;
	int endStateId;

	void work_through(
		EdgeNode* edgeNode,
		std::unordered_set<int>& epsilonClosure,
		std::unordered_map<int, std::unordered_set<int>>& conditionsPaths
	);

public:
	int get_start_state_id();

	void set_start_state_id(int state);

	int get_end_state_id();

	void set_end_state_id(int state);

	NFANode& get_state(int state);

	int get_new_state_id();

	void insert_edge(int src, int dst, int condition);

	std::unordered_set<int> get_epsilon_closure(std::unordered_set<int> nfaStates);

	std::unordered_map<int, std::unordered_set<int>> get_conditions_paths(std::unordered_set<int> nfaStates);

};
