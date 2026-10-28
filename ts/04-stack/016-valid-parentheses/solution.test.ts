import { expect, test } from "bun:test";
import { isValid } from "./solution";

test("examples", () => {
  expect(isValid("()")).toBe(true);
  expect(isValid("()[]{}")).toBe(true);
  expect(isValid("(]")).toBe(false);
  expect(isValid("([)]")).toBe(false);
  expect(isValid("{[]}")).toBe(true);
});

test("edge cases", () => {
  expect(isValid("")).toBe(true);
  expect(isValid("(")).toBe(false);
  expect(isValid("]")).toBe(false);
  expect(isValid("((")).toBe(false);
});
