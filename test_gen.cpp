#include <bits/stdc++.h>
#define int long long

using namespace std;

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0);
    mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    freopen("./testcase/input.inp", "w", stdout);

    cout << uniform_int_distribution<int>(1, 1e9)(rng);
}