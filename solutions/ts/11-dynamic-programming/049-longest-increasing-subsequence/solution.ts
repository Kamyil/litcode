function lowerBound(a: number[], x: number): number {
  let lo = 0;
  let hi = a.length;
  while (lo < hi) {
    const mid = (lo + hi) >>> 1;
    if (a[mid] < x) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

export function lengthOfLIS(nums: number[]): number {
  const tails: number[] = [];
  for (const x of nums) tails[lowerBound(tails, x)] = x;
  return tails.length;
}

export function lisSequence(nums: number[]): number[] {
  const tails: number[] = [];
  const tailIdx: number[] = [];
  const parent = new Array<number>(nums.length).fill(-1);
  nums.forEach((x, i) => {
    const k = lowerBound(tails, x);
    tails[k] = x;
    tailIdx[k] = i;
    parent[i] = k > 0 ? tailIdx[k - 1] : -1;
  });
  const out: number[] = [];
  for (let i = tailIdx[tails.length - 1] ?? -1; i !== -1; i = parent[i]) out.push(nums[i]);
  return out.reverse();
}
