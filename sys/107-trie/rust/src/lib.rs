//! 26-way trie with pruning removal. See ../README.md.
#![allow(dead_code, unused_variables)]

#[derive(Default)]
struct Node {
    children: [Option<Box<Node>>; 26],
    prefix_count: u32,
    end: bool,
}

pub struct Trie {
    root: Box<Node>,
    words: usize,
    nodes: usize,
}

impl Trie {
    pub fn new() -> Self {
        todo!()
    }

    /// Number of distinct words.
    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Allocated nodes, including the root.
    pub fn node_count(&self) -> usize {
        todo!()
    }

    /// False (and no change) if `word` contains anything outside a-z.
    pub fn insert(&mut self, word: &str) -> bool {
        todo!()
    }

    pub fn contains(&self, word: &str) -> bool {
        todo!()
    }

    pub fn count_prefix(&self, prefix: &str) -> usize {
        todo!()
    }

    /// Removes `word` and frees nodes that no longer lead anywhere. False if absent.
    pub fn remove(&mut self, word: &str) -> bool {
        todo!()
    }

    /// Lexicographic order.
    pub fn words_with_prefix(&self, prefix: &str) -> Vec<String> {
        todo!()
    }
}

impl Default for Trie {
    fn default() -> Self {
        Self::new()
    }
}
