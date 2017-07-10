
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


