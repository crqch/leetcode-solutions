#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  string convert(string s, int numRows) {
    if (numRows == 1 || numRows >= s.size())
      return s;
    vector<string> rows(numRows);

    int x = 0;
    int y = 0;
    bool up = false;

    for (int i = 0; i < s.size(); i++) {
      rows[y].append(string(1, s[i]));
      if (up) {
        y -= 1;
        x += 1;
        if (y == 0) {
          up = false;
        }
      } else {
        y += 1;
        if (y == numRows - 1)
          up = true;
      }
    }

    string res = "";

    for (auto &row : rows) {
      res.append(row);
    }

    return res;
  }
};
