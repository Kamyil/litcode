use binary_heap::{heap_sort, BinaryHeap};
use std::cmp::Reverse;

fn pseudo_random(n: usize, seed: u64) -> Vec<i64> {
    let mut s = seed;
    (0..n)
        .map(|_| {
            s = s.wrapping_mul(6364136223846793005).wrapping_add(1442695040888963407);
            ((s >> 33) % 100_000) as i64 - 50_000
        })
        .collect()
}

#[test]
fn max_heap() {
    let mut h = BinaryHeap::new();
    assert!(h.pop().is_none());
    for x in [5, 3, 8, 1, 9, 2] {
        h.push(x);
    }
    assert_eq!(h.peek(), Some(&9));
    assert_eq!(h.len(), 6);
    let out: Vec<_> = std::iter::from_fn(|| h.pop()).collect();
    assert_eq!(out, vec![9, 8, 5, 3, 2, 1]);
}

#[test]
fn min_heap_with_reverse() {
    let mut h = BinaryHeap::new();
    for (p, task) in [(3, "write tests"), (1, "fix prod"), (2, "review")] {
        h.push(Reverse((p, task)));
    }
    assert_eq!(h.pop().unwrap().0 .1, "fix prod");
    assert_eq!(h.pop().unwrap().0 .1, "review");
}

#[test]
fn from_vec_heapifies() {
    let xs = pseudo_random(20_000, 1);
    let mut h = BinaryHeap::from(xs.clone());
    let mut sorted = xs;
    sorted.sort_unstable_by(|a, b| b.cmp(a));
    let out: Vec<_> = std::iter::from_fn(|| h.pop()).collect();
    assert_eq!(out, sorted);
}

#[test]
fn into_sorted_vec() {
    let h = BinaryHeap::from(vec!["pear", "apple", "fig", "banana"]);
    assert_eq!(h.into_sorted_vec(), vec!["apple", "banana", "fig", "pear"]);
}

#[test]
fn heap_sort_matches_std() {
    for n in [0, 1, 2, 3, 10, 1000, 50_000] {
        let mut xs = pseudo_random(n, n as u64);
        let mut want = xs.clone();
        want.sort();
        heap_sort(&mut xs);
        assert_eq!(xs, want, "n = {n}");
    }
    let mut words = vec!["d".to_string(), "a".into(), "c".into(), "b".into()];
    heap_sort(&mut words);
    assert_eq!(words, ["a", "b", "c", "d"]);
}
