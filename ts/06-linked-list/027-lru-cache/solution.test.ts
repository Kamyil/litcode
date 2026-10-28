import { expect, test } from "bun:test";
import { LRUCache } from "./solution";

test("example", () => {
  const c = new LRUCache(2);
  c.put(1, 1);
  c.put(2, 2);
  expect(c.get(1)).toBe(1);
  c.put(3, 3); // evicts 2
  expect(c.get(2)).toBe(-1);
  c.put(4, 4); // evicts 1
  expect(c.get(1)).toBe(-1);
  expect(c.get(3)).toBe(3);
  expect(c.get(4)).toBe(4);
});

test("update refreshes recency", () => {
  const c = new LRUCache(2);
  c.put(1, 1);
  c.put(2, 2);
  c.put(1, 10);
  c.put(3, 3); // evicts 2, not 1
  expect(c.get(1)).toBe(10);
  expect(c.get(2)).toBe(-1);
});

test("capacity 1", () => {
  const c = new LRUCache(1);
  c.put(1, 1);
  c.put(2, 2);
  expect(c.get(1)).toBe(-1);
  expect(c.get(2)).toBe(2);
});

test("against a model", () => {
  const cap = 5;
  const c = new LRUCache(cap);
  const model = new Map<number, number>();
  let seed = 3;
  const rnd = (n: number) => (seed = (seed * 48271) % 2147483647) % n;
  for (let i = 0; i < 5000; i++) {
    const k = rnd(12);
    if (rnd(2)) {
      const want = model.has(k) ? model.get(k)! : -1;
      if (model.has(k)) {
        model.delete(k);
        model.set(k, want);
      }
      expect(c.get(k)).toBe(want);
    } else {
      model.delete(k);
      model.set(k, i);
      if (model.size > cap) model.delete(model.keys().next().value!);
      c.put(k, i);
    }
  }
});
