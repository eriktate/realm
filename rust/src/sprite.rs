use gm::{Quad, Rect, Vec3, Vec2};

pub struct TexQuad {
    tl: Vec2,
    tr: Vec2,
    bl: Vec2,
    br: Vec2,
}

impl TexQuad {
    pub fn new(tl: Vec2, tr: Vec2, bl: Vec2, br: Vec2) -> TexQuad {
        TexQuad{ tl, tr, bl ,br }
    }
}

impl From<&Rect> for TexQuad {
    fn from(rect: &Rect) -> TexQuad {
        TexQuad {
            tl: rect.pos,
            tr: Vec2::new(rect.pos.x + rect.w, rect.pos.y),
            bl: Vec2::new(rect.pos.x, rect.y + rect.h),
            br: Vec2::new(rect.pos.x + rect.w, rect.pos.y + rect.h),
        }
    }
}

pub struct Atlas {

}

pub struct Animation {
    id: u32
    framerate: f32,
    current_frame: f32,
    frames: Vec<TexQuad>,
}

pub struct Sprite {
    id: u32,
    pos: Vec3,
    width: u32,
    height: u32,
    hitbox: Rect,
    animated: bool,
    tex: TexQuad,
}

impl Sprite {
    pub fn make_quad(&self) -> Quad {}

    pub fn set_pos(&mut self, pos: Vec3) {
        self.pos = pos;
    }

    pub fn new(pos: Vec3, width: u32, height: u32) -> Sprite {
        Sprite{
            pos,
            width,
            height,
            hitbox: Rect::new(pos, width, height),
        }
    }
}
