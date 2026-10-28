import { expect, test } from "bun:test";
import { dailyTemperatures } from "./solution";

test("examples", () => {
  expect(dailyTemperatures([73, 74, 75, 71, 69, 72, 76, 73])).toEqual([1, 1, 4, 2, 1, 1, 0, 0]);
  expect(dailyTemperatures([30, 40, 50, 60])).toEqual([1, 1, 1, 0]);
  expect(dailyTemperatures([30, 60, 90])).toEqual([1, 1, 0]);
});

test("equal temperatures are not warmer", () => {
  expect(dailyTemperatures([50, 50, 50, 51])).toEqual([3, 2, 1, 0]);
});

test("large decreasing input is linear", () => {
  const n = 200_000;
  const temps = Array.from({ length: n }, (_, i) => n - i);
  temps.push(n + 1);
  const out = dailyTemperatures(temps);
  expect(out[0]).toBe(n);
});
