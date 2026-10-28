import { Heap } from "../033-binary-heap/solution";

export function findKthLargest(nums: number[], k: number): number {
  return new KthLargest(k, nums).peek();
}

export class KthLargest {
  private heap = new Heap<number>((a, b) => a - b);

  constructor(
    private k: number,
    nums: number[],
  ) {
    for (const x of nums) this.push(x);
  }

  private push(x: number): void {
    this.heap.push(x);
    if (this.heap.size > this.k) this.heap.pop();
  }

  peek(): number {
    return this.heap.peek()!;
  }

  add(val: number): number {
    this.push(val);
    return this.peek();
  }
}
