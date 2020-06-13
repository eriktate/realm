use crate::gm::{Quad, Rect, Vec2, Vec3, Vertex};
use crate::texture;

#[derive(Clone)]
pub struct Animation {
    // TODO (etate): Make this a reference
    atlas: texture::Atlas,
    framerate: f32,
    current_frame: f32,
    frames: Vec<texture::Quad>,
}

pub enum Show {
    Tex(texture::Quad),
    Anim(Animation),
}

pub struct Sprite {
    id: u32,
    pos: Vec3,
    width: u32,
    height: u32,
    hitbox: Rect,
    show: Show,
}

impl Sprite {
    pub fn new(id: u32, pos: Vec3, width: u32, height: u32, show: Show) -> Sprite {
        Sprite {
            id,
            pos,
            width,
            height,
            hitbox: Rect::new(pos, width as f32, height as f32),
            show,
        }
    }

    pub fn to_quad(&self) -> Quad {
        let tex_quad = self.tex_quad();

        Quad::new(
            Vertex::new(self.pos, tex_quad.tl),
            Vertex::new(
                Vec3::new(self.pos.x + self.width as f32, self.pos.y, self.pos.z),
                tex_quad.tr,
            ),
            Vertex::new(
                Vec3::new(self.pos.x, self.pos.y + self.height as f32, self.pos.z),
                tex_quad.bl,
            ),
            Vertex::new(
                Vec3::new(
                    self.pos.x + self.width as f32,
                    self.pos.y + self.height as f32,
                    self.pos.z,
                ),
                tex_quad.br,
            ),
        )
    }

    pub fn tex_quad(&self) -> texture::Quad {
        match &self.show {
            Show::Tex(tex) => tex.clone(),
            Show::Anim(anim) => anim.tex_quad(),
        }
    }

    pub fn set_pos(&mut self, pos: Vec3) {
        self.pos = pos;
    }

    pub fn tick(&mut self, delta: f32) {
        match &mut self.show {
            Show::Anim(anim) => anim.tick(delta),
            _ => (),
        }
    }
}

impl Animation {
    pub fn new(atlas: &texture::Atlas, framerate: f32, frames: Vec<usize>) -> Animation {
        Animation {
            atlas: atlas.clone(),
            framerate,
            current_frame: 0.0,
            frames: frames
                .iter()
                .copied()
                .map(|frame| atlas.get_frame_by_idx(frame))
                .collect(),
        }
    }

    pub fn tex_quad(&self) -> texture::Quad {
        self.frames[self.current_frame as usize]
    }

    pub fn tick(&mut self, delta: f32) {
        self.current_frame += self.framerate * (delta as f32);
        if self.current_frame > self.frames.len() as f32 {
            self.current_frame -= self.frames.len() as f32;
        }
    }
}
