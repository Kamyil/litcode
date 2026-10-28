export function climbStairs(n: number): number {
  let a = 1; // ways to reach step i-1
  let b = 1; // ways to reach step i
  for (let i = 1; i < n; i++) [a, b] = [b, a + b];
  return b;
}

export function minCostClimbingStairs(cost: number[]): number {
  let a = 0; // min cost to stand on step i-2
  let b = 0; // min cost to stand on step i-1
  for (let i = 2; i <= cost.length; i++) [a, b] = [b, Math.min(b + cost[i - 1], a + cost[i - 2])];
  return b;
}
