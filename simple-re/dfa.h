#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "nfa.h"


struct DFAState
{
	std::unordered_set<int> nfaStates;
	// first: condition, second: dfaState
	std::unordered_map<int, int> paths;

	DFAState() {
		this->nfaStates = std::unordered_set<int>();
		this->paths = std::unordered_map<int, int>();
	}

	DFAState(std::unordered_set<int> nfaStates) {
		this->nfaStates = nfaStates;
		this->paths = std::unordered_map<int, int>();
	}

	DFAState(std::unordered_set<int> nfaStates, std::unordered_map<int, int> paths) {
		this->nfaStates = nfaStates;
		this->paths = paths;
	}

	void add_path(int dfaState, int condition) {
		this->paths[condition] = dfaState;
	}
};


class DFA {

private:

	std::unordered_map<int, DFAState> stateNodes;
	int stateNum = 0;
	int startStateId;
	int endNFAStateId;

public:

	int get_start_state_id();

	void set_start_state_id(int stateId);

	int get_end_nfastate_id();

	void set_end_nfastate_id(int stateId);

	int insert_node(DFAState dfaState);

	int get_state_id(DFAState dfaState);

	DFAState& get_state(int stateId);

	bool is_exist(DFAState dfaState);

	//	std::unordered_set<int> move(int dfaState, int condition);

};
