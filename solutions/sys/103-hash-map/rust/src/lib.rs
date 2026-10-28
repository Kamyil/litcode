//! Reference open-addressing hash map.
use std::borrow::Borrow;
use std::hash::{Hash, Hasher};

const OFFSET: u64 = 0xcbf29ce484222325;
const PRIME: u64 = 0x100000001b3;

pub fn fnv1a(bytes: &[u8]) -> u64 {
    let mut h = FnvHasher::default();
    h.write(bytes);
    h.finish()
}

pub struct FnvHasher(u64);

impl Default for FnvHasher {
    fn default() -> Self {
        FnvHasher(OFFSET)
    }
}

impl Hasher for FnvHasher {
    fn write(&mut self, bytes: &[u8]) {
        for &b in bytes {
            self.0 ^= b as u64;
            self.0 = self.0.wrapping_mul(PRIME);
        }
    }

    fn finish(&self) -> u64 {
        self.0
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

enum Probe {
    Found(usize),
    Vacant(usize),
}

fn empty_slots<K, V>(cap: usize) -> Vec<Slot<K, V>> {
    (0..cap).map(|_| Slot::Empty).collect()
}

fn hash_of<Q: Hash + ?Sized>(key: &Q) -> u64 {
    let mut h = FnvHasher::default();
    key.hash(&mut h);
    h.finish()
}

impl<K: Hash + Eq, V> HashMap<K, V> {
    pub fn new() -> Self {
        HashMap { slots: empty_slots(8), len: 0, tombstones: 0 }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn capacity(&self) -> usize {
        self.slots.len()
    }

    fn probe<Q>(&self, key: &Q) -> Probe
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        let mask = self.slots.len() - 1;
        let mut i = hash_of(key) as usize & mask;
        let mut tomb = None;
        loop {
            match &self.slots[i] {
                Slot::Empty => return Probe::Vacant(tomb.unwrap_or(i)),
                Slot::Tombstone => {
                    tomb.get_or_insert(i);
                }
                Slot::Full(k, _) if k.borrow() == key => return Probe::Found(i),
                Slot::Full(..) => {}
            }
            i = (i + 1) & mask;
        }
    }

    fn resize(&mut self, cap: usize) {
        let old = std::mem::replace(&mut self.slots, empty_slots(cap));
        self.len = 0;
        self.tombstones = 0;
        for slot in old {
            if let Slot::Full(k, v) = slot {
                if let Probe::Vacant(i) = self.probe(&k) {
                    self.slots[i] = Slot::Full(k, v);
                    self.len += 1;
                }
            }
        }
    }

    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        let cap = self.slots.len();
        if (self.len + self.tombstones + 1) * 10 > cap * 7 {
            let grow = (self.len + 1) * 10 > cap * 5;
            self.resize(if grow { cap * 2 } else { cap });
        }
        match self.probe(&key) {
            Probe::Found(i) => match &mut self.slots[i] {
                Slot::Full(_, v) => Some(std::mem::replace(v, value)),
                _ => unreachable!(),
            },
            Probe::Vacant(i) => {
                if let Slot::Tombstone = self.slots[i] {
                    self.tombstones -= 1;
                }
                self.slots[i] = Slot::Full(key, value);
                self.len += 1;
                None
            }
        }
    }

    pub fn get<Q>(&self, key: &Q) -> Option<&V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        match self.probe(key) {
            Probe::Found(i) => match &self.slots[i] {
                Slot::Full(_, v) => Some(v),
                _ => unreachable!(),
            },
            Probe::Vacant(_) => None,
        }
    }

    pub fn get_mut<Q>(&mut self, key: &Q) -> Option<&mut V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        match self.probe(key) {
            Probe::Found(i) => match &mut self.slots[i] {
                Slot::Full(_, v) => Some(v),
                _ => unreachable!(),
            },
            Probe::Vacant(_) => None,
        }
    }

    pub fn remove<Q>(&mut self, key: &Q) -> Option<V>
    where
        K: Borrow<Q>,
        Q: Hash + Eq + ?Sized,
    {
        let Probe::Found(i) = self.probe(key) else { return None };
        match std::mem::replace(&mut self.slots[i], Slot::Tombstone) {
            Slot::Full(_, v) => {
                self.len -= 1;
                self.tombstones += 1;
                Some(v)
            }
            _ => unreachable!(),
        }
    }

    pub fn iter(&self) -> impl Iterator<Item = (&K, &V)> + '_ {
        self.slots.iter().filter_map(|s| match s {
            Slot::Full(k, v) => Some((k, v)),
            _ => None,
        })
    }
}

impl<K: Hash + Eq, V> Default for HashMap<K, V> {
    fn default() -> Self {
        Self::new()
    }
}
