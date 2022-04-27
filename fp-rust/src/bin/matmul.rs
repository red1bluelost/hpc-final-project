use fp_rust::matrix::{multiply::*, *};
use std::fs::File;
use std::time::Instant;
use std::thread;

fn main() {
    let start = Instant::now();

    let a;
    let b;
    {
        let make_thr_func = |path| move || {
            let mut f = File::open(path).expect("failed to open file for matrix");
            Matrix::import(&mut f).expect("failed to import matrix A")
        };
        let ta = thread::spawn(make_thr_func("A.mat"));
        let tb = thread::spawn(make_thr_func("B.mat"));
        a = ta.join().expect("failed to join Matrix A thread");
        b = tb.join().expect("failed to join Matrix B thread");
    }
    let end_import = Instant::now();

    let c = multiply(&a, &b);
    let end_multiply = Instant::now();

    let mut f = File::create("C.mat").expect("failed to open file for matrix C");
    Matrix::export(&mut f, &c).expect("failed to export matrix C");
    let end_export = Instant::now();

    println!("-- Timing MatMul Rust --");
    for (s, d) in [
        ("Import", end_import - start),
        ("Multiply", end_multiply - end_import),
        ("Export", end_export - end_multiply),
        ("Total", end_export - start),
    ] {
        println!("{}: {} us", s, d.as_micros());
    }
}
