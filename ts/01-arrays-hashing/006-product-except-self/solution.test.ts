import { expect, test } from "bun:test";
import { productExceptSelf } from "./solution";

const fix = (xs: number[]) => xs.map((x) => (Object.is(x, -0) ? 0 : x));

test("examples", () => {
  expect(fix(productExceptSelf([1, 2, 3, 4]))).toEqual([24, 12, 8, 6]);
  expect(fix(productExceptSelf([-1, 1, 0, -3, 3]))).toEqual([0, 0, 9, 0, 0]);
});

test("two zeros", () => {
  expect(fix(productExceptSelf([0, 4, 0]))).toEqual([0, 0, 0]);
});
