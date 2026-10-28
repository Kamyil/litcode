//! A Postgres-style slotted page, byte-compatible with the C version. See ../README.md.
#![allow(dead_code, unused_variables)]

pub const PAGE_SIZE: usize = 8192;
pub const HEADER_SIZE: usize = 8;
pub const ITEM_ID_SIZE: usize = 4;
pub const TUPLE_ALIGN: usize = 8;

pub struct Page {
    buf: Box<[u8; PAGE_SIZE]>,
}

impl Page {
    /// An initialized, empty page.
    pub fn new() -> Self {
        todo!()
    }

    pub fn as_bytes(&self) -> &[u8; PAGE_SIZE] {
        &self.buf
    }

    /// Contiguous free space: upper - lower.
    pub fn free_space(&self) -> usize {
        todo!()
    }

    pub fn num_slots(&self) -> u16 {
        todo!()
    }

    pub fn insert(&mut self, data: &[u8]) -> Option<u16> {
        todo!()
    }

    pub fn get(&self, slot: u16) -> Option<&[u8]> {
        todo!()
    }

    pub fn delete(&mut self, slot: u16) -> bool {
        todo!()
    }

    pub fn update(&mut self, slot: u16, data: &[u8]) -> bool {
        todo!()
    }

    pub fn compact(&mut self) {
        todo!()
    }
}

impl Default for Page {
    fn default() -> Self {
        Self::new()
    }
}
