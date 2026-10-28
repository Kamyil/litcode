import { Heap } from "../033-binary-heap/solution";

export class MedianFinder {
  private low = new Heap<number>((a, b) => b - a); // max-heap, lower half
  private high = new Heap<number>((a, b) => a - b); // min-heap, upper half

  addNum(x: number): void {
    this.low.push(x);
    this.high.push(this.low.pop()!);
    if (this.high.size > this.low.size) this.low.push(this.high.pop()!);
  }

  findMedian(): number {
    if (this.low.size > this.high.size) return this.low.peek()!;
    return (this.low.peek()! + this.high.peek()!) / 2;
  }
}
