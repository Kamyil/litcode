import { expect, test } from "bun:test";
import { MinStack } from "./solution";

test("example", () => {
  const s = new MinStack();
  s.push(-2);
  s.push(0);
  s.push(-3);
  expect(s.getMin()).toBe(-3);
  s.pop();
  expect(s.top()).toBe(0);
  expect(s.getMin()).toBe(-2);
});

test("duplicate minimums", () => {
  const s = new MinStack();
  s.push(1);
  s.push(1);
  s.push(2);
  s.pop();
  s.pop();
  expect(s.getMin()).toBe(1);
});

test("against a naive model", () => {
  const s = new MinStack();
  const model: number[] = [];
  let seed = 7;
  const rnd = () => (seed = (seed * 1103515245 + 12345) % 2147483648);
  for (let i = 0; i < 2000; i++) {
    if (model.length && rnd() % 3 === 0) {
      s.pop();
      model.pop();
    } else {
      const x = (rnd() % 200) - 100;
      s.push(x);
      model.push(x);
    }
    if (model.length) {
      expect(s.top()).toBe(model[model.length - 1]);
      expect(s.getMin()).toBe(Math.min(...model));
    }
  }
});
