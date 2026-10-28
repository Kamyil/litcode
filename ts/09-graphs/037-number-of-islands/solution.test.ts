import { expect, test } from "bun:test";
import { maxAreaOfIsland, numIslands } from "./solution";

const g = (rows: string[]) => rows.map((r) => r.split(""));

test("numIslands", () => {
  expect(numIslands(g(["11110", "11010", "11000", "00000"]))).toBe(1);
  expect(numIslands(g(["11000", "11000", "00100", "00011"]))).toBe(3);
  expect(numIslands(g(["0"]))).toBe(0);
  expect(numIslands(g(["101", "010", "101"]))).toBe(5);
});

test("maxAreaOfIsland", () => {
  expect(maxAreaOfIsland(g(["11000", "11000", "00100", "00011"]))).toBe(4);
  expect(maxAreaOfIsland(g(["000"]))).toBe(0);
});

test("does not mutate input", () => {
  const grid = g(["110", "011"]);
  numIslands(grid);
  maxAreaOfIsland(grid);
  expect(grid).toEqual(g(["110", "011"]));
});

test("big all-land grid (no stack overflow)", () => {
  const big = Array.from({ length: 300 }, () => Array(300).fill("1"));
  expect(numIslands(big)).toBe(1);
  expect(maxAreaOfIsland(big)).toBe(90_000);
});
