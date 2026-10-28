import { expect, test } from "bun:test";
import { Heap } from "./solution";

test("min-heap basics", () => {
  const h = new Heap<number>((a, b) => a - b);
  expect(h.size).toBe(0);
  expect(h.pop()).toBeUndefined();
  for (const x of [5, 3, 8, 1, 9, 2]) h.push(x);
  expect(h.size).toBe(6);
  expect(h.peek()).toBe(1);
  const out: number[] = [];
  while (h.size) out.push(h.pop()!);
  expect(out).toEqual([1, 2, 3, 5, 8, 9]);
});

test("max-heap with objects", () => {
  const h = new Heap<{ p: number; name: string }>((a, b) => b.p - a.p);
  h.push({ p: 1, name: "low" });
  h.push({ p: 10, name: "high" });
  h.push({ p: 5, name: "mid" });
  expect(h.pop()!.name).toBe("high");
  expect(h.pop()!.name).toBe("mid");
});

test("from() heapifies and is a heap sort", () => {
  let seed = 9;
  const xs = Array.from({ length: 5000 }, () => (seed = (seed * 16807) % 2147483647) % 1000);
  const h = Heap.from(xs, (a, b) => a - b);
  expect(h.size).toBe(xs.length);
  const out: number[] = [];
  while (h.size) out.push(h.pop()!);
  expect(out).toEqual([...xs].sort((a, b) => a - b));
});

test("from() does not mutate the input array", () => {
  const xs = [3, 1, 2];
  Heap.from(xs, (a, b) => a - b);
  expect(xs).toEqual([3, 1, 2]);
});
