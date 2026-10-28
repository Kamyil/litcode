import { ListNode } from "../../lib/list";

function meetingPoint(head: ListNode | null): ListNode | null {
  let slow = head;
  let fast = head;
  while (fast && fast.next) {
    slow = slow!.next;
    fast = fast.next.next;
    if (slow === fast) return slow;
  }
  return null;
}

export function hasCycle(head: ListNode | null): boolean {
  return meetingPoint(head) !== null;
}

export function detectCycle(head: ListNode | null): ListNode | null {
  let meet = meetingPoint(head);
  if (!meet) return null;
  let p = head!;
  while (p !== meet) {
    p = p.next!;
    meet = meet!.next!;
  }
  return p;
}
