// miku is the most kawaii girl
#include <bits/stdc++.h>
using namespace std;
void debug(const char* msg, ...) {
#ifdef ISLOCAL
  va_list arg;
  static char pbString[512];
  va_start(arg, msg);
  vsprintf(pbString, msg, arg);
  cerr << pbString;
  va_end(arg);
#endif
}

//@

void solve(int) {
}

//@

/*@note note*/
int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0), cout.tie(0);
  debug("compiling finished!\n");
  //
  int T = 1;
  // cin >> T;
  for (int i = 1; i <= T; i++) {
    solve(i);
  }
  return 0;
}