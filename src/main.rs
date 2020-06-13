use glfw::{Action, Context, Key};

mod gl;
mod gm;
mod shader;
mod sprite;
mod texture;

use gm::{Vec2, Vec3, Vertex};
use sprite::{Show, Sprite};
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

    // do texture things
    let tex = Texture::load("spritesheet.png");
    let atlas = texture::Atlas::new(tex, 128, 192, 16, 32, 0, 0, 1, 4);
    let anim = sprite::Animation::new(&atlas, 4.0, vec![0, 1, 2, 3]);

    // make sprite
    let tex_quad = tex.tex_quad(128, 192, 16, 32);
    let mut spr = Sprite::new(0, Vec3::new(0.0, 0.0, 0.0), 16, 32, Show::Anim(anim));

    // load shaders
    let vert_src = include_str!("../shaders/sprite_vs.glsl");
    let frag_src = include_str!("../shaders/sprite_fs.glsl");
    let shader_program = shader::Shader::new(vert_src, frag_src).unwrap();
    shader_program.set_u32("tex", gl::Textures::Tex0 as u32);

    let mut quads = vec![spr.to_quad()];

    let indices = gm::make_indices(&quads);
    println!("{:?}", indices);
    let vao = gl::create_vao();
    let vbo = gl::create_vbo(&quads);
    let ebo = gl::create_ebo(&indices);

    gl::bind_vao(vao); // start recording vao
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
    gl::bind_vao(0); // stop recording vao
    gl::bind_vbo(0);
    gl::bind_ebo(0);

    // alpha in texture won't work without setting the blend mode
    gl::enable(gl::Capability::Blend);
    gl::blend_func(gl::BlendFactor::SrcAlpha, gl::BlendFactor::OneMinusSrcAlpha);

    let projection = gm::Mat4::ortho(0.0, (WIDTH / 2) as f32, 0.0, (HEIGHT / 2) as f32, 1.0, -1.0);
    shader_program.set_mat4("transform", projection);
    let mut time_last_frame = glfw.get_time() as f32;
    while !window.should_close() {
        glfw.poll_events();
        let current_time = glfw.get_time() as f32;
        let elapsed_time = current_time - time_last_frame;
        time_last_frame = current_time;

        spr.tick(elapsed_time);
        quads[0] = spr.to_quad();

        gl::update_vbo(vbo, &quads);
        for (_, event) in glfw::flush_messages(&events) {
            match event {
                glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
                    window.set_should_close(true)
                }
                _ => {}
            }
        }

        gl::clear_color(0.5, 0.8, 0.5, 1.0);
        gl::clear(gl::BufferBit::Color as u32);
        shader_program.use_program();
        gl::active_texture(0);
        gl::bind_texture(gl::TextureType::Tex2D, tex.id);
        gl::bind_vao(vao);
        gl::draw_elements(gl::DrawMode::Triangles, &indices);
        window.swap_buffers();
    }
}
