use crate::gm::{f32_eq, Vec2, Vec3};

#[repr(usize)]
#[derive(Copy, Clone)]
pub enum KeyState {
    Pressed,
    Released,
    Down,
    Up,
    End,
}

#[repr(usize)]
#[derive(Copy, Clone)]
pub enum KeyInput {
    Left,
    Right,
    Up,
    Down,
    End, // must be the last member of enum
}

pub struct Controller {
    values: [f32; KeyInput::End as usize],
    states: [KeyState; KeyInput::End as usize],
}

impl Controller {
    pub fn new() -> Controller {
        Controller {
            values: [0.0; KeyInput::End as usize],
            states: [KeyState::Up; KeyInput::End as usize],
        }
    }
    pub fn press(&mut self, input: KeyInput) {
        self.values[input as usize] = 1.0;
    }

    pub fn release(&mut self, input: KeyInput) {
        self.values[input as usize] = 0.0;
    }

    pub fn clear(&mut self) {
        for i in 0..self.values.len() {
            self.values[i] = 0.0;
        }
    }

    pub fn lock_in(&mut self) {
        for i in 0..self.values.len() {
            self.states[i] = if f32_eq(self.values[i], 0.0) {
                match self.states[i] {
                    KeyState::Pressed | KeyState::Down => KeyState::Released,
                    _ => KeyState::Up,
                }
            } else {
                match self.states[i] {
                    KeyState::Released | KeyState::Up => KeyState::Pressed,
                    _ => KeyState::Down,
                }
            }
        }
    }

    pub fn key_state(&self, key: KeyInput) -> KeyState {
        self.states[key as usize]
    }

    pub fn key_value(&self, key: KeyInput) -> f32 {
        self.values[key as usize]
    }

    pub fn move_vec(&self) -> Vec3 {
        let right = self.key_value(KeyInput::Right);
        let left = self.key_value(KeyInput::Left);
        let up = self.key_value(KeyInput::Up);
        let down = self.key_value(KeyInput::Down);

        Vec3::new(right - left, down - up, 0.0)
    }
}
