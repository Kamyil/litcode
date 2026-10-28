import { expect, test } from "bun:test";
import { containsDuplicate, containsNearbyDuplicate } from "./solution";

test("containsDuplicate", () => {
  expect(containsDuplicate([1, 2, 3, 1])).toBe(true);
  expect(containsDuplicate([1, 2, 3, 4])).toBe(false);
  expect(containsDuplicate([])).toBe(false);
  expect(containsDuplicate([1, 1, 1, 3, 3, 4, 3, 2, 4, 2])).toBe(true);
});

test("containsNearbyDuplicate", () => {
  expect(containsNearbyDuplicate([1, 2, 3, 1], 3)).toBe(true);
  expect(containsNearbyDuplicate([1, 0, 1, 1], 1)).toBe(true);
  expect(containsNearbyDuplicate([1, 2, 3, 1, 2, 3], 2)).toBe(false);
  expect(containsNearbyDuplicate([1, 2, 1], 0)).toBe(false);
});
