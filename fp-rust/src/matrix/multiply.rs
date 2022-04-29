use super::*;
use crate::util;
use crossbeam::thread;
use std::cmp::min;

pub fn multiply(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);
    for r in 0..a_row {
        for c in 0..b_col {
            let mut sum = 0.0;
            for k in 0..a_col {
                sum += a.at(r, k) * b.at(k, c);
            }
            c_mat.set(r, c, sum);
        }
    }
    c_mat
}

pub fn multiply_blocked(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);
    const BLOCK_SIZE: usize = 8;
    for ro in (0..a_row).step_by(BLOCK_SIZE) {
        for co in (0..b_col).step_by(BLOCK_SIZE) {
            for ko in (0..a_col).step_by(BLOCK_SIZE) {
                for r in ro..min(ro + BLOCK_SIZE, a_row) {
                    for c in co..min(co + BLOCK_SIZE, b_col) {
                        let mut sum = if ko == 0 { 0.0 } else { c_mat.at(r, c) };
                        for k in ko..min(ko + BLOCK_SIZE, a_col) {
                            sum += a.at(r, k) * b.at(k, c);
                        }
                        c_mat.set(r, c, sum);
                    }
                }
            }
        }
    }
    c_mat
}

pub fn multiply_parallel(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);
    let max_proc = std::thread::available_parallelism().unwrap().get();
    let work_width = util::div_ru(a_row, max_proc);

    thread::scope(|s| {
        let mut rest = c_mat.data.as_mut_slice();
        for start in (0..a_row).step_by(work_width) {
            let (slice, remain) = if start + work_width <= a_row {
                rest.split_at_mut(work_width * b_col)
            } else {
                (rest, [].as_mut_slice())
            };
            rest = remain;
            s.spawn(move |_| {
                for r in start..min(start + work_width, a_row) {
                    for c in 0..b_col {
                        let mut sum = 0.0;
                        for k in 0..a_col {
                            sum += a.at(r, k) * b.at(k, c);
                        }
                        slice[(r - start) * b_col + c] = sum;
                    }
                }
            });
        }
    })
    .unwrap();
    c_mat
}

pub fn multiply_fast(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);

    const BLOCK_SIZE: usize = 8;
    let max_proc = std::thread::available_parallelism().unwrap().get();
    let work_width = util::div_ru(util::div_ru(a_row, max_proc), BLOCK_SIZE) * BLOCK_SIZE;

    thread::scope(|s| {
        let mut rest = c_mat.data.as_mut_slice();
        for start in (0..a_row).step_by(work_width) {
            let (slice, remain) = if start + work_width <= a_row {
                rest.split_at_mut(work_width * b_col)
            } else {
                (rest, [].as_mut_slice())
            };
            rest = remain;
            s.spawn(move |_| {
                let idx = |r, c| (r - start) * b_col + c;
                for ro in (start..min(start + work_width, a_row)).step_by(BLOCK_SIZE) {
                    for co in (0..b_col).step_by(BLOCK_SIZE) {
                        for ko in (0..a_col).step_by(BLOCK_SIZE) {
                            for r in ro..min(ro + BLOCK_SIZE, a_row) {
                                let mut c = co;
                                while c < min(co + BLOCK_SIZE, b_col) {
                                    if c + 4 <= min(co + BLOCK_SIZE, b_col) {
                                        let mut sum = [0.0; 4];
                                        if ko != 0 {
                                            for i in 0..4 {
                                                sum[i] = slice[idx(r, c + i)];
                                            }
                                        }
                                        for k in ko..min(ko + BLOCK_SIZE, a_col) {
                                            for i in 0..4 {
                                                sum[i] += a.at(r, k) * b.at(k, c + i);
                                            }
                                        }
                                        for i in 0..4 {
                                            slice[idx(r, c + i)] = sum[i];
                                        }
                                        c += 4;
                                    } else {
                                        let mut sum = if ko == 0 { 0.0 } else { slice[idx(r, c)] };
                                        for k in ko..min(ko + BLOCK_SIZE, a_col) {
                                            sum += a.at(r, k) * b.at(k, c);
                                        }
                                        slice[idx(r, c)] = sum;
                                        c += 1;
                                    }
                                }
                            }
                        }
                    }
                }
            });
        }
    })
    .unwrap();
    c_mat
}

#[cfg(test)]
mod test {
    use super::*;
    use rand::{self, distributions::Standard, Rng};

    #[test]
    fn multiply_test() {
        for i in 1..101 {
            let a = Matrix::identity(i);
            let b = Matrix::from_vec(i, i, (0..).take(i * i).map(f64::from).collect());
            assert_eq!(b, multiply(&a, &b));
        }
    }

    type MultFunc = fn(&Matrix, &Matrix) -> Matrix;
    fn compare_multiplies(repeat: usize, mult_base: MultFunc, mult_test: MultFunc) {
        for _ in 0..repeat {
            let mut r = rand::thread_rng();
            let (m, k, n) = (
                r.gen_range(10..500),
                r.gen_range(10..500),
                r.gen_range(10..500),
            );
            let nums = r
                .sample_iter(Standard)
                .take(m * k + k * n)
                .collect::<Vec<f64>>();
            let (va, vb) = nums.split_at(m * k);
            let a = Matrix::from_vec(m, k, va.to_vec());
            let b = Matrix::from_vec(k, n, vb.to_vec());
            assert_eq!(mult_base(&a, &b), mult_test(&a, &b));
        }
    }

    #[test]
    fn multiply_blocked_test() {
        compare_multiplies(10, multiply, multiply_blocked);
    }

    #[test]
    fn multiply_parallel_test() {
        compare_multiplies(50, multiply_blocked, multiply_parallel);
    }

    #[test]
    fn multiply_fast_test() {
        compare_multiplies(50, multiply_blocked, multiply_fast);
    }
}
