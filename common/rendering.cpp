
// Implement the stb image library
#define STB_IMAGE_IMPLEMENTATION

#include "rendering.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void mesh::load_file(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_ImproveCacheLocality);
    assert(scene && "Error loading mesh from file");

    for (int m = 0; m < scene->mNumMeshes; ++m) {
        std::vector<vec3> verts, normals;
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

        part p;
        p.vertices = verts.size();
        p.add_buffer(verts, 3, GL_FLOAT, false);
        p.add_buffer(normals, 3, GL_FLOAT, false);

        const aiMaterial* material = scene->mMaterials[obj->mMaterialIndex];
        for (int i = 0; i < material->mNumProperties; ++i) {
            printf("Prop: %s\n", material->mProperties[i]->mKey.C_Str());
        }

        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        p.albedo = { color.r, color.g, color.b };
        p.metalness = 0.0f;
        p.roughness = 0.4f;

        parts.push_back(std::move(p));
    }
}

void mesh::generate_part(const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<std::pair<int, int>>& indices, const vec3& albedo, float roughness, float metalness) {
    part p;

    std::vector<vec3> v, n;

    for (const auto& i : indices) {
        v.push_back(vertices[i.first]);
        n.push_back(normals[i.second]);
    }

    p.vertices = indices.size();
    p.add_buffer(v, 3, GL_FLOAT, false);
    p.add_buffer(n, 3, GL_FLOAT, false);

    p.albedo = albedo;
    p.metalness = metalness;
    p.roughness = roughness;

    parts.push_back(std::move(p));
}


mesh mesh::cube() {
    mesh m;

    std::vector<vec3> verts = {
        { -1, -1, -1 },
        { 1, -1, -1 },
        { -1, -1, 1 },
        { 1, -1, 1 },
        { -1, 1, -1 },
        { 1, 1, -1 },
        { -1, 1, 1 },
        { 1, 1, 1 }
    };

    std::vector<vec3> norms = {
        { 1, 0, 0 },
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 0, -1, 0 },
        { 0, 0, 1 },
        { 0, 0, -1 }
    };

    std::vector<std::pair<int, int>> inds = {
        { 1, 0 }, { 3, 0 }, { 5, 0 }, { 3, 0 },{ 7, 0 }, { 5, 0 },
        { 0, 1 }, { 2, 1 }, { 4, 1 }, { 2, 1 },{ 6, 1 }, { 4, 1 },
        { 4, 2 }, { 5, 2 }, { 6, 2 }, { 5, 2 },{ 7, 2 }, { 6, 2 },
        { 0, 3 }, { 1, 3 }, { 2, 3 }, { 1, 3 },{ 3, 3 }, { 2, 3 },
        { 2, 4 }, { 3, 4 }, { 6, 4 }, { 3, 4 },{ 7, 4 }, { 6, 4 },
        { 0, 5 }, { 1, 5 }, { 4, 5 }, { 1, 5 },{ 5, 5 }, { 4, 5 },
    };

    m.generate_part(verts, norms, inds, { 1.0f, 1.0f, 1.0f }, 0.4f, 0.0f);

    return m;
}

