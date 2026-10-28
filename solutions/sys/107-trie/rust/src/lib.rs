//! Reference trie.

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

fn idx(b: u8) -> usize {
    (b - b'a') as usize
}

impl Trie {
    pub fn new() -> Self {
        Trie { root: Box::default(), words: 0, nodes: 1 }
    }

    pub fn len(&self) -> usize {
        self.words
    }

    pub fn is_empty(&self) -> bool {
        self.words == 0
    }

    pub fn node_count(&self) -> usize {
        self.nodes
    }

    fn walk(&self, s: &str) -> Option<&Node> {
        let mut n = &*self.root;
        for b in s.bytes() {
            if !b.is_ascii_lowercase() {
                return None;
            }
            n = n.children[idx(b)].as_deref()?;
        }
        Some(n)
    }

    pub fn insert(&mut self, word: &str) -> bool {
        if !word.bytes().all(|b| b.is_ascii_lowercase()) {
            return false;
        }
        if self.contains(word) {
            return true;
        }
        let mut n = &mut *self.root;
        n.prefix_count += 1;
        for b in word.bytes() {
            let child = &mut n.children[idx(b)];
            if child.is_none() {
                self.nodes += 1;
            }
            n = child.get_or_insert_with(Box::default);
            n.prefix_count += 1;
        }
        n.end = true;
        self.words += 1;
        true
    }

    pub fn contains(&self, word: &str) -> bool {
        self.walk(word).is_some_and(|n| n.end)
    }

    pub fn count_prefix(&self, prefix: &str) -> usize {
        self.walk(prefix).map_or(0, |n| n.prefix_count as usize)
    }

    pub fn remove(&mut self, word: &str) -> bool {
        if !self.contains(word) {
            return false;
        }
        self.words -= 1;
        let mut n = &mut *self.root;
        n.prefix_count -= 1;
        for b in word.bytes() {
            let slot = &mut n.children[idx(b)];
            let child = slot.as_mut().unwrap();
            child.prefix_count -= 1;
            if child.prefix_count == 0 {
                // everything below is this word's private chain
                let mut chain = slot.take();
                while let Some(mut node) = chain {
                    self.nodes -= 1;
                    chain = node.children.iter_mut().find_map(|c| c.take());
                }
                return true;
            }
            n = slot.as_mut().unwrap();
        }
        n.end = false;
        true
    }

    pub fn words_with_prefix(&self, prefix: &str) -> Vec<String> {
        fn dfs(n: &Node, buf: &mut String, out: &mut Vec<String>) {
            if n.end {
                out.push(buf.clone());
            }
            for (i, c) in n.children.iter().enumerate() {
                if let Some(c) = c {
                    buf.push((b'a' + i as u8) as char);
                    dfs(c, buf, out);
                    buf.pop();
                }
            }
        }
        let mut out = Vec::new();
        if let Some(n) = self.walk(prefix) {
            dfs(n, &mut prefix.to_string(), &mut out);
        }
        out
    }
}

impl Default for Trie {
    fn default() -> Self {
        Self::new()
    }
}
