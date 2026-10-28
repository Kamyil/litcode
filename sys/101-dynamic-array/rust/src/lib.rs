//! MyVec<T>: a growable array built directly on the global allocator.
//! No `Vec` allowed inside — that's the point. See ../README.md.
#![allow(dead_code, unused_variables, unused_imports)]

use std::alloc::{self, Layout};
use std::ptr::{self, NonNull};

pub struct MyVec<T> {
    ptr: NonNull<T>, // NonNull::dangling() while cap == 0
    cap: usize,
    len: usize,
}

impl<T> MyVec<T> {
    pub fn new() -> Self {
        todo!()
    }

    pub fn with_capacity(cap: usize) -> Self {
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

    pub fn push(&mut self, value: T) {
        todo!()
    }

    pub fn pop(&mut self) -> Option<T> {
        todo!()
    }

    pub fn get(&self, i: usize) -> Option<&T> {
        todo!()
    }

    pub fn get_mut(&mut self, i: usize) -> Option<&mut T> {
        todo!()
    }

    /// Panics if `i > len`.
    pub fn insert(&mut self, i: usize, value: T) {
        todo!()
    }

    /// Panics if `i >= len`.
    pub fn remove(&mut self, i: usize) -> T {
        todo!()
    }

    pub fn as_slice(&self) -> &[T] {
        todo!()
    }
}

impl<T> Default for MyVec<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Drop for MyVec<T> {
    fn drop(&mut self) {
        // TODO: drop every live element, then free the buffer.
        // (Left empty instead of todo!() so a failing test doesn't abort the test binary.)
    }
}
