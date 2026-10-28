export function permute(nums: number[]): number[][] {
  const out: number[][] = [];
  const a = [...nums];
  const go = (k: number) => {
    if (k === a.length) {
      out.push([...a]);
      return;
    }
    for (let i = k; i < a.length; i++) {
      [a[k], a[i]] = [a[i], a[k]];
      go(k + 1);
      [a[k], a[i]] = [a[i], a[k]];
    }
  };
  go(0);
  return out;
}

export function permuteUnique(input: number[]): number[][] {
  const nums = [...input].sort((a, b) => a - b);
  const used = new Array<boolean>(nums.length).fill(false);
  const out: number[][] = [];
  const cur: number[] = [];
  const go = () => {
    if (cur.length === nums.length) {
      out.push([...cur]);
      return;
    }
    for (let i = 0; i < nums.length; i++) {
      if (used[i] || (i > 0 && nums[i] === nums[i - 1] && !used[i - 1])) continue;
      used[i] = true;
      cur.push(nums[i]);
      go();
      cur.pop();
      used[i] = false;
    }
  };
  go();
  return out;
}

function reverse(a: number[], l: number, r: number): void {
  while (l < r) {
    [a[l], a[r]] = [a[r], a[l]];
    l++;
    r--;
  }
}

export function nextPermutation(a: number[]): void {
  let i = a.length - 2;
  while (i >= 0 && a[i] >= a[i + 1]) i--;
  if (i >= 0) {
    let j = a.length - 1;
    while (a[j] <= a[i]) j--;
    [a[i], a[j]] = [a[j], a[i]];
  }
  reverse(a, i + 1, a.length - 1);
}
