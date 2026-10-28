import { expect, test } from "bun:test";
import { ladderLength } from "./solution";

test("examples", () => {
  expect(ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log", "cog"])).toBe(5);
  expect(ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log"])).toBe(0);
  expect(ladderLength("a", "c", ["a", "b", "c"])).toBe(2);
  expect(ladderLength("hot", "dog", ["hot", "dog"])).toBe(0);
});
