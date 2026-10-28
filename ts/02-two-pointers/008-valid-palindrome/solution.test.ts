import { expect, test } from "bun:test";
import { isPalindrome, validPalindromeOneDelete } from "./solution";

test("isPalindrome", () => {
  expect(isPalindrome("A man, a plan, a canal: Panama")).toBe(true);
  expect(isPalindrome("race a car")).toBe(false);
  expect(isPalindrome(" ")).toBe(true);
  expect(isPalindrome("0P")).toBe(false);
  expect(isPalindrome(".,")).toBe(true);
});

test("validPalindromeOneDelete", () => {
  expect(validPalindromeOneDelete("aba")).toBe(true);
  expect(validPalindromeOneDelete("abca")).toBe(true);
  expect(validPalindromeOneDelete("abc")).toBe(false);
  expect(validPalindromeOneDelete("deeee")).toBe(true);
  expect(validPalindromeOneDelete("cbbcc")).toBe(true);
  expect(validPalindromeOneDelete("abcdefdba")).toBe(false);
});
