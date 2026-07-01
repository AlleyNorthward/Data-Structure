#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

int main() {
  freopen(".input", "r", stdin);
  int n;
  std::cin >> n;
  std::vector<int> pre(n + 1, 0);

  for (int i = 1; i < n + 1; ++i) {
    int x;
    std::cin >> x;
    pre[i] = pre[i - 1] + x;
  }

  int ans = INT_MIN;
  int min_pre = 0;
  for (int r = 1; r < n + 1; ++r) {
    ans = std::max(ans, pre[r] - min_pre);
    min_pre = std::min(min_pre, pre[r]);
  }

  std::cout << ans;
}
