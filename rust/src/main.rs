use glfw::{Action, Context, Key};

mod gl;
mod gm;
mod shader;
mod texture;

use gm::{Vec2, Vec3, Vertex};
use texture::Texture;

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

    // load texture
    let tex = Texture::load("../spritesheet.png");

    // load shaders
    let vert_src = include_str!("../vert_shader.glsl");
    let frag_src = include_str!("../frag_shader.glsl");
    let shader_program = shader::Shader::new(vert_src, frag_src).unwrap();
    shader_program.set_u32("tex", gl::Textures::Tex0 as u32);

    // init data
    // let vertices = vec![
    //     Vertex::new(Vec3::new(0.0, 0.5, 0.0), Vec2::new(0.5, 1.0)),
    //     Vertex::new(Vec3::new(0.5, -0.5, 0.0), Vec2::new(1.0, 0.0)),
    //     Vertex::new(Vec3::new(-0.5, -0.5, 0.0), Vec2::zero()),
    // ];
    let quads = vec![gm::Quad::new(
        gm::Vertex::new(Vec3::new(-0.5, 0.5, 0.0), Vec2::new(0.0, 1.0)),
        gm::Vertex::new(Vec3::new(0.5, 0.5, 0.0), Vec2::new(1.0, 1.0)),
        gm::Vertex::new(Vec3::new(-0.5, -0.5, 0.0), Vec2::new(0.0, 0.0)),
        gm::Vertex::new(Vec3::new(0.5, -0.5, 0.0), Vec2::new(1.0, 0.0)),
    )];

    let indices = gm::make_indices(&quads);
    let vao = gl::create_vao();
    let vbo = gl::create_vbo(&quads);
    let ebo = gl::create_ebo(&indices);

    gl::bind_vao(vao);
    gl::bind_vbo(vbo);
    gl::bind_ebo(ebo);
    gl::vertex_attrib_pointer(0, 3, false, std::mem::size_of::<Vertex>() as u32, 0);
    gl::vertex_attrib_pointer(
        1,
        2,
        false,
        std::mem::size_of::<Vertex>() as u32,
        std::mem::size_of::<Vec3>() as u32,
    );

    gl::enable_vertex_attrib_array(0);
    gl::enable_vertex_attrib_array(1);
    gl::bind_vao(0);
    gl::bind_vbo(0);
    gl::bind_ebo(0);

    // alpha in texture won't work without setting the blend mode
    gl::enable(gl::Capability::Blend);
    gl::blend_func(gl::BlendFactor::SrcAlpha, gl::BlendFactor::OneMinusSrcAlpha);

    while !window.should_close() {
        gl::clear_color(0.5, 0.8, 0.5, 1.0);
        gl::clear(gl::BufferBit::Color as u32);
        shader_program.use_program();
        gl::active_texture(0);
        gl::bind_texture(gl::TextureType::Tex2D, tex.id);
        gl::bind_vao(vao);
        gl::draw_elements(gl::DrawMode::Triangles, &indices);
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
