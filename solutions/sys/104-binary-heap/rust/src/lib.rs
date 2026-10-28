//! Reference max-heap + heapsort.

pub struct BinaryHeap<T> {
    data: Vec<T>,
}

fn sift_down<T: Ord>(data: &mut [T], mut i: usize) {
    let n = data.len();
    loop {
        let (l, r) = (2 * i + 1, 2 * i + 2);
        let mut best = i;
        if l < n && data[l] > data[best] {
            best = l;
        }
        if r < n && data[r] > data[best] {
            best = r;
        }
        if best == i {
            return;
        }
        data.swap(i, best);
        i = best;
    }
}

impl<T: Ord> BinaryHeap<T> {
    pub fn new() -> Self {
        BinaryHeap { data: Vec::new() }
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub fn push(&mut self, value: T) {
        self.data.push(value);
        let mut i = self.data.len() - 1;
        while i > 0 {
            let p = (i - 1) / 2;
            if self.data[i] <= self.data[p] {
                break;
            }
            self.data.swap(i, p);
            i = p;
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.data.is_empty() {
            return None;
        }
        let top = self.data.swap_remove(0);
        sift_down(&mut self.data, 0);
        Some(top)
    }

    pub fn peek(&self) -> Option<&T> {
        self.data.first()
    }

    pub fn into_sorted_vec(self) -> Vec<T> {
        let mut v = self.data;
        heap_sort(&mut v);
        v
    }
}

impl<T: Ord> From<Vec<T>> for BinaryHeap<T> {
    fn from(mut data: Vec<T>) -> Self {
        for i in (0..data.len() / 2).rev() {
            sift_down(&mut data, i);
        }
        BinaryHeap { data }
    }
}

impl<T: Ord> Default for BinaryHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}

pub fn heap_sort<T: Ord>(data: &mut [T]) {
    for i in (0..data.len() / 2).rev() {
        sift_down(data, i);
    }
    for end in (1..data.len()).rev() {
        data.swap(0, end);
        sift_down(&mut data[..end], 0);
    }
}
