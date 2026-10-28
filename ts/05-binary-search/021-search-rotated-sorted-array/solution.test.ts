import { expect, test } from "bun:test";
import { findMin, searchRotated } from "./solution";

test("findMin", () => {
  expect(findMin([3, 4, 5, 1, 2])).toBe(1);
  expect(findMin([4, 5, 6, 7, 0, 1, 2])).toBe(0);
  expect(findMin([11, 13, 15, 17])).toBe(11);
  expect(findMin([2, 1])).toBe(1);
  expect(findMin([1])).toBe(1);
});

test("searchRotated", () => {
  expect(searchRotated([4, 5, 6, 7, 0, 1, 2], 0)).toBe(4);
  expect(searchRotated([4, 5, 6, 7, 0, 1, 2], 3)).toBe(-1);
  expect(searchRotated([1], 0)).toBe(-1);
  expect(searchRotated([3, 1], 1)).toBe(1);
  expect(searchRotated([5, 1, 3], 5)).toBe(0);
});

test("every rotation, every target", () => {
  const base = Array.from({ length: 23 }, (_, i) => i * 2);
  for (let r = 0; r < base.length; r++) {
    const rot = [...base.slice(r), ...base.slice(0, r)];
    expect(findMin(rot)).toBe(0);
    for (let t = -1; t <= 46; t++) expect(searchRotated(rot, t)).toBe(rot.indexOf(t));
  }
});
