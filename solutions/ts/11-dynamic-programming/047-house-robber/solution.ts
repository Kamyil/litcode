export function rob(nums: number[]): number {
  let prev2 = 0;
  let prev1 = 0;
  for (const x of nums) [prev2, prev1] = [prev1, Math.max(prev1, prev2 + x)];
  return prev1;
}

export function robCircle(nums: number[]): number {
  if (nums.length === 1) return nums[0];
  return Math.max(rob(nums.slice(1)), rob(nums.slice(0, -1)));
}
