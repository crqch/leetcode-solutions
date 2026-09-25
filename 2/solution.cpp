struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    int carry = 0;
    ListNode *root = nullptr, *l_temp = nullptr;
    int a, b, temp;

    while (true) {
      a = 0;
      b = 0;
      if (l1 != nullptr) {
        a = l1->val;
        l1 = l1->next;
      }
      if (l2 != nullptr) {
        b = l2->val;
        l2 = l2->next;
      }

      temp = carry + a + b;
      carry = temp / 10;

      if (!root) {
        root = new ListNode(temp % 10);
        l_temp = root;
      } else if (l1 != nullptr || l2 != nullptr || temp % 10 != 0 ||
                 carry != 0) {
        l_temp->next = new ListNode(temp % 10);
        l_temp = l_temp->next;
      }

      if (l1 == nullptr && l2 == nullptr && carry == 0)
        break;
    }

    return root;
  }
};
