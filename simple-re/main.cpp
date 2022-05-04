#include <iostream>
#include "re.h"
using namespace std;



int main() {
	string pattern = "(ab)+(xy?)mn.";	

	cout << re::match("ababxmnz", pattern) << endl;
}
