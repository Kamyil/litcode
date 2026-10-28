export function subsets(nums: number[]): number[][] {
  const out: number[][] = [];
  for (let mask = 0; mask < 1 << nums.length; mask++) {
    out.push(nums.filter((_, i) => mask & (1 << i)));
  }
  return out;
}

export function subsetsWithDup(input: number[]): number[][] {
  const nums = [...input].sort((a, b) => a - b);
  const out: number[][] = [];
  const cur: number[] = [];
  const backtrack = (start: number) => {
    out.push([...cur]);
    for (let i = start; i < nums.length; i++) {
      if (i > start && nums[i] === nums[i - 1]) continue;
      cur.push(nums[i]);
      backtrack(i + 1);
      cur.pop();
    }
  };
  backtrack(0);
  return out;
}
