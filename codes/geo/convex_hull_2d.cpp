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
template <typename T>
struct Vec {
  T x, y;
  Vec(T _x = 0, T _y = 0) : x(_x), y(_y) {}
  Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y); }
  Vec operator-(const Vec& o) const { return Vec(x - o.x, y - o.y); }
  Vec operator*(const T& o) const { return Vec(x * o, y * o); }
  friend T dot(const Vec& a, const Vec& b) { return a.x * b.x + a.y * b.y; }
  friend T cross(const Vec& a, const Vec& b) { return a.x * b.y - a.y * b.x; }
};

using LD = long double;
using V = Vec<LD>;

const LD eps = 1e-10;

vector<V> convexHull(vector<V> p) {
  sort(p.begin(), p.end(), [&](const V& a, const V& b) { return tie(a.x, a.y) < tie(b.x, b.y); });
  p.erase(unique(p.begin(), p.end(), [](const V& a, const V& b) { return fabs(a.x - b.x) <= eps && fabs(a.y - b.y) <= eps; }), p.end());  // 删除重复的元素。这里若采用全整形计算则不需要 eps.
  int n = p.size();
  vector<V> hull = {p[0]};
  for (int i = 1; i < n; i++) {
    while ((int)hull.size() >= 2) {
      V bk = hull.back(), bk2 = *++hull.rbegin();
      if (cross(bk - bk2, p[i] - bk) >= 0) break;
      hull.pop_back();
    }
    hull.push_back(p[i]);
  }
  int rec = hull.size();
  for (int i = n - 2; i >= 0; i--) {
    while ((int)hull.size() >= rec + 1) {
      V bk = hull.back(), bk2 = *++hull.rbegin();
      if (cross(bk - bk2, p[i] - bk) >= 0) break;
      hull.pop_back();
    }
    if (i > 0) hull.push_back(p[i]);
  }
  return hull;
}
//@

int main() {
  int n;
  cin >> n;
  vector<V> p(n);
  for (int i = 0; i < n; i++) cin >> p[i].x >> p[i].y;
  vector<V> hull = convexHull(p);
  int m = hull.size();
  LD length = 0;
  for (int i = 0; i < m; i++) {
    V a = hull[i], b = hull[(i + 1) % m];
    length += sqrt(dot(a - b, a - b));
  }
  //
  for (auto [x, y] : hull) debug("%Lf %Lf\n", x, y);
  //
  cout << fixed << setprecision(2) << length << '\n';
  return 0;
}

/*@name
Convex Hull 2D
*/

/*@in
4
4 8
4 12
5 9.3
7 8
*/
/*@out
12.00
*/

/*@note
P2742 【模板】二维凸包 / [USACO5.1] 圈奶牛Fencing the Cows
link: https://www.luogu.com.cn/problem/P2742。测试为输出凸包周长。
本代码实现为 Andrew。写 Graham 在处理共线情况可能出 bug。
*/

/*
10
0 -4
0 -1
2 2
0 -1
2 -1
1 -1
-1 2
4 -3
1 3
4 -1
*/