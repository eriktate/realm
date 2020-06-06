use glfw::{Action, Context, Key};

mod gl;
mod shader;

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
    glfw.window_hint(glfw::WindowHint::ContextVersion(3, 3));

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
        Vertex::new(0.0, 0.5, 0.0),
    ];

    let vao = gl::create_vao();
    let vbo = gl::create_vbo(vao, vertices);

    let vert_src = include_str!("../vert_shader.glsl");
    let frag_src = include_str!("../frag_shader.glsl");
    let shader_program = shader::Shader::new(vert_src, frag_src).unwrap();

    while !window.should_close() {
        gl::clear_color(0.5, 0.8, 0.5, 1.0);
        gl::clear(gl::BufferBit::Color as u32);
        gl::bind_vertex_array(vao);
        shader_program.use_program();
        gl::draw_arrays(gl::DrawMode::Triangles, 0, 1);

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
