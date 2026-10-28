Helpers first: `align_up(n) = (n + 7) & ~7`, `header(page)`, `item(page, slot)`.
`page_free_space` = `upper - lower`.
---
"Free space after compaction" = `PAGE_SIZE - lower - sum(align_up(len) for live slots)`.
Compute it to decide whether compaction can help *before* touching anything.
---
Compaction, simple version: copy the page to a temporary buffer (8 KB on the stack is fine), set
`upper = PAGE_SIZE`, and for every live slot copy its bytes from the temp copy to the new `upper`.
Slot numbers don't change; only offsets do.
---
Insert: pick `slot` = lowest unused, or `nslots` (needs 4 more bytes of line pointers). `need = align_up(len) + (new slot ? 4 : 0)`.
If `upper - lower < need`, compact if that would be enough, else fail.
---
Update that grows: check feasibility excluding the old tuple's size; then mark the slot unused,
compact if needed (this drops the old bytes — that's why you check feasibility first), place the new
tuple, and point the slot at it. Copy from the caller's buffer, never from the page.
