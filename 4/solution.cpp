#include <algorithm>
#include <vector>
using namespace std;

class Solution {
public:
  double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
    if (nums1.size() > nums2.size()) {
      return findMedianSortedArrays(nums2, nums1);
    }

    if (nums1.size() == 0)
      return median(nums2);

    int n = nums1.size();
    int m = nums2.size();

    int halfIndex = (n + m) / 2;
    int i = n / 2;
    int step = max(1, n / 4);

    int j = halfIndex - i - 1;

    while (true) {
      if (i == -1 || j == -1)
        break;
      if (j + 1 < m && nums1[i] > nums2[j + 1]) {
        i -= step;
      } else if (i + 1 < n && nums2[j] > nums1[i + 1]) {
        i += step;
      } else {
        break;
      }
      step = max(1, step / 2);
      j = halfIndex - i - 1;
    }

    if ((n + m) % 2 == 0) {
      vector<int> nums;

      if (i != -1) {

        nums.push_back(nums1[i]);
        if (i > 0) {
          nums.push_back(nums1[i - 1]);
        }
      }
      if (j != -1) {

        nums.push_back(nums2[j]);
        if (j > 0) {
          nums.push_back(nums2[j - 1]);
        }
      }

      sort(nums.begin(), nums.end());

      return (nums[nums.size() - 1] + nums[nums.size() - 2]) / 2.0;
    } else {
      if (i == -1)
        return nums2[j];
      if (j == -1)
        return nums1[i];
      return max(nums1[i], nums2[j]);
    }
  }

  double median(vector<int> &nums) {
    if (nums.size() % 2 == 0) {
      return (nums[(nums.size() - 1) / 2] + nums[nums.size() / 2]) / 2.0;
    } else {
      return nums[nums.size() / 2];
    }
  }
};
