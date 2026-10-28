import { ListNode } from "../../lib/list";
import { Heap } from "../033-binary-heap/solution";

export function mergeKLists(lists: (ListNode | null)[]): ListNode | null {
  const heap = Heap.from(
    lists.filter((l): l is ListNode => l !== null),
    (a, b) => a.val - b.val,
  );
  const dummy = new ListNode(0);
  let tail = dummy;
  while (heap.size) {
    const n = heap.pop()!;
    tail = tail.next = n;
    if (n.next) heap.push(n.next);
  }
  tail.next = null;
  return dummy.next;
}
