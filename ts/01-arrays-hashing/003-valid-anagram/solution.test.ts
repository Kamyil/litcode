import { expect, test } from "bun:test";
import { isAnagram } from "./solution";

test("examples", () => {
  expect(isAnagram("anagram", "nagaram")).toBe(true);
  expect(isAnagram("rat", "car")).toBe(false);
});

test("edge cases", () => {
  expect(isAnagram("", "")).toBe(true);
  expect(isAnagram("a", "ab")).toBe(false);
  expect(isAnagram("aacc", "ccac")).toBe(false);
});
