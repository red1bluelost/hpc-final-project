use std::ops::{Add, AddAssign, Div, Shl, Sub};

pub fn div_ru<T>(n: T, d: T) -> T
where
    T: Div<Output = T> + Add<Output = T> + Sub<Output = T> + Copy + From<u8>,
{
    (n + d - 1.into()) / d
}

pub fn log2<T>(i: T) -> T
where
    T: AddAssign + Shl<Output = T> + PartialOrd + Copy + From<u8>,
{
    let mut r = 0.into();
    while T::from(1) << r < i {
        r += 1.into();
    }
    r
}
