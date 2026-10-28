export function trap(height: number[]): number {
  let l = 0;
  let r = height.length - 1;
  let leftMax = 0;
  let rightMax = 0;
  let water = 0;
  while (l < r) {
    leftMax = Math.max(leftMax, height[l]);
    rightMax = Math.max(rightMax, height[r]);
    if (leftMax < rightMax) water += leftMax - height[l++];
    else water += rightMax - height[r--];
  }
  return water;
}
