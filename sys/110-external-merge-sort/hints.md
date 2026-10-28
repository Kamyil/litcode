Run generation: `n = fread(buf, sizeof(int64_t), mem_limit, in)` in a loop; `qsort` the chunk; write it
to `tmp_dir/run-<i>.bin`. When the loop ends, **free the buffer** before merging.
---
Merge: open every run file, allocate `k` heap entries `{ int64_t value; size_t run; }`, read the first value
of each run into the heap, then: pop min → write → read the next value from that run → push (if any).
---
A heap of k entries is plenty fast; sift-down after replacing the top (instead of pop + push) is the classic optimization.
---
Cleanup: `fclose` + `remove()` every run file, even on early error paths. Don't forget to free the
array of `FILE*`s and the heap.
---
Rust: `File::create(tmp_dir.join(format!("run-{i}.bin")))`, write with `w.write_all(&x.to_le_bytes())`.
Read one value: `let mut b = [0u8; 8]; r.read_exact(&mut b)` → `Err(e) if e.kind() == UnexpectedEof` means the run is done.
