Iterative: `next = curr.next; curr.next = prev; prev = curr; curr = next`. Return `prev`.
---
Recursive: reverse the rest, then `head.next.next = head; head.next = null`.
---
reverseBetween: a dummy node before head avoids the `left === 1` special case. Walk to the node
before `left`, then repeatedly move the node after the sublist's start to the front of the sublist.
