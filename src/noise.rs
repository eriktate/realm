use crate::gm::Vec2;
use rand::prelude::*;

const SEED: u64 = 31415;

const GRADIENTS: [Vec2; 4] = [
    Vec2 { x: 1.0, y: 1.0 },
    Vec2 { x: -1.0, y: 1.0 },
    Vec2 { x: 1.0, y: -1.0 },
    Vec2 { x: -1.0, y: -1.0 },
];

const PERMUTATIONS: [u32; 510] = [
    176, 207, 193, 129, 55, 155, 12, 166, 34, 226, 246, 242, 29, 67, 91, 153, 22, 160, 40, 107,
    253, 115, 254, 101, 84, 113, 180, 248, 252, 211, 6, 204, 122, 15, 62, 215, 88, 199, 30, 37, 93,
    132, 72, 202, 5, 89, 208, 81, 221, 23, 145, 143, 51, 206, 21, 158, 8, 151, 189, 141, 3, 110,
    33, 138, 25, 26, 96, 58, 9, 28, 42, 174, 104, 147, 60, 38, 236, 168, 117, 108, 66, 156, 175,
    134, 159, 75, 54, 4, 200, 18, 112, 11, 177, 41, 173, 79, 198, 212, 170, 241, 35, 161, 20, 39,
    73, 106, 186, 197, 114, 92, 196, 59, 222, 235, 250, 65, 48, 192, 24, 61, 17, 70, 43, 77, 181,
    167, 223, 14, 130, 237, 76, 0, 124, 157, 56, 190, 188, 249, 32, 10, 150, 205, 116, 78, 57, 127,
    227, 225, 154, 184, 172, 210, 103, 214, 64, 247, 152, 131, 224, 135, 179, 36, 82, 109, 183, 7,
    45, 46, 49, 148, 209, 123, 85, 232, 213, 68, 94, 63, 133, 219, 231, 244, 182, 238, 126, 86, 80,
    74, 95, 121, 13, 230, 87, 191, 217, 144, 194, 185, 201, 165, 1, 100, 142, 71, 187, 171, 52,
    218, 243, 149, 139, 118, 251, 229, 27, 98, 239, 19, 245, 140, 240, 228, 120, 83, 203, 128, 234,
    53, 136, 99, 44, 119, 47, 163, 164, 137, 125, 105, 216, 220, 162, 178, 233, 50, 195, 146, 31,
    90, 102, 69, 169, 2, 97, 111, 16, 176, 207, 193, 129, 55, 155, 12, 166, 34, 226, 246, 242, 29,
    67, 91, 153, 22, 160, 40, 107, 253, 115, 254, 101, 84, 113, 180, 248, 252, 211, 6, 204, 122,
    15, 62, 215, 88, 199, 30, 37, 93, 132, 72, 202, 5, 89, 208, 81, 221, 23, 145, 143, 51, 206, 21,
    158, 8, 151, 189, 141, 3, 110, 33, 138, 25, 26, 96, 58, 9, 28, 42, 174, 104, 147, 60, 38, 236,
    168, 117, 108, 66, 156, 175, 134, 159, 75, 54, 4, 200, 18, 112, 11, 177, 41, 173, 79, 198, 212,
    170, 241, 35, 161, 20, 39, 73, 106, 186, 197, 114, 92, 196, 59, 222, 235, 250, 65, 48, 192, 24,
    61, 17, 70, 43, 77, 181, 167, 223, 14, 130, 237, 76, 0, 124, 157, 56, 190, 188, 249, 32, 10,
    150, 205, 116, 78, 57, 127, 227, 225, 154, 184, 172, 210, 103, 214, 64, 247, 152, 131, 224,
    135, 179, 36, 82, 109, 183, 7, 45, 46, 49, 148, 209, 123, 85, 232, 213, 68, 94, 63, 133, 219,
    231, 244, 182, 238, 126, 86, 80, 74, 95, 121, 13, 230, 87, 191, 217, 144, 194, 185, 201, 165,
    1, 100, 142, 71, 187, 171, 52, 218, 243, 149, 139, 118, 251, 229, 27, 98, 239, 19, 245, 140,
    240, 228, 120, 83, 203, 128, 234, 53, 136, 99, 44, 119, 47, 163, 164, 137, 125, 105, 216, 220,
    162, 178, 233, 50, 195, 146, 31, 90, 102, 69, 169, 2, 97, 111, 16,
];

