import { expect, test } from "bun:test";
import { lengthOfLongestSubstring } from "./solution";

test("examples", () => {
  expect(lengthOfLongestSubstring("abcabcbb")).toBe(3);
  expect(lengthOfLongestSubstring("bbbbb")).toBe(1);
  expect(lengthOfLongestSubstring("pwwkew")).toBe(3);
});

test("edge cases", () => {
  expect(lengthOfLongestSubstring("")).toBe(0);
  expect(lengthOfLongestSubstring(" ")).toBe(1);
  expect(lengthOfLongestSubstring("abba")).toBe(2); // left must never move backwards
  expect(lengthOfLongestSubstring("dvdf")).toBe(3);
});
