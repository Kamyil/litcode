# 401 · Answers

**1.** O(n²): n + (n−1) + ... + 1 = n(n+1)/2.

**2.** O(n): the inner loop runs 1 + 2 + 4 + ... + 2^k with 2^k < n, a geometric series < 2n.

**3.** O(2ⁿ): two calls per level, depth n. (Memoize identical calls and it becomes O(n).)

**4.** O(n log n): T(n) = 2T(n/2) + O(n), the merge-sort recurrence (Master theorem case 2).

**5.** `includes` is O(n), so the loop is O(n²). With a Set: O(n) to build + O(1) average per lookup = O(n) total.

**6.** Copies happen at sizes 1, 2, 4, ..., < n, which sums to < 2n. Total work O(n) for n pushes means O(1) per
push on average (amortized), even though the occasional push that triggers a resize costs O(n).

**7.** Copies at 100, 200, 300, ... n, which sums to ≈ n²/200 = O(n²). Additive growth destroys amortized O(1).

**8.** With factor 2, the new block is always bigger than the sum of all previous blocks, so the allocator can
never reuse the freed old blocks for the next growth. With φ ≈ 1.618 or 1.5 it eventually can. It also wastes less memory on average.

**9.** A binary heap: insert O(log n), peek-min O(1), pop-min O(log n). Deleting an arbitrary element needs its
index (an extra hash map from element to position), then O(log n). A balanced BST does all of it in O(log n) too.

**10.** Balanced tree / B-tree: O(log n) lookup, O(log n + k) range of k items, sorted iteration O(n).
A hash set gives O(1) average lookup but no order, so a range query would be O(n).

**11.** O(n log n) ≈ 1.7×10⁶ ops: fine. O(n√n) ≈ 3×10⁷: usually fine. O(n²) = 10¹⁰: too slow. O(2ⁿ): impossible.
(Rule of thumb: ~10⁸ simple operations per second.)

**12.** Constant factors (e.g. hashing cost vs comparisons), cache misses (pointer chasing vs sequential arrays),
memory allocation / GC pressure, branch misprediction, or the "O(n)" being O(n) only on average with bad
worst cases (hash collisions). Big-O hides all of these.

**13.** O(h), where h is the tree height: O(log n) for balanced trees, O(n) for a degenerate (linked-list) tree.

**14.** Merge sort: O(n log n) time always, O(n) extra space (arrays). Quicksort: O(n log n) average,
O(n²) worst case (bad pivots), O(log n) stack space if you recurse into the smaller half first.

**15.** Not free if it's stack: each frame costs memory, and stacks are small (JS ~10k–100k frames, Rust
main thread 8 MB / spawned threads 2 MB, C 8 MB by default). Deep recursion on adversarial input (degenerate
trees, long linked lists) crashes. Postgres guards recursion with `check_stack_depth()` / `max_stack_depth`.

**16.** log₄₀₀(10⁹) ≈ 3.5, so 4 levels. With the top levels cached, a lookup costs ~1 random disk read. "O(log n)"
hides that the base is huge. A binary tree would need ~30 levels, i.e. 30 random reads.

**17.** Hash join: O(m + n) time, memory O(min(m, n)) for the hash table (or spill to disk in batches).
Nested loop + index: O(m × log n) for m outer rows (each probe descends the B-tree). Great for small m,
terrible when m is large and the estimate was wrong.
