#include <iostream>
using namespace std;

typedef int ElemType;
typedef struct LNode {
	ElemType data;
	struct LNode *pNext;
}ListNode, *LinkList;

int GetListLength(LinkList pHead) {
	if (pHead == NULL) return -1;

	int length = 0;

	ListNode *pNode = pHead->pNext;
	while (pNode != NULL) {
		length++;
		pNode = pNode->pNext;
	}
	return length;
}


ListNode *FindFirstCommonNode(LinkList pHead1, LinkList pHead2) {
	int l1 = GetListLength(pHead1);
	int l2 = GetListLength(pHead2);

	int lengthDiff = l1 - l2;
	ListNode * pListLong = pHead1->pNext;
	ListNode * pListShort = pHead2->pNext;

	if (l1 < l2) {
		lengthDiff = l2 - l1;
		ListNode * pListLong = pHead2->pNext;
		ListNode * pListShort = pHead1->pNext;
	}

	for (int i = 0; i<lengthDiff; i++) {
		pListLong = pListLong->pNext;
	}

	while (pListLong != NULL && pListShort != NULL && pListLong != pListShort) {
		pListLong = pListLong->pNext;
		pListShort = pListShort -> pNext;
	}
	ListNode * firstcommon = pListLong;
	return firstcommon;
}

int main() {
	return 0;
}


