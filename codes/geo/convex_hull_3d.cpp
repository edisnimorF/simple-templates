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
using LD = long double;

const LD EPS = 1e-10, INF = 1e30, pi = acos(-1);

template <typename T>
int fcmp(T x) {
  if (is_same<T, LD>::value) {
    return x < -EPS ? -1 : (x > EPS ? 1 : 0);
  } else
    return (x > 0) - (x < 0);
}

template <typename T>
struct Vec {
  T x, y, z;
  Vec(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
  LD mod() const { return sqrtl((LD)x * x + (LD)y * y + (LD)z * z); }
  friend Vec<LD> base(Vec v) { return v / v.mod(); }
  //
  Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y, z + o.z); }
  Vec operator-(const Vec& o) const { return Vec(x - o.x, y - o.y, z - o.z); }
  Vec operator*(const T& o) const { return Vec(x * o, y * o, z * o); }
  Vec<LD> operator/(const LD& o) const { return Vec<LD>(x / o, y / o, z / o); }
  friend Vec operator*(const T& a, const Vec& b) { return b * a; }
  //
  friend T dot(const Vec& a, const Vec& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }
  friend Vec cross(const Vec& a, const Vec& b) {
    return Vec(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x);
  }
};

using VecD = Vec<LD>;
//@

//@
struct ConvexHull3D {
  using Face = array<int, 3>;
  //
  int n, id;
  vector<vector<bool>> vis;
  vector<Face> face;
  vector<VecD> p;
  //
  ConvexHull3D(int _n, vector<VecD> x = {}) : n(_n) {
    vis.resize(n, vector<bool>(n, false));
    p.resize(n);
    id = 0;
    if (x.empty()) return;
    assert((int)x.size() <= n && x.size() > 3);
    int m = x.size();
    for (int i = 2; i < m; i++) {
      if (fcmp(cross(x[1] - x[0], x[i] - x[0]).mod())) {
        swap(x[i], x[2]);
        break;
      }
    }
    for (int i = 3; i < m; i++) {
      if (fcmp(dot(cross(x[1] - x[0], x[2] - x[0]), x[i] - x[0]))) {
        swap(x[i], x[3]);
        break;
      }
    }
    p[0] = x[0], p[1] = x[1], p[2] = x[2];
    id = 3;
    face = {{0, 1, 2}, {2, 1, 0}};
    for (int i = 3; i < m; i++) addp(x[i]);
  }
  //
  VecD norm(Face f) {
    return base(cross(p[f[1]] - p[f[0]], p[f[2]] - p[f[1]]));
  }

  bool visiable(Face f, VecD x) { return fcmp(dot(norm(f), x - p[f[0]])) > 0; }
  //
  void addp(VecD x) {  // 三维凸包的求解采用增量法。一个点集的三维凸包的外表面由三角形组成。每次添加一个点，会添加一些三角形，并删去一些三角形。
    p[id++] = x;
    vector<Face> nface;
    for (auto f : face) {
      bool ok = visiable(f, x);
      if (!ok) nface.push_back(f);
      for (int i = 0; i < 3; i++) {
        vis[f[i]][f[(i + 1) % 3]] = ok;
      }
    }
    for (auto f : face) {
      for (int i = 0; i < 3; i++) {
        int u = f[i], v = f[(i + 1) % 3];
        if (vis[u][v] && !vis[v][u]) {
          nface.push_back({u, v, id - 1});
        }
      }
    }
    face.swap(nface);
  }
};
//@

void solve(int testId) {
  int n;
  cin >> n;
  vector<VecD> p(n);
  for (int i = 0; i < n; i++) {
    cin >> p[i].x >> p[i].y >> p[i].z;
  }
  ConvexHull3D ch(n, p);
  // solve the surface area
  LD area = 0;
  for (auto f : ch.face) {
    auto a = ch.p[f[0]], b = ch.p[f[1]], c = ch.p[f[2]];
    LD res = cross(b - a, c - a).mod() / 2;
    area += res;
  }
  cout << fixed << setprecision(3) << area << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0), cout.tie(0);
  int T = 1;
  // cin >> T;
  for (int i = 1; i <= T; i++) {
    solve(i);
  }
  return 0;
}
/*@name
Convex Hull 3D
*/

/*@note
https://www.luogu.com.cn/problem/P4724
*/

/*@in
4
0 0 0
1 0 0
0 1 0
0 0 1
*/

/*@out
2.366
*/