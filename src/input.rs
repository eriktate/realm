use crate::gm::{f32_eq, Vec2};

pub enum State {
    Pressed,
    Released,
    Down,
    Up,
}

#[repr(usize)]
pub enum Input {
    Left,
    Right,
    Up,
    Down,
    End, // must be the last member of enum
}

pub struct Controller {
    pub pos: Vec2,
    pub inputs: [State; Input::End as usize],
}

impl Controller {
    pub fn press(&mut self, input: Input) {
        self.inputs[input as usize] = State::Pressed;
    }

    pub fn release(&mut self, input: Input) {
        self.inputs[input as usize] = State::Released;
    }

    pub fn clear(&mut self) {
        for i in 0..self.inputs.len() {
            self.inputs[i] = State::Up;
        }
    }

    pub fn lock_in(&mut self) {
        for i in 0..self.inputs.len() {
            self.inputs[i] = if f32_eq(self.inputs[i], 0.0) {
                match self.inputs[i] {
                    State::Pressed | State::Down => State::Released,
                    _ => State::Up,
                }
            } else {
                match self.inputs[i] {
                    State::Released | State::Up => State::Pressed,
                    _ => State::Down,
                }
            }
        }
    }
}
