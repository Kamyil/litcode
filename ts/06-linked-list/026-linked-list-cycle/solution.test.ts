import { expect, test } from "bun:test";
import { ListNode, listFrom } from "../../lib/list";
import { detectCycle, hasCycle } from "./solution";

// Builds a list and links the tail to the node at index `pos` (-1 = no cycle).
function withCycle(values: number[], pos: number): { head: ListNode | null; entry: ListNode | null } {
  const head = listFrom(values);
  let entry: ListNode | null = null;
  let tail = head;
  let i = 0;
  for (let n = head; n; n = n.next, i++) {
    if (i === pos) entry = n;
    tail = n;
  }
  if (tail && entry) tail.next = entry;
  return { head, entry };
}

test("hasCycle", () => {
  expect(hasCycle(withCycle([3, 2, 0, -4], 1).head)).toBe(true);
  expect(hasCycle(withCycle([1, 2], 0).head)).toBe(true);
  expect(hasCycle(withCycle([1], -1).head)).toBe(false);
  expect(hasCycle(null)).toBe(false);
});

test("detectCycle", () => {
  for (const [vals, pos] of [[[3, 2, 0, -4], 1], [[1, 2], 0], [[1], 0], [[1, 2, 3, 4, 5, 6, 7], 4]] as const) {
    const { head, entry } = withCycle([...vals], pos);
    expect(detectCycle(head)).toBe(entry);
  }
  expect(detectCycle(withCycle([1, 2, 3], -1).head)).toBe(null);
});
