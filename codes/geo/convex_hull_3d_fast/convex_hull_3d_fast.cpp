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
// 3D point
template <typename T>
struct Vec3 {
  T x, y, z;
  Vec3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}
  Vec3 operator+(const Vec3& o) const {
    return Vec3(x + o.x, y + o.y, z + o.z);
  }
  Vec3 operator-(const Vec3& o) const {
    return Vec3(x - o.x, y - o.y, z - o.z);
  }
  Vec3 operator*(const T& o) const {
    return Vec3(x * o, y * o, z * o);
  }
  friend T dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }
  friend Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
  }
  T abs() {
    return sqrtl(dot(*this, *this));
  }
};

using LD = long double;
using V = Vec3<LD>;
//@

//@
// Convex Hull 3D
struct Face {
  array<int, 3> v;
  array<int, 3> adj;
  V norm;
  vector<int> future;
  int dead;

  Face(int a, int b, int c, V _norm) : v({a, b, c}), norm(_norm), dead(1e9) {}
};

const LD eps = 1e-9;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

bool operator==(const V& a, const V& b) {
  return fabs(a.x - b.x) < eps && fabs(a.y - b.y) <= eps;
}

void prepare(vector<V>& p) {
  //
  //
  int n = p.size();
  shuffle(p.begin(), p.end(), rng);
  int ind = 0;
  for (int i = 1; i < n; i++) {
    if (ind == 0) {
      if (!(p[0] == p[i])) {
        swap(p[i], p[ind = 1]);
      }
    } else if (ind == 1) {
      if (cross(p[1] - p[0], p[i] - p[0]).abs() > eps) {
        swap(p[i], p[ind = 2]);
      }
    } else {
      if (fabs(dot(cross(p[1] - p[0], p[2] - p[0]), p[i] - p[0])) > eps) {
        swap(p[i], p[ind = 3]);
        break;
      }
    }
  }
  assert(ind = 3);
}

vector<Face> hull3(vector<V> p) {
  prepare(p);
  int n = p.size();
  vector<Face> F;
  vector<vector<int>> conflict(n);
  F.push_back(Face(0, 1, 2, cross(p[1] - p[0], p[2] - p[0])));
  F.push_back(Face(0, 2, 1, cross(p[2] - p[0], p[1] - p[0])));
  F[0].adj = {3 + 2, 3 + 1, 3 + 0};
  F[1].adj = {3 + 2, 3 + 1, 3 + 0};
  //
  for (int i = 3; i < n; i++) {
    for (int f = 0; f < 2; f++) {
      LD t = dot(p[i] - p[F[f].v[0]], F[f].norm);
      if (t > eps) conflict[i].push_back(f);
      if (t < -eps) F[f].future.push_back(t);
    }
  }
  //
  for (int i = 3; i < n; i++) {
    for (Face& face : F) {
      face.dead = min(face.dead, i);
    }
    int v = -1;
    for (int f : conflict[i]) {
      if (F[f].dead != i) continue;
      for (int k = 0; k < 3; k++) {
        int g = F[f].adj[k];
        if ()
      }
    }
  }
}

void solve(int) {
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0), cout.tie(0);
  debug("compiling finished!\n");
  //
  int T = 1;
  cin >> T;
  for (int i = 1; i <= T; i++) {
    solve(i);
  }
  return 0;
}