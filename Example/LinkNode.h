#include "PreComplier.h"

namespace LinkNode
{
    using namespace std;

    struct ListNode {
        int val;
        ListNode* next;
        ListNode() {}
        ListNode(int x) : val(x), next(NULL) {}
    };

    class Solution {
    public:
        // 判断是否相交
        ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
            ListNode* opt1 = headA;
            ListNode* opt2 = headB;

            while (opt1 != opt2)
            {
                if (!opt1)
                    opt1 = headB;
                else
                    opt1 = opt1->next;

                if (!opt2)
                    opt2 = headA;
                else
                    opt2 = opt2->next;
            }

            return opt1;
        }

        // 反转
        ListNode* reverseList(ListNode* head) {
            if (!head || !head->next)
                return head;

            ListNode* newHead = reverseList(head->next);
            head->next->next = head;
            head->next = nullptr;

            return newHead;
        }

        // 排序列表
        vector<int> sortArray(vector<int>& nums) {
            temp.resize(nums.size());
            sortArray(nums, 0, nums.size() - 1);

            return nums;
        }
    private:
        vector<int> temp;
        void sortArray(vector<int>& nums, int left, int right)
        {
            if (left >= right)
                return;

            int mid = left + (right - left) / 2;

            sortArray(nums, left, mid);
            sortArray(nums, mid + 1, right);

            merge(nums, left, mid, right);
        }

        void merge(vector<int>& nums, int left, int mid, int right)
        {
            int i = left, j = mid + 1, opt = left;
            while (i <= right)
            {
                temp[i] = nums[i];
                i++;
            }
            i = left;

            while (i <= mid && j <= right)
            {
                if (temp[i] <= temp[j])
                {
                    nums[opt] = temp[i];
                    i++;
                }
                else
                {
                    nums[opt] = temp[j];
                    j++;
                }
                opt++;
            }

            while (i <= mid)
                nums[opt++] = temp[i++];

            while (j <= right)
                nums[opt++] = temp[j++];
        }

        void sort(vector<int>& nums, int left, int right)
        {
            if (left >= right)
            {
                return;
            }
            int mid = partion(nums, left, right);
            sort(nums, left, mid - 1);
            sort(nums, mid + 1, right);
        }

        // 4,3,2,5,1,6 ->3,4,2,5,1,6 ->3,2,4,5,1,6
        int partion(vector<int>& nums, int left, int right)
        {
            // left是初始化坑位
            int key = nums[left];

            // 循环退出条件是left == right
            while (left < right)
            {
                // 首先需要从右侧找一个比key小的数填进来
                while (left < right && nums[right] >= key)
                {
                    --right;
                }

                // right成为了一个坑位
                if (left < right)
                    nums[left] = nums[right];

                // 再次从左侧找一个不小于key的数填进来
                while (left < right && nums[left] < key)
                {
                    ++left;
                }

                // left成为了一个坑位
                if (left < right)
                    nums[right] = nums[left];
            }

            nums[left] = key;
            return left;
        }

        ListNode* reverseList(ListNode* head, ListNode* last) {
            if (head == last || head->next == last)
            {
                return head;
            }

            ListNode* newH = reverseList(head->next, last);
            head->next->next = head;
            head->next = last;

            return newH;
        }
    public:
        ListNode* reverseBetween(ListNode* head, int left, int right) {
            ListNode* dummy = new ListNode();
            dummy->next = head;
            ListNode* opt = dummy;
            ListNode* tail = head;
            ListNode* last = head;
            while (--left)
            {
                tail = tail->next;
                opt = opt->next;
            }
            while (right--) last = last->next;

            ListNode* nHead = reverseList(tail, last);
            opt->next = nHead;

            return dummy->next;
        }

        vector<int> dailyTemperatures(vector<int>& temperatures) {
            vector<int> res(temperatures.size(), 0);
            stack<std::pair<int, int>> temp;

            for (int i = 0; i < temperatures.size(); i++)
            {
                while (!temp.empty())
                {
                    auto iter = temp.top();

                    if (iter.first < temperatures[i])
                    {
                        temp.pop();
                        res[iter.second] = i - iter.second;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }

                temp.push({ temperatures[i] ,i });
            }

            return res;
        }
    };


}
