export class MinStack {
  private values: number[] = [];
  private mins: number[] = [];

  push(x: number): void {
    this.values.push(x);
    this.mins.push(this.mins.length ? Math.min(x, this.mins[this.mins.length - 1]) : x);
  }

  pop(): void {
    this.values.pop();
    this.mins.pop();
  }

  top(): number {
    return this.values[this.values.length - 1];
  }

  getMin(): number {
    return this.mins[this.mins.length - 1];
  }
}
