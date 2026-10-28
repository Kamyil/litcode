import { ListNode } from "../../lib/list";

export function reverseList(head: ListNode | null): ListNode | null {
  let prev: ListNode | null = null;
  let curr = head;
  while (curr) {
    const next = curr.next;
    curr.next = prev;
    prev = curr;
    curr = next;
  }
  return prev;
}

export function reverseListRecursive(head: ListNode | null): ListNode | null {
  if (!head || !head.next) return head;
  const newHead = reverseListRecursive(head.next);
  head.next.next = head;
  head.next = null;
  return newHead;
}

export function reverseBetween(head: ListNode | null, left: number, right: number): ListNode | null {
  const dummy = new ListNode(0, head);
  let before = dummy;
  for (let i = 1; i < left; i++) before = before.next!;
  const start = before.next!; // ends up as the tail of the reversed part
  for (let i = left; i < right; i++) {
    const moved = start.next!;
    start.next = moved.next;
    moved.next = before.next;
    before.next = moved;
  }
  return dummy.next;
}
