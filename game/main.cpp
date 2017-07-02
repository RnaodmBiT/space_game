#include <cstdio>
#include <SDL.h>
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <chaiscript/chaiscript.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <maths.hpp>
#include <rendering.hpp>
#include <io.hpp>
#include <nodes/camera.hpp>

#define WINDOW_TITLE        "Space"

using namespace chaiscript;



struct mesh {
    struct part {
        std::vector<gl::buffer> buffers;
        gl::array object;
        int vertices;


        vec3 diffuse;
    };

    std::vector<part> parts;

    mesh() { }

    mesh(mesh&& move) {
        std::swap(parts, move.parts);
    }

    void operator=(mesh&& move) {
        std::swap(parts, move.parts);
    }

    void draw(gl::uniform& diffuse) const {
        for (const part& p : parts) {
            diffuse.set(p.diffuse);

            p.object.draw(p.vertices);
        }
    }
};

mesh load_mesh(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_ImproveCacheLocality);
    assert(scene && "Error loading mesh from file");
    // assert(scene->mNumMeshes == 1 && "A mesh file must only contain one mesh");

    mesh mesh;

    for (int m = 0; m < scene->mNumMeshes; ++m) {
        std::vector<vec3> verts, normals;
        gl::buffer vertex_buffer, normal_buffer;

        const aiMesh* obj = scene->mMeshes[m];

        for (int i = 0; i < obj->mNumFaces; ++i) {
            for (int j = 0; j < 3; ++j) {
                int v = obj->mFaces[i].mIndices[j];
                aiVector3D p = obj->mVertices[v];
                aiVector3D n = obj->mNormals[v];
                verts.push_back(vec3{ p.x, p.y, p.z });
                normals.push_back(vec3{ n.x, n.y, n.z });
            }
        }

        vertex_buffer.set_data(verts);
        normal_buffer.set_data(normals);

        mesh::part p;
        p.vertices = verts.size();
        p.object.attach(vertex_buffer, 3, GL_FLOAT, false);
        p.object.attach(normal_buffer, 3, GL_FLOAT, false);
        p.buffers.push_back(std::move(vertex_buffer));
        p.buffers.push_back(std::move(normal_buffer));

        const aiMaterial* material = scene->mMaterials[obj->mMaterialIndex];
        for (int i = 0; i < material->mNumProperties; ++i) {
            printf("Prop: %s\n", material->mProperties[i]->mKey.C_Str());
        }

        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        p.diffuse = { color.r, color.g, color.b };

        mesh.parts.push_back(std::move(p));
    }

    return mesh;
}


/* game_options holds all of the current settings applied to the game.
 */
struct game_options {
    int width, height; /* The current operating resolution. */
};

/* The main entry point into the game.
*/
int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;
    bool running;


    game_options options = {
        1280,
        720
    };

    /* TODO: Parse the command line and configuration file options. */

    SDL_Init(SDL_INIT_EVERYTHING);

    ChaiScript chai;
    chai.use("test.chai");

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              options.width,
                              options.height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    assert(glewInit() == GLEW_OK && "Error initializing GLEW");

    gl::shader vertex(gl::shader::vertex), fragment(gl::shader::fragment);
    vertex.set_source(file::read("shaders/object.vert"));
    fragment.set_source(file::read("shaders/object.frag"));

    gl::program shader;
    shader.build(vertex, fragment);
    shader.use();

    glEnable(GL_DEPTH_TEST);
    mesh model = load_mesh("models/cube.dae");

    node root;
    camera viewport;
    root.add(&viewport);

    viewport.position = { -5.0f, 2.0f, -3.0f };
    viewport.direction = { 5.0f, -2.0f, 3.0f };
    viewport.up = { 0.0f, 1.0f, 0.0f };

    shader.get("transform").set(viewport.transform());
    
    float time = 0;

    running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            default:
                break;
            }
        }

        /* TODO: Update the game logic here. */
        root.update(1.0f);

        /* TODO: Render the game to the screen here. */
        root.draw();

        shader.get("world").set(rotate(time, { 0, 1, 0 }));
        time += 0.01f;

        model.draw(shader.get("diffuse"));

        SDL_GL_SwapWindow(window);
    }
    SDL_Quit();

    return 0;
}
