# 402 · Quiz: Data Structure Trade-offs

> **Difficulty:** Medium · **Topics:** arrays vs lists, hashing, trees, heaps, tries, caches · **Mode:** `lc solution 402`

**1.** Linked list insertion at a known node is O(1), array insertion is O(n). Why do arrays usually
win anyway, even for insert-heavy workloads of moderate size? (Postgres changed its `List` from a
linked list to an array in v13 — why?)

**2.** Open addressing vs separate chaining: name one advantage of each. What happens to linear
probing as the load factor approaches 1?

**3.** Why does deletion in an open-addressing table need **tombstones**? What goes wrong if you
just mark the slot empty?

**4.** An attacker can choose the keys your server hashes (e.g. JSON object keys). What attack is this,
and how do languages defend against it? (Rust's default `HashMap` uses SipHash with a random seed — why not FNV?)

**5.** A binary heap is stored in an array. Given index i (0-based), where are its children and parent?
Why is building a heap from n items O(n), not O(n log n)?

**6.** BST vs B-tree vs hash table for an on-disk index of 1 billion keys. Pick one for (a) point lookups only,
(b) range queries and ORDER BY. Explain in terms of disk reads.

**7.** Trie vs hash set for storing 1M words. Which uses less memory? Which supports prefix queries?
What's a radix tree?

**8.** A skip list gives O(log n) expected search. Why would anyone use it instead of a balanced tree?
(Hint: Redis sorted sets; lock-free concurrency.)

**9.** A Bloom filter says "definitely not present" or "probably present". What's it good for? Where
might a database use one? (Look up the `bloom` extension and how LSM trees use them.)

**10.** LRU vs clock-sweep (Postgres buffer manager) vs LFU. What's "sequential flooding", and which
policy suffers from it?

**11.** Union-Find with path compression + union by rank: what's the cost per operation and what's
it used for (two examples)?

**12.** You need a queue between a producer and a consumer thread with fixed memory. Which structure?
Why must its capacity often be a power of two?

**13.** Stack vs heap allocation: why is stack allocation faster, and why can't you return a pointer to
a local variable in C? What does Rust do if you try?

**14.** When would you choose a sorted array + binary search over a hash map? (Think: read-mostly
data, memory, cache, range queries.)
