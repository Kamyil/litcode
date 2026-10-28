//! Max-heap on a Vec<T>, plus in-place heapsort. See ../README.md.
#![allow(dead_code, unused_variables)]

pub struct BinaryHeap<T> {
    data: Vec<T>,
}

impl<T: Ord> BinaryHeap<T> {
    pub fn new() -> Self {
        todo!()
    }

    pub fn len(&self) -> usize {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn push(&mut self, value: T) {
        todo!()
    }

    /// Removes and returns the greatest element.
    pub fn pop(&mut self) -> Option<T> {
        todo!()
    }

    pub fn peek(&self) -> Option<&T> {
        todo!()
    }

    /// Ascending order.
    pub fn into_sorted_vec(self) -> Vec<T> {
        todo!()
    }
}

impl<T: Ord> From<Vec<T>> for BinaryHeap<T> {
    /// Heapify in O(n).
    fn from(data: Vec<T>) -> Self {
        todo!()
    }
}

impl<T: Ord> Default for BinaryHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}

/// In-place ascending heapsort.
pub fn heap_sort<T: Ord>(data: &mut [T]) {
    todo!()
}
