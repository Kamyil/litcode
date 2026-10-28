export function coinChange(coins: number[], amount: number): number {
  const dp = new Array<number>(amount + 1).fill(Infinity);
  dp[0] = 0;
  for (let a = 1; a <= amount; a++) {
    for (const c of coins) if (c <= a && dp[a - c] + 1 < dp[a]) dp[a] = dp[a - c] + 1;
  }
  return dp[amount] === Infinity ? -1 : dp[amount];
}

export function coinChangeWays(coins: number[], amount: number): number {
  const ways = new Array<number>(amount + 1).fill(0);
  ways[0] = 1;
  for (const c of coins) for (let a = c; a <= amount; a++) ways[a] += ways[a - c];
  return ways[amount];
}
