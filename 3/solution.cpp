#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    int maxV = 0;
    int i = 0;
    int table[200];

    for (int i = 0; i < 200; i++)
      table[i] = 0;

    for (int j = 0; j < s.size(); j++) {
      table[s[j]]++;
      while (table[s[j]] > 1) {
        table[s[i]]--;
        i++;
      }
      maxV = max(maxV, j - i + 1);
    }
    return maxV;
  }
};
