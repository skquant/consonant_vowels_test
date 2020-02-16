#include <iostream>
#include "encoding.h"
#include "jamo.h"

int main()
{
	using namespace std;
	for (;;) {
		string s;
		getline(cin, s);
		if (!cin.fail() && !s.empty()) {
			string t;
			WansungBreakJamo(s, t);
			cout << t << endl;
		} else {
			break;
		}
	}
}
