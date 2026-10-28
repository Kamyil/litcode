//! A bump allocator: `alloc(&self)` hands out `&mut T` that live as long as the arena borrow.
//! See ../README.md.
#![allow(dead_code, unused_variables, unused_imports)]

use std::alloc::{self, Layout};
use std::cell::{Cell, RefCell};
use std::ptr::NonNull;

pub struct Arena {
    block_size: usize,
    // TODO: your fields. Suggestion:
    //   blocks: RefCell<Vec<(NonNull<u8>, Layout)>>,  // every block; the last one is current
    //   used: Cell<usize>,                             // bytes used in the current block
}

impl Arena {
    pub fn new(block_size: usize) -> Self {
        todo!()
    }

    /// Allocates and moves `value` into the arena.
    pub fn alloc<T: Copy>(&self, value: T) -> &mut T {
        todo!()
    }

    pub fn alloc_str(&self, s: &str) -> &str {
        todo!()
    }

    pub fn alloc_slice_copy<T: Copy>(&self, src: &[T]) -> &mut [T] {
        todo!()
    }

    /// Frees every block except the current one. `&mut self` proves no allocation is still borrowed.
    pub fn reset(&mut self) {
        todo!()
    }

    pub fn block_count(&self) -> usize {
        todo!()
    }
}

impl Drop for Arena {
    fn drop(&mut self) {
        // TODO: dealloc every block.
    }
}
