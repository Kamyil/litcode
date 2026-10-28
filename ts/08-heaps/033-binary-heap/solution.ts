export class Heap<T> {
  constructor(private cmp: (a: T, b: T) => number) {}

  static from<T>(items: T[], cmp: (a: T, b: T) => number): Heap<T> {
    throw new Error("not implemented");
  }

  get size(): number {
    throw new Error("not implemented");
  }

  peek(): T | undefined {
    throw new Error("not implemented");
  }

  push(x: T): void {
    throw new Error("not implemented");
  }

  pop(): T | undefined {
    throw new Error("not implemented");
  }
}
