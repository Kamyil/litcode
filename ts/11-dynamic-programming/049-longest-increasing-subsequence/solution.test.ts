import { expect, test } from "bun:test";
import { lengthOfLIS, lisSequence } from "./solution";

function isSubsequence(sub: number[], of: number[]): boolean {
  let j = 0;
  for (const x of of) if (j < sub.length && sub[j] === x) j++;
  return j === sub.length;
}

test("lengthOfLIS", () => {
  expect(lengthOfLIS([10, 9, 2, 5, 3, 7, 101, 18])).toBe(4);
  expect(lengthOfLIS([0, 1, 0, 3, 2, 3])).toBe(4);
  expect(lengthOfLIS([7, 7, 7, 7])).toBe(1);
  expect(lengthOfLIS([])).toBe(0);
});

test("lengthOfLIS is n log n", () => {
  const nums = Array.from({ length: 200_000 }, (_, i) => (i * 7919) % 200_003);
  expect(lengthOfLIS(nums)).toBeGreaterThan(0);
});

test("lisSequence", () => {
  for (const nums of [[10, 9, 2, 5, 3, 7, 101, 18], [0, 1, 0, 3, 2, 3], [3, 1, 2], [5]]) {
    const seq = lisSequence(nums);
    expect(seq.length).toBe(lengthOfLIS(nums));
    expect(isSubsequence(seq, nums)).toBe(true);
    for (let i = 1; i < seq.length; i++) expect(seq[i]).toBeGreaterThan(seq[i - 1]);
  }
});
