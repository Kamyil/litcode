use dynamic_array::MyVec;
use std::cell::Cell;
use std::rc::Rc;

#[test]
fn empty() {
    let v: MyVec<i32> = MyVec::new();
    assert_eq!(v.len(), 0);
    assert_eq!(v.capacity(), 0);
    assert!(v.get(0).is_none());
    assert_eq!(v.as_slice(), &[] as &[i32]);
}

#[test]
fn push_get_pop() {
    let mut v = MyVec::new();
    for i in 0..100 {
        v.push(i);
    }
    assert_eq!(v.len(), 100);
    assert_eq!(v.get(42), Some(&42));
    assert_eq!(v.get(100), None);
    *v.get_mut(0).unwrap() = -1;
    assert_eq!(v.as_slice()[0], -1);
    assert_eq!(v.pop(), Some(99));
    assert_eq!(v.len(), 99);
}

#[test]
fn growth_doubles() {
    let mut v = MyVec::new();
    let mut caps = vec![];
    for i in 0..1000 {
        v.push(i);
        if caps.last() != Some(&v.capacity()) {
            caps.push(v.capacity());
        }
    }
    assert_eq!(caps, vec![4, 8, 16, 32, 64, 128, 256, 512, 1024]);
}

#[test]
fn with_capacity_preallocates() {
    let mut v = MyVec::with_capacity(10);
    assert_eq!(v.capacity(), 10);
    for i in 0..10 {
        v.push(i);
    }
    assert_eq!(v.capacity(), 10);
}

#[test]
fn insert_remove() {
    let mut v = MyVec::new();
    for x in [1, 2, 4, 5] {
        v.push(x);
    }
    v.insert(2, 3);
    v.insert(0, 0);
    v.insert(6, 6);
    assert_eq!(v.as_slice(), &[0, 1, 2, 3, 4, 5, 6]);
    assert_eq!(v.remove(0), 0);
    assert_eq!(v.remove(2), 3);
    assert_eq!(v.as_slice(), &[1, 2, 4, 5, 6]);
}

#[test]
#[should_panic]
fn insert_out_of_bounds_panics() {
    let mut v = MyVec::new();
    v.push(1);
    v.insert(5, 2);
}

#[test]
fn owns_heap_values() {
    let mut v = MyVec::new();
    for i in 0..20 {
        v.push(format!("s{i}"));
    }
    assert_eq!(v.remove(3), "s3");
    assert_eq!(v.pop().as_deref(), Some("s19"));
    assert_eq!(v.get(3).map(String::as_str), Some("s4"));
}

/// Increments a shared counter when dropped.
struct DropCounter(Rc<Cell<usize>>);
impl Drop for DropCounter {
    fn drop(&mut self) {
        self.0.set(self.0.get() + 1);
    }
}

#[test]
fn drops_every_element_exactly_once() {
    let drops = Rc::new(Cell::new(0));
    {
        let mut v = MyVec::new();
        for _ in 0..10 {
            v.push(DropCounter(drops.clone()));
        }
        let popped = v.pop();
        assert_eq!(drops.get(), 0, "pop must move the value out, not drop it");
        drop(popped);
        assert_eq!(drops.get(), 1);
        drop(v.remove(0));
        assert_eq!(drops.get(), 2);
    }
    assert_eq!(drops.get(), 10, "Drop for MyVec must drop the remaining 8");
}
