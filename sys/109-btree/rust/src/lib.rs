//! In-memory B+tree with i64 keys/values; nodes live in an arena (Vec) and link by index.
//! See ../README.md.
#![allow(dead_code, unused_variables)]

type NodeId = usize;

// Suggested node representation — change it if you like.
enum Node {
    Leaf { keys: Vec<i64>, vals: Vec<i64>, next: Option<NodeId> },
    Internal { keys: Vec<i64>, kids: Vec<NodeId> },
}

pub struct BTree {
    nodes: Vec<Node>,
    root: NodeId,
    max_keys: usize,
    len: usize,
    height: usize,
}

impl BTree {
    /// Panics if `max_keys < 3`.
    pub fn new(max_keys: usize) -> Self {
        todo!()
    }

    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// 1 = the root is a leaf.
    pub fn height(&self) -> usize {
        todo!()
    }

    /// Returns the previous value.
    pub fn insert(&mut self, key: i64, value: i64) -> Option<i64> {
        todo!()
    }

    pub fn get(&self, key: i64) -> Option<i64> {
        todo!()
    }

    /// No rebalancing.
    pub fn remove(&mut self, key: i64) -> Option<i64> {
        todo!()
    }

    /// Inclusive range, ascending.
    pub fn range(&self, lo: i64, hi: i64) -> Vec<(i64, i64)> {
        todo!()
    }
}
