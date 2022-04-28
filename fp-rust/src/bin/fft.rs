use fp_rust::fft::{transform::*, *};
use std::fs::File;
use std::time::Instant;

fn main() {
    let start = Instant::now();

    let mut f = File::open("X.vec").expect("failed to open file for vector X");
    let mut x = Vector::import(&mut f).expect("failed to import vector");

    let end_import = Instant::now();

    fft_dit_r2(&mut x);
    let end_fft = Instant::now();

    let mut f = File::create("Xf.vec").expect("failed to open file for vector Xf");
    x.export(&mut f).expect("failed to export vector");
    let end_export = Instant::now();

    println!("-- Timing FFT Rust --");
    for (s, d) in [
        ("Import", end_import - start),
        ("FFT", end_fft - end_import),
        ("Export", end_export - end_fft),
        ("Total", end_export - start),
    ] {
        println!("{}: {} us", s, d.as_micros());
    }
}
