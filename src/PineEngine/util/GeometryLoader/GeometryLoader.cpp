#include "GeometryLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <string>
#include <utility>

namespace PineEngine {
    GeometryLoader::GeometryLoader(const Path &path) : path(path) {
    }

    std::pair<std::vector<VertexData>, std::vector<uint32_t> > GeometryLoader::load() {
        return this->_loadFromFile();
    }

    std::pair<std::vector<VertexData>, std::vector<uint32_t> > GeometryLoader::_loadFromFile() {
        const auto path_ = this->path.asAbsolutePathString();

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path_, aiProcess_Triangulate | aiProcess_GenNormals);

        if (!scene) {
            throw std::runtime_error(FORMAT("Failed to load model \"{}\"!", path_));
        }

        VertexData vertices{.name = "vertexInPosition", .data = {}, .dimensionality = 4};
        VertexData normals{.name = "vertexInNormal", .data = {}, .dimensionality = 4};
        std::vector<uint32_t> indices;

        uint32_t indexOffset = 0;
        for (uint32_t mi = 0; mi < scene->mNumMeshes; mi++) {
            const aiMesh *mesh = scene->mMeshes[mi];

            for (uint32_t vi = 0; vi < mesh->mNumVertices; vi++) {
                vertices.data.push_back(mesh->mVertices[vi].x);
                vertices.data.push_back(mesh->mVertices[vi].y);
                vertices.data.push_back(mesh->mVertices[vi].z);
                vertices.data.push_back(1.0f);
                normals.data.push_back(mesh->mNormals[vi].x);
                normals.data.push_back(mesh->mNormals[vi].y);
                normals.data.push_back(mesh->mNormals[vi].z);
                normals.data.push_back(0.0f);
            }

            for (uint32_t fi = 0; fi < mesh->mNumFaces; fi++) {
                const auto &face = mesh->mFaces[fi];
                for (uint32_t ii = 0; ii < face.mNumIndices; ii++) {
                    indices.push_back(face.mIndices[ii] + indexOffset);
                }
            }

            indexOffset += mesh->mNumVertices;
        }

        return {{std::move(vertices), std::move(normals)}, indices};
    }
} // namespace PineEngine
