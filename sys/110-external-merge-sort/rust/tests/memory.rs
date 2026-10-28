//! Measures peak heap usage with a counting global allocator. Lives in its own test binary
//! (one test only) so no other test's allocations interfere.
mod common;

use common::Scratch;
use external_merge_sort::external_sort;
use std::alloc::{GlobalAlloc, Layout, System};
use std::sync::atomic::{AtomicUsize, Ordering::Relaxed};

struct Counting;
static LIVE: AtomicUsize = AtomicUsize::new(0);
static PEAK: AtomicUsize = AtomicUsize::new(0);

unsafe impl GlobalAlloc for Counting {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let p = unsafe { System.alloc(layout) };
        if !p.is_null() {
            let now = LIVE.fetch_add(layout.size(), Relaxed) + layout.size();
            PEAK.fetch_max(now, Relaxed);
        }
        p
    }
    unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
        unsafe { System.dealloc(ptr, layout) };
        LIVE.fetch_sub(layout.size(), Relaxed);
    }
}

#[global_allocator]
static ALLOC: Counting = Counting;

#[test]
fn bounded_memory() {
    let s = Scratch::new("memory");
    let (n, mem) = (300_000, 10_000);
    let sum = s.write_input(n, 0, 4);
    let base = LIVE.load(Relaxed);
    PEAK.store(base, Relaxed);
    let runs = external_sort(&s.input, &s.output, &s.runs, mem).unwrap();
    let peak = PEAK.load(Relaxed) - base;
    assert_eq!(runs, 30);
    s.check_output(n, sum);
    // the run buffer + one BufReader (8 KiB default) per run + slack
    let budget = mem * 8 + runs * 16 * 1024 + 64 * 1024;
    assert!(peak <= budget, "peak heap {peak} bytes > budget {budget}");
}
