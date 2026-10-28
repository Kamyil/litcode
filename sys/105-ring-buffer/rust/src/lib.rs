//! Fixed-capacity FIFO of T over uninitialized storage. See ../README.md.
#![allow(dead_code, unused_variables)]

use std::mem::MaybeUninit;

pub struct RingBuffer<T> {
    buf: Box<[MaybeUninit<T>]>,
    head: usize, // total popped
    tail: usize, // total pushed
}

impl<T> RingBuffer<T> {
    /// Panics unless `cap` is a non-zero power of two.
    pub fn with_capacity(cap: usize) -> Self {
        todo!()
    }

    pub fn capacity(&self) -> usize {
        todo!()
    }

    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn is_full(&self) -> bool {
        todo!()
    }

    /// Returns the value back if the buffer is full.
    pub fn push(&mut self, value: T) -> Result<(), T> {
        todo!()
    }

    pub fn pop(&mut self) -> Option<T> {
        todo!()
    }

    pub fn peek(&self) -> Option<&T> {
        todo!()
    }

    /// Oldest to newest.
    pub fn iter(&self) -> impl Iterator<Item = &T> + '_ {
        // TODO
        std::iter::empty()
    }
}

impl<T> Drop for RingBuffer<T> {
    fn drop(&mut self) {
        // TODO: drop the live elements (and only those).
    }
}
