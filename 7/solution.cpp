#include <cstdint>
#include <limits>

class Solution {
public:
  int reverse(int x) {

    int res = 0;

    while (x != 0) {
      if (res < INT32_MIN / 10)
        return 0;
      if (res > INT32_MAX / 10)
        return 0;
      res *= 10;
      res += x % 10;
      x /= 10;
    }

    return res;
  }
};
