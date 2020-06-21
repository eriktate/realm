use crate::gm::{Quad, Rect, Vec2, Vec3};
use crate::sprite::{Show, Sprite};

pub struct Scene {
    quads: Vec<Quad>,
    sprites: Vec<Sprite>,
    pub sprite_index: Vec<usize>,
}

impl Scene {
    pub fn new(count: usize) -> Scene {
        Scene {
            quads: Vec::with_capacity(count),
            sprites: Vec::with_capacity(count),
            sprite_index: Vec::with_capacity(count),
        }
    }

    pub fn new_sprite(
        &mut self,
        pos: Vec3,
        width: u32,
        height: u32,
        hb: Rect,
        solid: bool,
        show: Show,
    ) -> u32 {
        let id = self.sprite_index.len() as u32;
        let spr = Sprite::new(id, pos, width, height, hb, solid, show);
        let quad = spr.to_quad();
        self.sprites.push(spr);
        self.quads.push(quad);
        self.sprite_index.push(id as usize);
        id
    }

    pub fn get_sprite(&self, id: u32) -> &Sprite {
        let idx = self.sprite_index[id as usize];
        &self.sprites[idx]
    }

    pub fn get_mut_sprite(&mut self, id: u32) -> &mut Sprite {
        let idx = self.sprite_index[id as usize];
        &mut self.sprites[idx]
    }

    pub fn move_sprite(&mut self, id: u32, move_vec: Vec3) {
        self.set_sprite_pos(id, self.get_sprite(id).pos + move_vec)
    }

    pub fn set_sprite_pos(&mut self, id: u32, pos: Vec3) {
        let idx = self.sprite_index[id as usize];
        self.sprites[idx].set_pos(pos);
    }

    pub fn set_quad(&mut self, id: u32, quad: Quad) {
        self.quads[id as usize] = quad;
    }

    pub fn quads(&self) -> &Vec<Quad> {
        &self.quads
    }

    pub fn sprites(&self) -> &Vec<Sprite> {
        &self.sprites
    }

    pub fn tick(&mut self, delta: f32) {
        self.y_order();
        self.z_order();
        for i in 0..self.sprites.len() {
            self.sprites[i].tick(delta);
            self.quads[i] = self.sprites[i].to_quad();
        }
    }

    pub fn y_order(&mut self) {
        for i in 1..self.sprites.len() {
            for j in 0..i {
                let idx = i - j;
                if self.sprites[idx].pos.y >= self.sprites[idx - 1].pos.y {
                    break;
                }

                // swap elements
                self.sprites.swap(idx, idx - 1);

                // update index
                self.sprite_index[self.sprites[idx - 1].id as usize] = idx - 1;
                self.sprite_index[self.sprites[idx].id as usize] = idx;
            }
        }
    }

    pub fn z_order(&mut self) {
        for i in 1..self.sprites.len() {
            for j in 0..i {
                let idx = i - j;
                if self.sprites[idx].pos.z >= self.sprites[idx - 1].pos.z {
                    break;
                }

                // swap elements
                self.sprites.swap(idx, idx - 1);

                // update index
                self.sprite_index[self.sprites[idx - 1].id as usize] = idx - 1;
                self.sprite_index[self.sprites[idx].id as usize] = idx;
            }
        }
    }
}
