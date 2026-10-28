import { expect, test } from "bun:test";
import { lcsString, longestCommonSubsequence } from "./solution";

const isSub = (s: string, of: string) => {
  let j = 0;
  for (const c of of) if (j < s.length && s[j] === c) j++;
  return j === s.length;
};

test("length", () => {
  expect(longestCommonSubsequence("abcde", "ace")).toBe(3);
  expect(longestCommonSubsequence("abc", "abc")).toBe(3);
  expect(longestCommonSubsequence("abc", "def")).toBe(0);
  expect(longestCommonSubsequence("", "abc")).toBe(0);
});

test("string", () => {
  for (const [a, b] of [["abcde", "ace"], ["AGGTAB", "GXTXAYB"], ["abc", "def"], ["bsbininm", "jmjkbkjkv"]]) {
    const s = lcsString(a, b);
    expect(s.length).toBe(longestCommonSubsequence(a, b));
    expect(isSub(s, a) && isSub(s, b)).toBe(true);
  }
});

test("2000 x 2000", () => {
  const a = "ab".repeat(1000);
  const b = "ba".repeat(1000);
  expect(longestCommonSubsequence(a, b)).toBe(1999);
});
