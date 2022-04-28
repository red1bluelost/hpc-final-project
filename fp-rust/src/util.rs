use std::ops::{Add, Div, Sub};

pub fn div_ru<T>(n: T, d: T) -> T
where
    T: Div<Output = T> + Add<Output = T> + Sub<Output = T> + Copy + From<u8>,
{
    (n + d - 1.into()) / d
}
