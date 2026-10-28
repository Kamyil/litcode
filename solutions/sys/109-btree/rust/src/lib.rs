//! Reference B+tree on an index arena.

type NodeId = usize;

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

/// (old value if the key existed, (separator, new right node) if this node split)
type InsertResult = (Option<i64>, Option<(i64, NodeId)>);

fn upper_bound(keys: &[i64], k: i64) -> usize {
    keys.partition_point(|&x| x <= k)
}

impl BTree {
    pub fn new(max_keys: usize) -> Self {
        assert!(max_keys >= 3, "max_keys must be >= 3");
        let root = Node::Leaf { keys: Vec::new(), vals: Vec::new(), next: None };
        BTree { nodes: vec![root], root: 0, max_keys, len: 0, height: 1 }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn height(&self) -> usize {
        self.height
    }

    fn push(&mut self, n: Node) -> NodeId {
        self.nodes.push(n);
        self.nodes.len() - 1
    }

    fn find_leaf(&self, key: i64) -> NodeId {
        let mut id = self.root;
        while let Node::Internal { keys, kids } = &self.nodes[id] {
            id = kids[upper_bound(keys, key)];
        }
        id
    }

    fn insert_at(&mut self, id: NodeId, key: i64, value: i64) -> InsertResult {
        let max = self.max_keys;
        let child = match &mut self.nodes[id] {
            Node::Leaf { keys, vals, next } => {
                let i = match keys.binary_search(&key) {
                    Ok(i) => return (Some(std::mem::replace(&mut vals[i], value)), None),
                    Err(i) => i,
                };
                keys.insert(i, key);
                vals.insert(i, value);
                if keys.len() <= max {
                    return (None, None);
                }
                let mid = keys.len() / 2;
                let right = Node::Leaf { keys: keys.split_off(mid), vals: vals.split_off(mid), next: *next };
                let Node::Leaf { keys: rk, .. } = &right else { unreachable!() };
                let sep = rk[0];
                let rid = self.push(right);
                if let Node::Leaf { next, .. } = &mut self.nodes[id] {
                    *next = Some(rid);
                }
                return (None, Some((sep, rid)));
            }
            Node::Internal { keys, kids } => {
                let i = upper_bound(keys, key);
                (i, kids[i])
            }
        };
        let (i, child_id) = child;
        let (old, split) = self.insert_at(child_id, key, value);
        let Some((sep, rid)) = split else { return (old, None) };
        let Node::Internal { keys, kids } = &mut self.nodes[id] else { unreachable!() };
        keys.insert(i, sep);
        kids.insert(i + 1, rid);
        if keys.len() <= max {
            return (old, None);
        }
        let mid = keys.len() / 2;
        let right_keys = keys.split_off(mid + 1);
        let up = keys.pop().unwrap();
        let right_kids = kids.split_off(mid + 1);
        let new_id = self.push(Node::Internal { keys: right_keys, kids: right_kids });
        (old, Some((up, new_id)))
    }

    pub fn insert(&mut self, key: i64, value: i64) -> Option<i64> {
        let (old, split) = self.insert_at(self.root, key, value);
        if let Some((sep, rid)) = split {
            self.root = self.push(Node::Internal { keys: vec![sep], kids: vec![self.root, rid] });
            self.height += 1;
        }
        if old.is_none() {
            self.len += 1;
        }
        old
    }

    pub fn get(&self, key: i64) -> Option<i64> {
        let Node::Leaf { keys, vals, .. } = &self.nodes[self.find_leaf(key)] else { unreachable!() };
        keys.binary_search(&key).ok().map(|i| vals[i])
    }

    pub fn remove(&mut self, key: i64) -> Option<i64> {
        let id = self.find_leaf(key);
        let Node::Leaf { keys, vals, .. } = &mut self.nodes[id] else { unreachable!() };
        let i = keys.binary_search(&key).ok()?;
        keys.remove(i);
        self.len -= 1;
        Some(vals.remove(i))
    }

    pub fn range(&self, lo: i64, hi: i64) -> Vec<(i64, i64)> {
        let mut out = Vec::new();
        let mut cur = Some(self.find_leaf(lo));
        while let Some(id) = cur {
            let Node::Leaf { keys, vals, next } = &self.nodes[id] else { unreachable!() };
            for i in keys.partition_point(|&k| k < lo)..keys.len() {
                if keys[i] > hi {
                    return out;
                }
                out.push((keys[i], vals[i]));
            }
            cur = *next;
        }
        out
    }
}
