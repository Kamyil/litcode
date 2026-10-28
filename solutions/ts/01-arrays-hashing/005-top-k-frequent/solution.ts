export function topKFrequent(nums: number[], k: number): number[] {
  const counts = new Map<number, number>();
  for (const x of nums) counts.set(x, (counts.get(x) ?? 0) + 1);

  const buckets: number[][] = Array.from({ length: nums.length + 1 }, () => []);
  for (const [x, c] of counts) buckets[c].push(x);

  const out: number[] = [];
  for (let f = buckets.length - 1; f > 0 && out.length < k; f--) {
    for (const x of buckets[f]) {
      out.push(x);
      if (out.length === k) break;
    }
  }
  return out;
}
