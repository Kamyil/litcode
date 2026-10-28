//! Reference slotted page.

pub const PAGE_SIZE: usize = 8192;
pub const HEADER_SIZE: usize = 8;
pub const ITEM_ID_SIZE: usize = 4;
pub const TUPLE_ALIGN: usize = 8;

pub struct Page {
    buf: Box<[u8; PAGE_SIZE]>,
}

fn align_up(n: usize) -> usize {
    (n + TUPLE_ALIGN - 1) & !(TUPLE_ALIGN - 1)
}

impl Page {
    pub fn new() -> Self {
        let mut p = Page { buf: Box::new([0; PAGE_SIZE]) };
        p.set_lower(HEADER_SIZE);
        p.set_upper(PAGE_SIZE);
        p
    }

    pub fn as_bytes(&self) -> &[u8; PAGE_SIZE] {
        &self.buf
    }

    fn read(&self, off: usize) -> usize {
        u16::from_le_bytes([self.buf[off], self.buf[off + 1]]) as usize
    }

    fn write(&mut self, off: usize, v: usize) {
        self.buf[off..off + 2].copy_from_slice(&(v as u16).to_le_bytes());
    }

    fn lower(&self) -> usize {
        self.read(0)
    }
    fn set_lower(&mut self, v: usize) {
        self.write(0, v)
    }
    fn upper(&self) -> usize {
        self.read(2)
    }
    fn set_upper(&mut self, v: usize) {
        self.write(2, v)
    }

    /// (offset, len) of a slot.
    fn item(&self, slot: usize) -> (usize, usize) {
        let base = HEADER_SIZE + slot * ITEM_ID_SIZE;
        (self.read(base), self.read(base + 2))
    }

    fn set_item(&mut self, slot: usize, offset: usize, len: usize) {
        let base = HEADER_SIZE + slot * ITEM_ID_SIZE;
        self.write(base, offset);
        self.write(base + 2, len);
    }

    pub fn free_space(&self) -> usize {
        self.upper() - self.lower()
    }

    pub fn num_slots(&self) -> u16 {
        self.read(4) as u16
    }

    fn reclaimable(&self, skip: Option<usize>) -> usize {
        let used: usize = (0..self.num_slots() as usize)
            .filter(|&i| Some(i) != skip)
            .map(|i| align_up(self.item(i).1))
            .sum();
        PAGE_SIZE - self.lower() - used
    }

    pub fn compact(&mut self) {
        let copy = *self.buf;
        let mut upper = PAGE_SIZE;
        for slot in 0..self.num_slots() as usize {
            let (off, len) = self.item(slot);
            if len == 0 {
                continue;
            }
            upper -= align_up(len);
            self.buf[upper..upper + len].copy_from_slice(&copy[off..off + len]);
            self.set_item(slot, upper, len);
        }
        self.set_upper(upper);
    }

    fn place(&mut self, data: &[u8]) -> usize {
        if self.free_space() < align_up(data.len()) {
            self.compact();
        }
        let upper = self.upper() - align_up(data.len());
        self.buf[upper..upper + data.len()].copy_from_slice(data);
        self.set_upper(upper);
        upper
    }

    pub fn insert(&mut self, data: &[u8]) -> Option<u16> {
        if data.is_empty() {
            return None;
        }
        let n = self.num_slots() as usize;
        let slot = (0..n).find(|&i| self.item(i).1 == 0).unwrap_or(n);
        let new_slot = slot == n;
        let need = align_up(data.len()) + if new_slot { ITEM_ID_SIZE } else { 0 };
        if self.reclaimable(None) < need {
            return None;
        }
        if new_slot {
            if self.free_space() < need {
                self.compact();
            }
            self.set_item(slot, 0, 0);
            self.write(4, n + 1);
            self.set_lower(self.lower() + ITEM_ID_SIZE);
        }
        let off = self.place(data);
        self.set_item(slot, off, data.len());
        Some(slot as u16)
    }

    pub fn get(&self, slot: u16) -> Option<&[u8]> {
        if slot >= self.num_slots() {
            return None;
        }
        match self.item(slot as usize) {
            (_, 0) => None,
            (off, len) => Some(&self.buf[off..off + len]),
        }
    }

    pub fn delete(&mut self, slot: u16) -> bool {
        if self.get(slot).is_none() {
            return false;
        }
        self.set_item(slot as usize, 0, 0);
        true
    }

    pub fn update(&mut self, slot: u16, data: &[u8]) -> bool {
        if data.is_empty() || self.get(slot).is_none() {
            return false;
        }
        let slot = slot as usize;
        let (off, len) = self.item(slot);
        if align_up(data.len()) <= align_up(len) {
            self.buf[off..off + data.len()].copy_from_slice(data);
            self.set_item(slot, off, data.len());
            return true;
        }
        if self.reclaimable(Some(slot)) < align_up(data.len()) {
            return false;
        }
        self.set_item(slot, 0, 0);
        let off = self.place(data);
        self.set_item(slot, off, data.len());
        true
    }
}

impl Default for Page {
    fn default() -> Self {
        Self::new()
    }
}
