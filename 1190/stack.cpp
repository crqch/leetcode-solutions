#include <algorithm>
#include <stack>
#include <string>
using namespace std;

class Solution {
public:
  string reverseParentheses(string s) {
    stack<string> d;
    d.push("");

    for (auto &c : s) {

      if (c == '(') {
        d.push("");
        continue;
      }

      if (c == ')') {
        string s = d.top();
        reverse(s.begin(), s.end());

        d.pop();

        d.top().append(s);
        continue;
      }

      d.top().append(string(1, c));
    }

    string res = d.top();

    return res;
  }
};
