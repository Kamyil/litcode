//! Reference ring buffer.
use std::mem::MaybeUninit;

pub struct RingBuffer<T> {
    buf: Box<[MaybeUninit<T>]>,
    head: usize,
    tail: usize,
}

impl<T> RingBuffer<T> {
    pub fn with_capacity(cap: usize) -> Self {
        assert!(cap.is_power_of_two(), "capacity must be a power of two");
        RingBuffer { buf: (0..cap).map(|_| MaybeUninit::uninit()).collect(), head: 0, tail: 0 }
    }

    fn mask(&self) -> usize {
        self.buf.len() - 1
    }

    pub fn capacity(&self) -> usize {
        self.buf.len()
    }

    pub fn len(&self) -> usize {
        self.tail.wrapping_sub(self.head)
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn is_full(&self) -> bool {
        self.len() == self.capacity()
    }

    pub fn push(&mut self, value: T) -> Result<(), T> {
        if self.is_full() {
            return Err(value);
        }
        let i = self.tail & self.mask();
        self.buf[i].write(value);
        self.tail = self.tail.wrapping_add(1);
        Ok(())
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.is_empty() {
            return None;
        }
        let i = self.head & self.mask();
        self.head = self.head.wrapping_add(1);
        // SAFETY: slots in [head, tail) are initialized; we just moved head past this one.
        Some(unsafe { self.buf[i].assume_init_read() })
    }

    pub fn peek(&self) -> Option<&T> {
        if self.is_empty() {
            return None;
        }
        Some(unsafe { self.buf[self.head & self.mask()].assume_init_ref() })
    }

    pub fn iter(&self) -> impl Iterator<Item = &T> + '_ {
        (0..self.len()).map(move |k| unsafe { self.buf[self.head.wrapping_add(k) & self.mask()].assume_init_ref() })
    }
}

impl<T> Drop for RingBuffer<T> {
    fn drop(&mut self) {
        while self.pop().is_some() {}
    }
}
