use btree::BTree;
use std::collections::BTreeMap;

fn xorshift(s: &mut u64) -> u64 {
    *s ^= *s << 13;
    *s ^= *s >> 7;
    *s ^= *s << 17;
    *s
}

#[test]
#[should_panic]
fn rejects_tiny_nodes() {
    BTree::new(2);
}

#[test]
fn small() {
    let mut t = BTree::new(4);
    assert_eq!(t.height(), 1);
    for k in [50, 20, 80, 10, 30, 70, 90, 60, 40] {
        assert_eq!(t.insert(k, k * 10), None);
    }
    assert_eq!(t.len(), 9);
    assert!(t.height() >= 2);
    assert_eq!(t.get(70), Some(700));
    assert_eq!(t.get(55), None);
    assert_eq!(t.insert(30, -1), Some(300));
    assert_eq!(t.len(), 9);
    assert_eq!(t.range(25, 65), vec![(30, -1), (40, 400), (50, 500), (60, 600)]);
}

#[test]
fn sequential_height_bounds() {
    let mut m = 3;
    while m <= 64 {
        let mut t = BTree::new(m);
        let n = 100_000;
        for i in 0..n {
            t.insert(i, i);
        }
        let min_fanout = ((m + 2) / 2) as f64;
        let max_h = 2 + ((n as f64).ln() / min_fanout.ln()).ceil() as usize;
        let min_h = ((n as f64 / m as f64).ln() / ((m + 1) as f64).ln()).ceil() as usize + 1;
        assert!(t.height() <= max_h && t.height() >= min_h, "m={m} height={}", t.height());
        assert!((0..n).step_by(7).all(|i| t.get(i) == Some(i)));
        m *= 2;
    }
}

#[test]
fn random_against_std_btreemap() {
    let mut t = BTree::new(5);
    let mut model = BTreeMap::new();
    let mut s = 88172645463325252u64;
    for _ in 0..30_000 {
        let k = (xorshift(&mut s) % 1_000_000) as i64 - 500_000;
        assert_eq!(t.insert(k, k * 2), model.insert(k, k * 2));
    }
    assert_eq!(t.len(), model.len());
    let all: Vec<_> = model.iter().map(|(&k, &v)| (k, v)).collect();
    assert_eq!(t.range(i64::MIN, i64::MAX), all);
    let window: Vec<_> = model.range(-1000..=1000).map(|(&k, &v)| (k, v)).collect();
    assert_eq!(t.range(-1000, 1000), window);
    assert!(t.range(10, 5).is_empty());
}

#[test]
fn remove_without_rebalancing() {
    let mut t = BTree::new(4);
    for i in 0..1000 {
        t.insert(i, i);
    }
    let h = t.height();
    for i in (0..1000).step_by(2) {
        assert_eq!(t.remove(i), Some(i));
    }
    assert_eq!(t.remove(0), None);
    assert_eq!(t.len(), 500);
    assert_eq!(t.height(), h);
    for i in (101..300).step_by(2) {
        t.remove(i);
    }
    let got: Vec<i64> = t.range(90, 320).into_iter().map(|(k, _)| k).collect();
    let want: Vec<i64> = (91..100).step_by(2).chain((301..320).step_by(2)).collect();
    assert_eq!(got, want);
    t.insert(200, 2);
    assert_eq!(t.get(200), Some(2));
}
