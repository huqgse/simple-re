#include "dfa.h"
using namespace std;


int DFA::get_start_state_id() {
	return this->startStateId;
}

void DFA::set_start_state_id(int stateId) {
	this->startStateId = stateId;
}

int DFA::get_end_nfastate_id() {
	return this->endNFAStateId;
}

void DFA::set_end_nfastate_id(int stateId) {
	this->endNFAStateId = stateId;
}

int DFA::insert_node(DFAState dfaState) {
	for (auto stateNode : stateNodes) {
		if (stateNode.second.nfaStates == dfaState.nfaStates) {
			return stateNode.first;
		}
	}

	int stateId = stateNum++;
	stateNodes[stateId] = dfaState;

	return stateId;
}

bool DFA::is_exist(DFAState dfaState) {
	for (auto stateNode : stateNodes) {
		if (stateNode.second.nfaStates == dfaState.nfaStates) {
			return true;
		}
	}

	return false;
}

int DFA::get_state_id(DFAState dfaState) {
	for (auto stateNode : stateNodes) {
		if (stateNode.second.nfaStates == dfaState.nfaStates) {
			return stateNode.first;
		}
	}
}


DFAState& DFA::get_state(int stateId) {
	return stateNodes.at(stateId);
}



//unordered_set<int> DFA::move(int dfaState, int condition) {
//	unordered_set<int> nfaStates = stateNodes[dfaState].nfaStates;
//	unordered_set<int> conditionValidDFAStates;
//
//	std::unordered_map<int, std::unordered_set<int>> conditionPaths;
//	auto conditionsPaths = nfa.get_conditions_paths(nfaStates);
//
//	for (auto dfaState : conditionPaths[condition]) {
//		conditionValidDFAStates.insert(dfaState);
//
//	}
//
//	return conditionValidDFAStates;
//}
