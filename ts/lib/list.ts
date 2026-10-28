// Singly linked list node used by the linked-list problems.
export class ListNode {
  constructor(
    public val: number,
    public next: ListNode | null = null,
  ) {}
}

export function listFrom(values: number[]): ListNode | null {
  let head: ListNode | null = null;
  for (let i = values.length - 1; i >= 0; i--) head = new ListNode(values[i], head);
  return head;
}

export function listToArray(head: ListNode | null, limit = 10_000): number[] {
  const out: number[] = [];
  while (head && out.length < limit) {
    out.push(head.val);
    head = head.next;
  }
  return out;
}
