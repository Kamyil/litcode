import { expect, test } from "bun:test";
import { minDistance } from "./solution";

test("examples", () => {
  expect(minDistance("horse", "ros")).toBe(3);
  expect(minDistance("intention", "execution")).toBe(5);
  expect(minDistance("", "abc")).toBe(3);
  expect(minDistance("abc", "")).toBe(3);
  expect(minDistance("kitten", "sitting")).toBe(3);
  expect(minDistance("same", "same")).toBe(0);
});
