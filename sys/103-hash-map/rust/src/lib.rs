//! Open-addressing hash map with linear probing and tombstones. See ../README.md.
#![allow(dead_code, unused_variables, unused_imports)]

use std::borrow::Borrow;
use std::hash::{Hash, Hasher};

/// FNV-1a 64-bit over raw bytes.
pub fn fnv1a(bytes: &[u8]) -> u64 {
    todo!()
}

/// A `Hasher` that runs FNV-1a over whatever bytes `Hash` impls feed it.
#[derive(Default)]
pub struct FnvHasher {
    // TODO
}

impl Hasher for FnvHasher {
    fn write(&mut self, bytes: &[u8]) {
        todo!()
    }

    fn finish(&self) -> u64 {
        todo!()
    }
}

enum Slot<K, V> {
    Empty,
    Tombstone,
    Full(K, V),
}

pub struct HashMap<K, V> {
    slots: Vec<Slot<K, V>>,
    len: usize,
    tombstones: usize,
}

impl<K: Hash + Eq, V> HashMap<K, V> {
    /// Starts with capacity 8.
    pub fn new() -> Self {
        todo!()
    }

    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn capacity(&self) -> usize {
        todo!()
    }

    /// Returns the previous value for `key`, if any.
    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        todo!()
    }

    pub fn get<Q>(&self, key: &Q) -> Option<&V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        todo!()
    }

    pub fn get_mut<Q>(&mut self, key: &Q) -> Option<&mut V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        todo!()
    }

    pub fn remove<Q>(&mut self, key: &Q) -> Option<V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        todo!()
    }

    pub fn iter(&self) -> impl Iterator<Item = (&K, &V)> + '_ {
        // TODO
        std::iter::empty()
    }
}

impl<K: Hash + Eq, V> Default for HashMap<K, V> {
    fn default() -> Self {
        Self::new()
    }
}
