import { expect, test } from "bun:test";
import { countInRange, lowerBound, search, upperBound } from "./solution";

const nums = [1, 2, 2, 2, 5, 7];

test("bounds", () => {
  expect(lowerBound(nums, 2)).toBe(1);
  expect(upperBound(nums, 2)).toBe(4);
  expect(lowerBound(nums, 3)).toBe(4);
  expect(upperBound(nums, 3)).toBe(4);
  expect(lowerBound(nums, 0)).toBe(0);
  expect(upperBound(nums, 9)).toBe(6);
  expect(lowerBound([], 1)).toBe(0);
});

test("search", () => {
  expect(search(nums, 5)).toBe(4);
  expect(search(nums, 2)).toBe(1);
  expect(search(nums, 6)).toBe(-1);
  expect(search([], 6)).toBe(-1);
});

test("countInRange", () => {
  expect(countInRange(nums, 2, 5)).toBe(4);
  expect(countInRange(nums, 3, 4)).toBe(0);
  expect(countInRange(nums, -10, 10)).toBe(6);
});

test("brute-force cross-check", () => {
  const arr = Array.from({ length: 500 }, (_, i) => Math.floor(i / 3)).sort((a, b) => a - b);
  for (let x = -2; x < 180; x++) {
    expect(lowerBound(arr, x)).toBe(arr.findIndex((v) => v >= x) === -1 ? arr.length : arr.findIndex((v) => v >= x));
    expect(upperBound(arr, x)).toBe(arr.findIndex((v) => v > x) === -1 ? arr.length : arr.findIndex((v) => v > x));
  }
});
