import { expect, test } from "bun:test";
import { ListNode, listFrom, listToArray } from "../../lib/list";
import { mergeTwoLists } from "./solution";

test("examples", () => {
  expect(listToArray(mergeTwoLists(listFrom([1, 2, 4]), listFrom([1, 3, 4])))).toEqual([1, 1, 2, 3, 4, 4]);
  expect(mergeTwoLists(null, null)).toBe(null);
  expect(listToArray(mergeTwoLists(null, listFrom([0])))).toEqual([0]);
});

test("reuses the original nodes", () => {
  const a = listFrom([1, 5]);
  const b = listFrom([2]);
  const nodes = new Set<ListNode>([a!, a!.next!, b!]);
  let n = mergeTwoLists(a, b);
  while (n) {
    expect(nodes.has(n)).toBe(true);
    n = n.next;
  }
});
