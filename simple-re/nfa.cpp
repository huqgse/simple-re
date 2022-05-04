#include "nfa.h"
using namespace std;


void NFA::insert_edge(int src, int dst, int condition) {
	int t = max(src, dst);
	if (max(src, dst) + 1 > states.size()) {
		states.resize(max(src, dst) + 1);
	}

	EdgeNode* newEdgeNode = new EdgeNode(dst, condition);
	if (states[src].first == nullptr) {
		states[src].first = newEdgeNode;
	}
	else {
		EdgeNode* nextNode = states[src].first;
		while (nextNode->next != nullptr) {
			nextNode = nextNode->next;
		}
		nextNode->next = newEdgeNode;
	}
}


int NFA::get_new_state_id() {
	return stateNum++;
}


int NFA::get_start_state_id() {
	return startStateId;
}


void NFA::set_start_state_id(int stateId) {
	startStateId = stateId;
}


int NFA::get_end_state_id() {
	return endStateId;
}


void NFA::set_end_state_id(int stateId) {
	endStateId = stateId;
}


NFANode& NFA::get_state(int stateId) {
	if (stateId - 1 > states.size()) {
		exit(-1);
	}
	return states[stateId];
}


void NFA::work_through(
	EdgeNode* edgeNode,
	unordered_set<int>& epsilonClosure,
	unordered_map<int, unordered_set<int>>& conditionsPaths) {
	while (edgeNode != nullptr) {
		if (edgeNode->condition == EPSILON) {
			epsilonClosure.insert(edgeNode->stateId);
			work_through(states[edgeNode->stateId].first, epsilonClosure, conditionsPaths);
		}
		else {
			conditionsPaths[edgeNode->condition].insert(edgeNode->stateId);
		}
		edgeNode = edgeNode->next;
	}
}


unordered_set<int> NFA::get_epsilon_closure(unordered_set<int> nfaStates) {
	unordered_set<int> epsilonClosure;
	unordered_map<int, unordered_set<int>> conditionsPaths;
	for (auto nfaState : nfaStates) {
		epsilonClosure.insert(nfaState);
		work_through(states[nfaState].first, epsilonClosure, conditionsPaths);
	}

	return epsilonClosure;
}


unordered_map<int, unordered_set<int>> NFA::get_conditions_paths(unordered_set<int> nfaStates) {
	unordered_set<int> epsilonClosure;
	unordered_map<int, unordered_set<int>> conditionsPaths;
	for (auto nfaState : nfaStates) {
		work_through(states[nfaState].first, epsilonClosure, conditionsPaths);
	}

	return conditionsPaths;
}
