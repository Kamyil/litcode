# 401 · Quiz: Complexity & Big-O

> **Difficulty:** Easy · **Topics:** asymptotics, amortized analysis, recurrences, space · **Mode:** answer, then check with `lc solution 401`

Answer each question in one or two sentences, *out loud or in a notes file*, before looking.
Mark done with `lc done 401`.

## Warm-up: what's the complexity?

**1.** 
```ts
for (let i = 0; i < n; i++) for (let j = i; j < n; j++) work();
```

**2.**
```ts
for (let i = 1; i < n; i *= 2) for (let j = 0; j < i; j++) work();
```
(Careful — it's not O(n log n).)

**3.**
```ts
function f(n) { if (n <= 1) return 1; return f(n - 1) + f(n - 1); }
```

**4.**
```ts
function g(n) { if (n <= 1) return; g(n / 2); g(n / 2); for (let i = 0; i < n; i++) work(); }
```

**5.** `arr.includes(x)` inside a loop over `arr` (JS). And with `new Set(arr)` built once before the loop?

## Amortized analysis

**6.** A dynamic array doubles its capacity when full. Pushing n elements copies at most how many
elements in total? Why is `push` "amortized O(1)" even though one push can cost O(n)?

**7.** What if it grows by **+100** slots instead of ×2? Total copying cost for n pushes?

**8.** Why do real implementations often use a growth factor of 1.5 instead of 2? (Think about reusing freed memory.)

## Pick the right one

**9.** You need: insert, delete, and "give me the minimum" — all fast. Which structure, and what are the costs?

**10.** You need: membership test and **ordered** iteration / range queries. Hash set or balanced
tree (or B-tree)? Costs?

**11.** n = 10⁵. Which complexities are acceptable for ~1 second in a typical judge: O(n²), O(n log n), O(n√n), O(2ⁿ)?

**12.** Your O(n) algorithm is slower than a colleague's O(n log n) for n = 10⁶. Give three possible reasons.

## Space

**13.** What's the space complexity of recursive DFS on a tree with n nodes? Best case and worst case?

**14.** Merge sort: time and extra space? Quicksort (in place): time (avg/worst) and extra space?

**15.** Is O(log n) space "free"? When does recursion depth matter in practice (JS, Rust, C, Postgres)?

## Database flavour

**16.** A B-tree with fan-out 400 holds 10⁹ keys. How many levels? Why does that matter more than "O(log n)"?

**17.** A hash join of tables with m and n rows: time and memory? Nested loop with an index on the inner side?
