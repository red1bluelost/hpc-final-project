use fp_rust::matrix::*;

fn main() {
    let mut a = Matrix::new(3, 3);
    for i in 0..3 {
        a.set(i, i, 1.0);
    }
    let mut b = Matrix::new(3, 3);
    for i in 0..3 {
        for j in 0..3 {
            b.set(i, j, ((i + 1) * (j + 1)) as f64);
        }
    }
    let c = multiply::multiply(&a, &b);
}
