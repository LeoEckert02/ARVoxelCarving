#pragma once

#include "MeshDataStructures.h"
#include <unordered_map>
#include <vector>

namespace Mesh {

    class TriangleMesh {
    private:
        // Elements id generators
        ID::IdGenerator f_vIdGen;
        ID::IdGenerator f_tIdGen;

        // Elements counters
        uint32_t f_vCount;
        uint32_t f_tCount;

        // Mesh element containers
        std::vector<MDS::Vertex> f_vertices;
        std::unordered_map<uint32_t, MDS::Triangle> f_triangles;
    
    public:

        #pragma region Constructors

        TriangleMesh():
        f_vIdGen(),
        f_tIdGen(),
        f_vCount(0),
        f_tCount(0),
        f_vertices(),
        f_triangles()
        {}

        #pragma endregion

        void reserve(uint32_t vCount, uint32_t tCount);

        #pragma region Getters and Setters

        uint32_t getVertexCount() const;

        uint32_t getTriangleCount() const;

        const std::vector<MDS::Vertex>& getVertices() const;

        std::vector<MDS::Vertex>& vertices();

        const std::unordered_map<uint32_t, MDS::Triangle>& getTriangles() const;

        std::unordered_map<uint32_t, MDS::Triangle>& triangles();

        const MDS::Vertex& getVertex(uint32_t id) const;

        const MDS::Triangle& getTriangle(uint32_t id) const;

        MDS::Vertex getVertexCopy(uint32_t id) const;

        MDS::Triangle getTriangleCopy(uint32_t id) const;

        MDS::Vertex& vertex(uint32_t id);

        MDS::Triangle& triangle(uint32_t id);

        #pragma endregion

        #pragma region Adders

        uint32_t addVertex(const Vector3d& coordinates, const Vector4uc& color = Vector4uc(255, 255, 255, 255));

        uint32_t addVertex(Vector3d&& coordinates, Vector4uc&& color = Vector4uc(255, 255, 255, 255));
        
        uint32_t addVertex(const MDS::Vertex& vertex);

        uint32_t addVertex(MDS::Vertex&& vertex);

        uint32_t addTriangle(uint32_t v1Id, uint32_t v2Id, uint32_t v3Id);

        uint32_t addTriangle(const MDS::Triangle& triangle);

        uint32_t addTriangle(MDS::Triangle&& triangle);

        #pragma endregion

        #pragma region Removers

        void clear();

        #pragma endregion

        #pragma region Searchers

        std::vector<uint32_t> getVertexNeighbours(uint32_t vId) const;

        #pragma endregion
    };

}