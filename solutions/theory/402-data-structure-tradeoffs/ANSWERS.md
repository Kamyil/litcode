# 402 · Answers

**1.** Arrays are contiguous: iteration and search stream through cache lines, and the CPU prefetcher
loves them. Each list node is a separate allocation (malloc overhead, pointer per element) and
traversal is pointer chasing, a cache miss per node. Shifting n small elements with `memmove` is
extremely fast. You also have to *find* the insertion point, which is O(n) in a list anyway. Postgres
switched `List` to arrays (PG13) for faster access, fewer allocations and O(1) `list_nth`, and most
lists are short.

**2.** Open addressing: one flat array, cache-friendly, no per-entry allocation. Chaining: degrades
gracefully at high load factors, deletion is simple, and pointers to entries stay stable. With linear probing,
expected probe length grows like 1/(1−α)² for misses: clusters merge ("primary clustering") and
performance falls off a cliff near α = 1. Keep α ≲ 0.7–0.875.

**3.** A lookup stops at the first empty slot. If you empty a slot in the middle of a probe chain, keys
placed after it become unreachable. A tombstone says "keep probing, but you may reuse me for an insert."
Too many tombstones lengthen probes, so tables rehash periodically.

**4.** Hash flooding (HashDoS): crafted keys that all collide turn O(1) operations into O(n), so a
request with 100k keys costs O(n²) CPU. Defenses: keyed hash functions with a per-process random seed
(SipHash, used by Rust, Python, Ruby), and switching chains to trees (Java 8 HashMap). FNV is fast but
unkeyed and trivially attackable. It's fine for internal data, not for attacker-controlled keys.

**5.** Children `2i+1`, `2i+2`, parent `(i−1)/2`. Bottom-up heapify: half the nodes are leaves (0 work),
a quarter sift down at most 1 level, an eighth 2 levels, and so on. Σ n/2^(h+1) · h = O(n).

**6.** (a) A hash index can do ~1 disk read per lookup, but a B-tree is nearly as good (~1 uncached read
with 3–4 levels, upper levels cached), which is why Postgres hash indexes are rarely used. (b) B-tree: sorted
leaves + sibling links mean a range read is one descent plus sequential leaf reads. A BST is bad on disk:
~30 levels, one random read each, terrible fan-out.

**7.** Usually the hash set. A naive trie with 26 pointers per node uses far more memory. Only a trie
answers prefix queries efficiently (autocomplete). A radix (Patricia) tree compresses chains of
single-child nodes into one edge labeled with a string, which saves a lot of memory. Postgres SP-GiST
`text_ops` is a radix tree, and Linux uses radix trees for the page cache.

**8.** Simpler to implement than red-black/AVL trees, easy range iteration, and it's much easier to make
concurrent and lock-free (insertion only touches local pointers, no rotations). Redis uses skip lists for
sorted sets, and Java has `ConcurrentSkipListMap`.

**9.** A cheap pre-check that avoids expensive work for keys that are certainly absent, with no false
negatives and a tunable false-positive rate. LSM-tree databases (RocksDB, Cassandra) keep a Bloom filter per
SSTable so a point lookup skips files that can't contain the key. Postgres has a `bloom` index access method
for multi-column equality filters, and hash joins can use Bloom-like filters in some engines.

**10.** Sequential flooding: one big scan touches more pages than the cache holds, and strict LRU evicts the
entire hot working set for pages that will never be used again. LRU suffers most. Clock-sweep with
usage counts (plus Postgres's small ring buffers for big scans, VACUUM and COPY) resists it. LFU resists it
but adapts slowly when popularity shifts. Clock-sweep also avoids taking a global lock on every hit.

**11.** Amortized O(α(n)), the inverse Ackermann function, < 5 for any practical n. Uses: Kruskal's minimum
spanning tree, connected components / network connectivity, detecting cycles in undirected graphs,
grouping equal items (account merging), percolation.

**12.** A ring buffer (circular array), single-producer single-consumer with atomic head/tail indices. A
power-of-two capacity lets `index & (cap−1)` replace the expensive `%`, and with free-running counters
`tail − head` stays correct even after the integers wrap around.

**13.** Stack allocation is a pointer bump (adjusting the stack pointer), freed automatically on return,
always hot in cache. Heap allocation goes through an allocator (free lists, locking, fragmentation). A
local variable's storage is reused once the function returns, so the returned pointer dangles (undefined
behaviour; it may appear to work). Rust refuses to compile it: the reference's lifetime would outlive the
borrowed value ("returns a reference to data owned by the current function").

**14.** For static or read-mostly data: a sorted array is compact (no empty slots, no per-entry overhead), has
excellent locality, gives deterministic O(log n) worst case, supports range queries and ordered iteration, and is
trivially serializable (or mmappable). Updates are O(n), so it's bad for write-heavy data. Many databases
store immutable sorted runs exactly like this (SSTables).
