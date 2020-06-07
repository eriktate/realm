use glfw::{Action, Context, Key};

mod gl;
mod gm;
mod shader;

use gm::{Vec2, Vec3, Vertex};

const WIDTH: u32 = 1280;
const HEIGHT: u32 = 960;

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

    // load shaders
    let vert_src = include_str!("../vert_shader.glsl");
    let frag_src = include_str!("../frag_shader.glsl");
    let shader_program = shader::Shader::new(vert_src, frag_src).unwrap();

    // init data
    let vertices = vec![
        Vertex::new(Vec3::new(0.0, 0.5, 1.0), Vec2::zero()),
        Vertex::new(Vec3::new(0.5, -0.5, 0.0), Vec2::zero()),
        Vertex::new(Vec3::new(-0.5, -0.5, 0.0), Vec2::zero()),
    ];

    let vao = gl::create_vao();
    let vbo = gl::create_vbo(&vertices);

    gl::bind_vertex_array(vao);
    gl::bind_array_buffer(vbo);
    gl::vertex_attrib_pointer(0, 3, false, std::mem::size_of::<Vertex>() as u32, 0);

    gl::enable_vertex_attrib_array(0);
    gl::bind_array_buffer(0);

    gl::clear_color(0.5, 0.8, 0.5, 1.0);
    gl::clear(gl::BufferBit::Color as u32);
    shader_program.use_program();
    gl::bind_vertex_array(vao);
    gl::draw_arrays(gl::DrawMode::Triangles, 0, 3);
    window.swap_buffers();

    while !window.should_close() {
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
