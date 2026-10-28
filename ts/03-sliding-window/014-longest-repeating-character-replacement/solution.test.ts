import { expect, test } from "bun:test";
import { characterReplacement } from "./solution";

test("examples", () => {
  expect(characterReplacement("ABAB", 2)).toBe(4);
  expect(characterReplacement("AABABBA", 1)).toBe(4);
});

test("edge cases", () => {
  expect(characterReplacement("A", 0)).toBe(1);
  expect(characterReplacement("ABCDE", 0)).toBe(1);
  expect(characterReplacement("ABCDE", 10)).toBe(5);
  expect(characterReplacement("AAAB", 0)).toBe(3);
});
