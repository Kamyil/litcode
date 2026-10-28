`slow = fast = head; while (fast && fast.next) { slow = slow.next; fast = fast.next.next; if (slow === fast) ... }`
---
After they meet, put one pointer back at `head`; advance both one step at a time; they meet at the start.
