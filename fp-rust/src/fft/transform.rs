use super::Vector;
use crate::fft::Cmplx;
use crate::util;
use std::f64::consts::PI;

fn bit_rev(x: &mut Vec<Cmplx>) {
    let n = x.len();
    let mut r = 0;
    for i in 0..n - 1 {
        if i < r {
            x.swap(i, r);
        }
        let mut k = n / 2;
        while k <= r {
            r -= k;
            k /= 2;
        }
        r += k;
    }
}

pub fn fft_dit_r2(Vector(x): &mut Vector) {
    let n = x.len();
    let nu = util::log2(n);
    bit_rev(x);
    for m in 1..nu + 1 {
        let l = 1 << m;
        let l2 = l / 2;
        for ir in 0..l2 {
            let d = -2.0 * PI * ir as f64 / l as f64;
            let w = Cmplx::from_polar(1.0, d);
            for it in (ir..n).step_by(l) {
                let ib = it + l2;
                let t = x[ib] * w;
                x[ib] = x[it] - t;
                x[it] = x[it] + t;
            }
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    struct TU(Vector, Vector);

    fn fvi(v: &[f64]) -> Vector {
        Vector(v.iter().map(|&f| f.into()).collect())
    }

    fn cvi(v: &[(f64, f64)]) -> Vector {
        Vector(v.iter().map(|&(r, i)| Cmplx::new(r, i)).collect())
    }

    #[test]
    fn fft_dit_r2_test() {
        let tests: Vec<TU> = vec![
            TU(
                fvi(&[1., 2., 3., 4.]),
                cvi(&[(10., 0.), (-2., 2.), (-2., 0.), (-2., -2.)]),
            ),
            TU(
                fvi(&[1., 2., 3., 4., 5., 6., 7., 8.]),
                cvi(&[
                    (36., 0.),
                    (-4., 9.6569),
                    (-4., 4.),
                    (-4., 1.6569),
                    (-4., 0.),
                    (-4., -1.6569),
                    (-4., -4.),
                    (-4., -9.6569),
                ]),
            ),
            TU(
                fvi(&[1., 1., 1., 1., 0., 0., 0., 0.]),
                cvi(&[
                    (4., 0.),
                    (1., -2.4142),
                    (0., 0.),
                    (1., -0.4142),
                    (0., 0.),
                    (1., 0.4142),
                    (0., 0.),
                    (1., 2.4142),
                ]),
            ),
        ];
        for mut tt in tests {
            let TU(check, expect) = &mut tt;
            fft_dit_r2(check);
            let (Vector(check), Vector(expect)) = (check, expect);
            assert_eq!(check.len(), expect.len());
            for (&mut c, &mut e) in std::iter::zip(check, expect) {
                assert!((c.re - e.re).abs() <= 0.0001);
                assert!((c.im - e.im).abs() <= 0.0001);
            }
        }
    }
}
