`new Heap<ListNode>((a, b) => a.val - b.val)`; push every non-null head.
---
Pop the min, append it to the tail, push `node.next` if it exists. Dummy head again.
