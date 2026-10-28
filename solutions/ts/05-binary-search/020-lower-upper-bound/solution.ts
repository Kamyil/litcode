export function lowerBound(nums: number[], x: number): number {
  let lo = 0;
  let hi = nums.length; // answer always in [lo, hi]
  while (lo < hi) {
    const mid = (lo + hi) >>> 1;
    if (nums[mid] < x) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

export function upperBound(nums: number[], x: number): number {
  let lo = 0;
  let hi = nums.length;
  while (lo < hi) {
    const mid = (lo + hi) >>> 1;
    if (nums[mid] <= x) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

export function search(nums: number[], x: number): number {
  const i = lowerBound(nums, x);
  return i < nums.length && nums[i] === x ? i : -1;
}

export function countInRange(nums: number[], lo: number, hi: number): number {
  return upperBound(nums, hi) - lowerBound(nums, lo);
}
