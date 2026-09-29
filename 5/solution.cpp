#include <string>

using namespace std;

class Solution {
public:
  string longestPalindrome(string s) {
    string longest = "";

    for (int i = 0; i < s.size(); i++) {
      int j = 0;
      while (i - j - 1 >= 0 && i + j + 1 < s.size() &&
             s[i - j - 1] == s[i + j + 1]) {

        j += 1;
      }
      if (j * 2 + 1 > longest.size()) {
        longest = s.substr(i - j, 2 * j + 1);
      }
    }

    for (int i = 0; i < s.size(); i++) {
      int j = -1;
      while (i - j - 1 >= 0 && i + j + 2 < s.size() &&
             s[i - j - 1] == s[i + j + 2]) {
        j += 1;
      }

      if (j != -1) {

        if ((j + 1) * 2 > longest.size()) {
          longest = s.substr(i - j, 2 * (j + 1));
        }
      }
    }

    return longest;
  }
};
