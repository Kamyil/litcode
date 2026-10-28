//! Reference LRU cache with an index-linked list.
use std::collections::HashMap;
use std::hash::Hash;

const NIL: usize = usize::MAX;

struct Node<K, V> {
    key: K,
    value: V,
    prev: usize,
    next: usize,
}

pub struct LruCache<K, V> {
    capacity: usize,
    map: HashMap<K, usize>,
    nodes: Vec<Node<K, V>>,
    head: usize,
    tail: usize,
}

impl<K: Hash + Eq + Clone, V> LruCache<K, V> {
    pub fn new(capacity: usize) -> Self {
        assert!(capacity > 0);
        LruCache { capacity, map: HashMap::with_capacity(capacity), nodes: Vec::with_capacity(capacity), head: NIL, tail: NIL }
    }

    pub fn len(&self) -> usize {
        self.map.len()
    }

    pub fn is_empty(&self) -> bool {
        self.map.is_empty()
    }

    fn unlink(&mut self, i: usize) {
        let (prev, next) = (self.nodes[i].prev, self.nodes[i].next);
        if prev == NIL { self.head = next } else { self.nodes[prev].next = next }
        if next == NIL { self.tail = prev } else { self.nodes[next].prev = prev }
    }

    fn push_front(&mut self, i: usize) {
        self.nodes[i].prev = NIL;
        self.nodes[i].next = self.head;
        if self.head != NIL {
            self.nodes[self.head].prev = i;
        }
        self.head = i;
        if self.tail == NIL {
            self.tail = i;
        }
    }

    pub fn get(&mut self, key: &K) -> Option<&V> {
        let i = *self.map.get(key)?;
        self.unlink(i);
        self.push_front(i);
        Some(&self.nodes[i].value)
    }

    pub fn put(&mut self, key: K, value: V) -> Option<(K, V)> {
        if let Some(&i) = self.map.get(&key) {
            self.nodes[i].value = value;
            self.unlink(i);
            self.push_front(i);
            return None;
        }
        let node = Node { key: key.clone(), value, prev: NIL, next: NIL };
        let (i, evicted) = if self.map.len() == self.capacity {
            let lru = self.tail;
            self.unlink(lru);
            let old = std::mem::replace(&mut self.nodes[lru], node);
            self.map.remove(&old.key);
            (lru, Some((old.key, old.value)))
        } else {
            self.nodes.push(node);
            (self.nodes.len() - 1, None)
        };
        self.map.insert(key, i);
        self.push_front(i);
        evicted
    }

    pub fn keys_mru(&self) -> Vec<K> {
        let mut out = Vec::with_capacity(self.len());
        let mut i = self.head;
        while i != NIL {
            out.push(self.nodes[i].key.clone());
            i = self.nodes[i].next;
        }
        out
    }
}
