import { expect, test } from "bun:test";
import { minWindow } from "./solution";

test("examples", () => {
  expect(minWindow("ADOBECODEBANC", "ABC")).toBe("BANC");
  expect(minWindow("a", "a")).toBe("a");
  expect(minWindow("a", "aa")).toBe("");
});

test("multiplicity", () => {
  expect(minWindow("aaflslflsldkalskaaa", "aaa")).toBe("aaa");
  expect(minWindow("abcabdebac", "cda")).toBe("cabd");
});