struct Perlin {
    width: u32,
    height: u32,
}

impl Perlin {
    pub fn new(width: u32, height: u32) -> Perlin {
        Perlin { width, height }
    }

    pub fn noise(&self, pos: &Vec2) -> f32 {
        // calculate lattice points
        let p0 = Vec2::new((pos.x as i32) as f32, (pos.y as i32) as f32);
        let p1 = p0 + Vec2::new(1.0, 0.0);
        let p2 = p0 + Vec2::new(0.0, 1.0);
        let p3 = p0 + Vec2::new(1.0, 1.0);

        // find point values within unit square
        let tx = pos.x - p0.x;
        let ty = pos.y - p0.y;

        let xi = p0.x as u32 & 0xff;
        let yi = p0.y as u32 & 0xff;

        // find hashes for each point
        let h0: u32 = PERMUTATIONS[(PERMUTATIONS[xi as usize] + yi) as usize];
        let h1: u32 = PERMUTATIONS[(PERMUTATIONS[xi as usize] + yi + 1) as usize];
        let h2: u32 = PERMUTATIONS[(PERMUTATIONS[(xi + 1) as usize] + yi) as usize];
        let h3: u32 = PERMUTATIONS[(PERMUTATIONS[(xi + 1) as usize] + yi + 1) as usize];

        let g0 = grad(h0, p0);
        let g1 = grad(h1, p1);
        let g2 = grad(h2, p2);
        let g3 = grad(h3, p3);

        let fade_x = fade(tx);
        let fade_y = fade(ty);
        // OLD
        // let xi = pos.x as i32 & 255;
        // let yi = pos.y as i32 & 255;

        // find point within unit cube
        // xf = pos.x - (pos.x as i32) as f32;
        // yf = pos.y - (pos.y as i32) as f32;

        // let u = fade(xf);
        // let v = fade(yf);

        // generate hashes for each point of unit square
        // let aa: i32 = PERMUTATIONS[PERMUTATIONS[xi] + yi];
        // let ab: i32 = PERMUTATIONS[PERMUTATIONS[xi] + yi + 1];
        // let ba: i32 = PERMUTATIONS[PERMUTATIONS[xi + 1] + yi];
        // let bb: i32 = PERMUTATIONS[PERMUTATIONS[xi + 1] + yi + 1];

        // let x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
        // let x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);
        // let y1 = lerp(grad(aa, xf, yf), grad(ab, xf, yf - 1), v);
        // let y2 = lerp(grad(ba, xf - 1, yf), grad(bb, xf - 1, yf - 1), v);
        1.0
    }
}

fn grad(hash: u32, p: Vec2) -> f32 {
    match hash & 0xF {
        0x0 => p.x + p.y,
        0x1 => -p.x + p.y,
        0x2 => p.x - p.y,
        0x3 => -p.x - p.y,
        0x4 => p.y + p.x,
        0x5 => -p.y + p.x,
        0x6 => p.y - p.x,
        0x7 => -p.y - p.x,
        0x8 => p.x + p.y,
        0x9 => -p.x + p.y,
        0xA => p.x - p.y,
        0xB => -p.x - p.y,
        0xC => p.y + p.x,
        0xD => -p.y + p.x,
        0xE => p.y - p.x,
        0xF => -p.y - p.x,
        _ => 0.0,
    }
}

fn fade(t: f32) -> f32 {
    t * t * t * (t * (t * 6.0 - 15.0) + 10.0)
}

fn lerp(a: f32, b: f32, x: f32) -> f32 {
    a + x * (b - a)
}

// brute force a hash table
fn make_hash_table() {
    let mut table: Vec<u32> = Vec::with_capacity(255);
    let mut counter = 0;
    while counter < 255 {
        let new_val = (random::<f32>() * 255.0) as u32;
        let mut exists = false;
        for val in &table {
            if *val == new_val {
                exists = true;
            }
        }

        if exists {
            continue;
        }

        table.push(new_val);
        counter += 1;
    }

    println!("{:?}", table);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_perlin() {
        let pos = Vec2::new(2.0, 3.0);
        let noise = perlin(&pos);
        println!("Noise: {}", noise);

        // two invocations on the same vec should be equal
        assert_eq!(noise, perlin(&pos));
    }

    #[test]
    fn test_make_hash_table() {
        make_hash_table();
    }
}
