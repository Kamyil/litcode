import { expect, test } from "bun:test";
import { MedianFinder } from "./solution";

test("example", () => {
  const m = new MedianFinder();
  m.addNum(1);
  m.addNum(2);
  expect(m.findMedian()).toBe(1.5);
  m.addNum(3);
  expect(m.findMedian()).toBe(2);
});

test("against sorting", () => {
  const m = new MedianFinder();
  const all: number[] = [];
  let seed = 5;
  for (let i = 0; i < 1500; i++) {
    const x = ((seed = (seed * 16807) % 2147483647) % 2000) - 1000;
    m.addNum(x);
    all.push(x);
    all.sort((a, b) => a - b);
    const n = all.length;
    const want = n % 2 ? all[(n - 1) / 2] : (all[n / 2 - 1] + all[n / 2]) / 2;
    expect(m.findMedian()).toBe(want);
  }
});
