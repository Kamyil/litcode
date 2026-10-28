//! Reference MyVec<T>.
use std::alloc::{self, Layout};
use std::ptr::{self, NonNull};

pub struct MyVec<T> {
    ptr: NonNull<T>,
    cap: usize,
    len: usize,
}

impl<T> MyVec<T> {
    pub fn new() -> Self {
        assert!(std::mem::size_of::<T>() != 0, "zero-sized types not supported");
        MyVec { ptr: NonNull::dangling(), cap: 0, len: 0 }
    }

    pub fn with_capacity(cap: usize) -> Self {
        let mut v = Self::new();
        if cap > 0 {
            v.grow_to(cap);
        }
        v
    }

    fn grow_to(&mut self, new_cap: usize) {
        let new_layout = Layout::array::<T>(new_cap).expect("capacity overflow");
        let raw = if self.cap == 0 {
            unsafe { alloc::alloc(new_layout) }
        } else {
            let old_layout = Layout::array::<T>(self.cap).unwrap();
            unsafe { alloc::realloc(self.ptr.as_ptr().cast(), old_layout, new_layout.size()) }
        };
        self.ptr = match NonNull::new(raw.cast::<T>()) {
            Some(p) => p,
            None => alloc::handle_alloc_error(new_layout),
        };
        self.cap = new_cap;
    }

    fn reserve_one(&mut self) {
        if self.len == self.cap {
            self.grow_to(if self.cap == 0 { 4 } else { self.cap * 2 });
        }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn capacity(&self) -> usize {
        self.cap
    }

    pub fn push(&mut self, value: T) {
        self.reserve_one();
        unsafe { self.ptr.as_ptr().add(self.len).write(value) };
        self.len += 1;
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.len == 0 {
            return None;
        }
        self.len -= 1;
        Some(unsafe { self.ptr.as_ptr().add(self.len).read() })
    }

    pub fn get(&self, i: usize) -> Option<&T> {
        self.as_slice().get(i)
    }

    pub fn get_mut(&mut self, i: usize) -> Option<&mut T> {
        if i < self.len {
            Some(unsafe { &mut *self.ptr.as_ptr().add(i) })
        } else {
            None
        }
    }

    pub fn insert(&mut self, i: usize, value: T) {
        assert!(i <= self.len, "insert index out of bounds");
        self.reserve_one();
        unsafe {
            let p = self.ptr.as_ptr().add(i);
            ptr::copy(p, p.add(1), self.len - i);
            p.write(value);
        }
        self.len += 1;
    }

    pub fn remove(&mut self, i: usize) -> T {
        assert!(i < self.len, "remove index out of bounds");
        unsafe {
            self.len -= 1;
            let p = self.ptr.as_ptr().add(i);
            let value = p.read();
            ptr::copy(p.add(1), p, self.len - i);
            value
        }
    }

    pub fn as_slice(&self) -> &[T] {
        unsafe { std::slice::from_raw_parts(self.ptr.as_ptr(), self.len) }
    }
}

impl<T> Default for MyVec<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Drop for MyVec<T> {
    fn drop(&mut self) {
        unsafe {
            ptr::drop_in_place(ptr::slice_from_raw_parts_mut(self.ptr.as_ptr(), self.len));
            if self.cap > 0 {
                alloc::dealloc(self.ptr.as_ptr().cast(), Layout::array::<T>(self.cap).unwrap());
            }
        }
    }
}
