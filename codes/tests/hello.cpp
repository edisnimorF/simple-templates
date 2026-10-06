// miku is the most kawaii girl
#include <bits/stdc++.h>
using namespace std;
#ifdef ISLOCAL
#define debug(...) fprintf(stderr, "\033[36m"), fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\033[0m")
#else
#define debug(...)
#endif

//@
void solve(int _testId_) {
	int a, b;
	cin>>a>>b;
	cout<<a+b<<'\n';
}
//@

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
	//
	int T = 1;
	cin >> T;
	for (int i = 1; i <= T; i++) {
		solve(i);
	}
	return 0;
}
/*@name
a pre name
*/
/*@name
A+B problem
*/

/*@note
prenode
*/
/*@note
That is a simple problem. Input A, B and output the answer A+B.
*/

/*@in
1 2
*/

/*@out
3
*/