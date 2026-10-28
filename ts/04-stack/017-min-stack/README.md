# 017 · Min Stack

> **Difficulty:** Medium · **Topics:** stack, design · **Target:** every operation O(1)

Design a stack that supports `push(x)`, `pop()`, `top()`, and `getMin()` — all in O(1).
`pop`, `top` and `getMin` are only called on a non-empty stack.

## Think about

- What extra information does each stack frame need so that popping restores the previous
  minimum?
- Space-optimized variant: a second stack that only stores *new* minimums. Careful with
  duplicates of the minimum.
