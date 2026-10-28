//! LRU cache: HashMap<K, index> + a doubly linked list stored in a Vec (indices, not pointers).
//! See ../README.md.
#![allow(dead_code, unused_variables, unused_imports)]

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
    head: usize, // most recently used, NIL if empty
    tail: usize, // least recently used, NIL if empty
}

impl<K: Hash + Eq + Clone, V> LruCache<K, V> {
    pub fn new(capacity: usize) -> Self {
        todo!()
    }

    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn get(&mut self, key: &K) -> Option<&V> {
        todo!()
    }

    /// Returns the evicted (key, value), if any.
    pub fn put(&mut self, key: K, value: V) -> Option<(K, V)> {
        todo!()
    }

    /// Keys from most to least recently used.
    pub fn keys_mru(&self) -> Vec<K> {
        todo!()
    }
}
