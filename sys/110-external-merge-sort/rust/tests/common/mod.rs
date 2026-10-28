// Shared helpers for the external sort tests (included with `mod common;`).
#![allow(dead_code)]

use std::fs;
use std::io::{BufReader, BufWriter, Read, Write};
use std::path::PathBuf;

pub struct Scratch {
    pub base: PathBuf,
    pub input: PathBuf,
    pub output: PathBuf,
    pub runs: PathBuf,
}

impl Scratch {
    pub fn new(name: &str) -> Self {
        let base = std::env::temp_dir().join(format!("lc-xsort-rs-{name}-{}", std::process::id()));
        let _ = fs::remove_dir_all(&base);
        let runs = base.join("runs");
        fs::create_dir_all(&runs).unwrap();
        Scratch { input: base.join("input.bin"), output: base.join("output.bin"), runs, base }
    }

    /// Writes n pseudo-random values; returns an order-independent checksum.
    pub fn write_input(&self, n: usize, modulo: i64, seed: u64) -> u64 {
        let mut w = BufWriter::new(fs::File::create(&self.input).unwrap());
        let mut s = seed;
        let mut sum = 0u64;
        for _ in 0..n {
            s ^= s << 13;
            s ^= s >> 7;
            s ^= s << 17;
            let x = if modulo > 0 { (s as i64).rem_euclid(modulo) } else { s as i64 };
            sum = sum.wrapping_add((x as u64).wrapping_mul(2654435761));
            w.write_all(&x.to_le_bytes()).unwrap();
        }
        sum
    }

    pub fn check_output(&self, n: usize, sum: u64) {
        let mut r = BufReader::new(fs::File::open(&self.output).expect("output file missing"));
        let (mut prev, mut count, mut s) = (i64::MIN, 0usize, 0u64);
        let mut b = [0u8; 8];
        while r.read_exact(&mut b).is_ok() {
            let x = i64::from_le_bytes(b);
            assert!(x >= prev, "output not sorted at index {count}");
            prev = x;
            s = s.wrapping_add((x as u64).wrapping_mul(2654435761));
            count += 1;
        }
        assert_eq!(count, n, "wrong number of values");
        assert_eq!(s, sum, "values changed");
    }

    pub fn leftover_runs(&self) -> usize {
        fs::read_dir(&self.runs).unwrap().count()
    }
}

impl Drop for Scratch {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.base);
    }
}
