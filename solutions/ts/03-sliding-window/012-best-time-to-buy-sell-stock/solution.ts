export function maxProfit(prices: number[]): number {
  let minSoFar = Infinity;
  let best = 0;
  for (const p of prices) {
    minSoFar = Math.min(minSoFar, p);
    best = Math.max(best, p - minSoFar);
  }
  return best;
}

export function maxProfitUnlimited(prices: number[]): number {
  let total = 0;
  for (let i = 1; i < prices.length; i++) total += Math.max(0, prices[i] - prices[i - 1]);
  return total;
}
