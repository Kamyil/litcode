//! Reference arena.
use std::alloc::{self, Layout};
use std::cell::{Cell, RefCell};
use std::ptr::NonNull;

const BLOCK_ALIGN: usize = 16;

pub struct Arena {
    block_size: usize,
    blocks: RefCell<Vec<(NonNull<u8>, Layout)>>, // last = current
    used: Cell<usize>,
}

impl Arena {
    pub fn new(block_size: usize) -> Self {
        Arena { block_size, blocks: RefCell::new(Vec::new()), used: Cell::new(0) }
    }

    fn alloc_raw(&self, layout: Layout) -> NonNull<u8> {
        let mut blocks = self.blocks.borrow_mut();
        if let Some(&(base, block)) = blocks.last() {
            let base = base.as_ptr() as usize;
            let start = (base + self.used.get() + layout.align() - 1) & !(layout.align() - 1);
            if start + layout.size() <= base + block.size() {
                self.used.set(start + layout.size() - base);
                return NonNull::new(start as *mut u8).unwrap();
            }
        }
        let size = self.block_size.max(layout.size() + layout.align()).max(1);
        let block = Layout::from_size_align(size, BLOCK_ALIGN).unwrap();
        let base = NonNull::new(unsafe { alloc::alloc(block) }).unwrap_or_else(|| alloc::handle_alloc_error(block));
        blocks.push((base, block));
        let addr = base.as_ptr() as usize;
        let start = (addr + layout.align() - 1) & !(layout.align() - 1);
        self.used.set(start + layout.size() - addr);
        NonNull::new(start as *mut u8).unwrap()
    }

    pub fn alloc<T: Copy>(&self, value: T) -> &mut T {
        let p = self.alloc_raw(Layout::new::<T>()).cast::<T>();
        unsafe {
            p.as_ptr().write(value);
            &mut *p.as_ptr()
        }
    }

    pub fn alloc_slice_copy<T: Copy>(&self, src: &[T]) -> &mut [T] {
        let p = self.alloc_raw(Layout::for_value(src)).cast::<T>();
        unsafe {
            std::ptr::copy_nonoverlapping(src.as_ptr(), p.as_ptr(), src.len());
            std::slice::from_raw_parts_mut(p.as_ptr(), src.len())
        }
    }

    pub fn alloc_str(&self, s: &str) -> &str {
        let bytes = self.alloc_slice_copy(s.as_bytes());
        unsafe { std::str::from_utf8_unchecked(bytes) }
    }

    pub fn reset(&mut self) {
        let blocks = self.blocks.get_mut();
        if let Some(current) = blocks.pop() {
            for (p, layout) in blocks.drain(..) {
                unsafe { alloc::dealloc(p.as_ptr(), layout) };
            }
            blocks.push(current);
        }
        self.used.set(0);
    }

    pub fn block_count(&self) -> usize {
        self.blocks.borrow().len()
    }
}

impl Drop for Arena {
    fn drop(&mut self) {
        for (p, layout) in self.blocks.get_mut().drain(..) {
            unsafe { alloc::dealloc(p.as_ptr(), layout) };
        }
    }
}
