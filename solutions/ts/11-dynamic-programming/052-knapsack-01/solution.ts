export function knapsack(weights: number[], values: number[], capacity: number): number {
  const dp = new Array<number>(capacity + 1).fill(0);
  for (let i = 0; i < weights.length; i++) {
    for (let c = capacity; c >= weights[i]; c--) dp[c] = Math.max(dp[c], dp[c - weights[i]] + values[i]);
  }
  return dp[capacity];
}

export function canPartition(nums: number[]): boolean {
  const sum = nums.reduce((a, b) => a + b, 0);
  if (sum % 2) return false;
  const target = sum / 2;
  const reach = new Uint8Array(target + 1);
  reach[0] = 1;
  for (const x of nums) for (let s = target; s >= x; s--) if (reach[s - x]) reach[s] = 1;
  return reach[target] === 1;
}
