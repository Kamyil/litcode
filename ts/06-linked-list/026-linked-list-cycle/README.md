# 026 · Linked List Cycle (Floyd)

> **Difficulty:** Medium · **Topics:** fast & slow pointers · **Target:** O(n), O(1) space

- `hasCycle(head)`: does the list loop back on itself?
- `detectCycle(head)`: return the node where the cycle begins, or `null`.

Both in O(1) extra memory (a `Set` of visited nodes is the O(n) warm-up — do it first if you like).

## Think about

- Tortoise and hare: slow moves 1, fast moves 2. If there's a cycle they meet inside it.
- Why does resetting one pointer to `head` and moving both at speed 1 land on the cycle
  start? Derive it: if the tail before the cycle has length `a` and they met `b` steps into
  the cycle of length `c`, then `a ≡ c - b (mod c)`.
- Same trick finds the duplicate in an array of n+1 values in [1, n] (LeetCode 287).
