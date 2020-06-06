use glfw::{Action, Context, Key};

mod gl;

const WIDTH: u32 = 1280;
const HEIGHT: u32 = 960;

struct Vertex {
    x: f32,
    y: f32,
    z: f32,
}

impl Vertex {
    fn new(x: f32, y: f32, z: f32) -> Vertex {
        Vertex { x, y, z }
    }
}

fn main() {
    let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();

    let (mut window, events) = glfw
        .create_window(
            WIDTH,
            HEIGHT,
            "float - playground",
            glfw::WindowMode::Windowed,
        )
        .expect("failed to create GLFW window");

    window.make_current();
    window.set_key_polling(true);

    gl::init(&glfw);
    gl::viewport(0, 0, WIDTH, HEIGHT);

    let vertices = vec![
        Vertex::new(0.0, 0.5, 0.0),
        Vertex::new(0.5, -0.5, 0.0),
        Vertex::new(-0.5, -0.5, 0.0),
    ];

    let vao = gl::create_vao();
    let vbo = gl::create_vbo(vao, vertices);

    while !window.should_close() {
        window.swap_buffers();

        glfw.poll_events();
        for (_, event) in glfw::flush_messages(&events) {
            match event {
                glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
                    window.set_should_close(true)
                }
                _ => {}
            }
        }
    }
}
