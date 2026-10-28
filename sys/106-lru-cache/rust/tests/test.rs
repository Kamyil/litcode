use lru_cache::LruCache;

#[test]
fn example() {
    let mut c = LruCache::new(2);
    assert_eq!(c.put(1, "one"), None);
    assert_eq!(c.put(2, "two"), None);
    assert_eq!(c.get(&1), Some(&"one"));
    assert_eq!(c.put(3, "three"), Some((2, "two")));
    assert_eq!(c.get(&2), None);
    assert_eq!(c.put(4, "four"), Some((1, "one")));
    assert_eq!(c.keys_mru(), vec![4, 3]);
    assert_eq!(c.len(), 2);
}

#[test]
fn update_refreshes() {
    let mut c = LruCache::new(2);
    c.put("a".to_string(), 1);
    c.put("b".to_string(), 2);
    assert_eq!(c.put("a".to_string(), 10), None);
    assert_eq!(c.keys_mru(), vec!["a", "b"]);
    assert_eq!(c.put("c".to_string(), 3), Some(("b".to_string(), 2)));
    assert_eq!(c.get(&"a".to_string()), Some(&10));
}

#[test]
fn storage_does_not_grow() {
    let mut c = LruCache::new(100);
    for i in 0..100_000 {
        c.put(i, i * 2);
    }
    assert_eq!(c.len(), 100);
    assert_eq!(c.get(&99_999), Some(&199_998));
    assert_eq!(c.get(&99_899), None);
    let keys = c.keys_mru();
    assert_eq!(keys[0], 99_999);
    assert_eq!(keys.len(), 100);
}

#[test]
fn against_model() {
    let cap = 16;
    let mut c = LruCache::new(cap);
    let mut model: Vec<(u32, u64)> = Vec::new(); // front = MRU
    let mut s: u64 = 99;
    for t in 0..50_000u64 {
        s = s.wrapping_mul(6364136223846793005).wrapping_add(1);
        let k = ((s >> 33) % 40) as u32;
        if (s >> 20) & 1 == 1 {
            let pos = model.iter().position(|e| e.0 == k);
            let want = pos.map(|p| model[p].1);
            assert_eq!(c.get(&k).copied(), want);
            if let Some(p) = pos {
                let e = model.remove(p);
                model.insert(0, e);
            }
        } else {
            let mut want_evicted = None;
            if let Some(p) = model.iter().position(|e| e.0 == k) {
                model.remove(p);
            } else if model.len() == cap {
                want_evicted = model.pop();
            }
            model.insert(0, (k, t));
            assert_eq!(c.put(k, t), want_evicted);
        }
    }
    assert_eq!(c.keys_mru(), model.iter().map(|e| e.0).collect::<Vec<_>>());
}
