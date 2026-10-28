use hash_map::{fnv1a, HashMap};
use std::cell::Cell;
use std::rc::Rc;

#[test]
fn fnv_vectors() {
    assert_eq!(fnv1a(b""), 0xcbf29ce484222325);
    assert_eq!(fnv1a(b"a"), 0xaf63dc4c8601ec8c);
    assert_eq!(fnv1a(b"foobar"), 0x85944171f73967e8);
}

#[test]
fn basics() {
    let mut m = HashMap::new();
    assert_eq!(m.capacity(), 8);
    assert_eq!(m.insert("alice".to_string(), 1), None);
    assert_eq!(m.insert("bob".to_string(), 2), None);
    assert_eq!(m.insert("alice".to_string(), 10), Some(1));
    assert_eq!(m.get("alice"), Some(&10)); // &str lookup on String keys
    assert_eq!(m.get("carol"), None);
    *m.get_mut("bob").unwrap() += 5;
    assert_eq!(m.get("bob"), Some(&7));
    assert_eq!(m.len(), 2);
}

#[test]
fn remove_keeps_probe_chains() {
    let mut m = HashMap::new();
    for i in 0..5 {
        m.insert(i, i * 10);
    }
    assert_eq!(m.remove(&2), Some(20));
    assert_eq!(m.remove(&2), None);
    for i in 0..5 {
        assert_eq!(m.get(&i).is_some(), i != 2);
    }
    m.insert(2, 99);
    assert_eq!(m.get(&2), Some(&99));
}

#[test]
fn many_keys() {
    let mut m = HashMap::new();
    let n = 100_000;
    for i in 0..n {
        m.insert(format!("key-{i}"), i);
    }
    assert_eq!(m.len(), n);
    assert!(m.capacity() as f64 >= n as f64 / 0.7 && m.capacity() <= 4 * n);
    assert!(m.capacity().is_power_of_two());
    for i in 0..n {
        assert_eq!(m.get(format!("key-{i}").as_str()), Some(&i));
    }
    for i in (0..n).step_by(2) {
        m.remove(format!("key-{i}").as_str());
    }
    assert_eq!(m.len(), n / 2);
    assert_eq!(m.iter().count(), n / 2);
    assert!(m.iter().all(|(k, v)| k == &format!("key-{v}") && v % 2 == 1));
}

#[test]
fn tombstone_churn() {
    let mut m = HashMap::new();
    for i in 0..200_000 {
        m.insert(i, i);
        m.remove(&i);
    }
    assert_eq!(m.len(), 0);
    assert!(m.capacity() <= 64, "capacity grew to {}", m.capacity());
    assert_eq!(m.get(&-1), None);
}

struct DropCounter(Rc<Cell<usize>>);
impl Drop for DropCounter {
    fn drop(&mut self) {
        self.0.set(self.0.get() + 1);
    }
}

#[test]
fn values_are_dropped_correctly() {
    let drops = Rc::new(Cell::new(0));
    {
        let mut m = HashMap::new();
        for i in 0..100 {
            m.insert(i, DropCounter(drops.clone()));
        }
        assert_eq!(drops.get(), 0, "resizing must move values, not drop them");
        let old = m.insert(0, DropCounter(drops.clone()));
        assert_eq!(drops.get(), 0);
        drop(old);
        assert_eq!(drops.get(), 1);
        drop(m.remove(&1));
        assert_eq!(drops.get(), 2);
    }
    assert_eq!(drops.get(), 101);
}
