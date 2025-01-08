#include "Mesh.h"

void Mesh::TriangleMesh::reserve(uint32_t vCount, uint32_t tCount) {
    f_vertices.reserve(vCount);
    f_triangles.reserve(tCount);
}

uint32_t Mesh::TriangleMesh::getVertexCount() const {
    return f_vCount;
}

uint32_t Mesh::TriangleMesh::getTriangleCount() const {
    return f_tCount;
}

const std::vector<MDS::Vertex>& Mesh::TriangleMesh::getVertices() const {
    return f_vertices;
}

std::vector<MDS::Vertex>& Mesh::TriangleMesh::vertices() {
    return f_vertices;
}

const std::unordered_map<uint32_t, MDS::Triangle>& Mesh::TriangleMesh::getTriangles() const {
    return f_triangles;
}

std::unordered_map<uint32_t, MDS::Triangle>& Mesh::TriangleMesh::triangles() {
    return f_triangles;
}

const MDS::Vertex& Mesh::TriangleMesh::getVertex(uint32_t id) const {
    return f_vertices.at(id);
}

const MDS::Triangle& Mesh::TriangleMesh::getTriangle(uint32_t id) const {
    return f_triangles.at(id);
}

MDS::Vertex Mesh::TriangleMesh::getVertexCopy(uint32_t id) const {
    return MDS::Vertex(f_vertices.at(id));
}

MDS::Triangle Mesh::TriangleMesh::getTriangleCopy(uint32_t id) const {
    return MDS::Triangle(f_triangles.at(id));
}

MDS::Vertex& Mesh::TriangleMesh::vertex(uint32_t id) {
    return f_vertices.at(id);
}

MDS::Triangle& Mesh::TriangleMesh::triangle(uint32_t id) {
    return f_triangles.at(id);
}

uint32_t Mesh::TriangleMesh::addVertex(const Vector3d &coordinates, const Vector4uc &color) {
    uint32_t id = f_vIdGen.generateId();
    f_vertices.emplace_back(MDS::Vertex(id, coordinates, color));
    f_vCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addVertex(Vector3d &&coordinates, Vector4uc &&color) {
    uint32_t id = f_vIdGen.generateId();
    f_vertices.emplace_back(MDS::Vertex(id, std::move(coordinates), std::move(color)));
    f_vCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addVertex(const MDS::Vertex &vertex) {
    uint32_t id = f_vIdGen.generateId();
    f_vertices.emplace_back(MDS::Vertex(id, vertex));
    f_vCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addVertex(MDS::Vertex &&vertex) {
    uint32_t id = f_vIdGen.generateId();
    f_vertices.emplace_back(MDS::Vertex(id, std::move(vertex)));
    f_vCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addTriangle(uint32_t v1Id, uint32_t v2Id, uint32_t v3Id) {
    uint32_t id = f_tIdGen.generateId();
    f_triangles.emplace(id, MDS::Triangle(id, v1Id, v2Id, v3Id));
    f_tCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addTriangle(const MDS::Triangle &triangle) {
    uint32_t id = f_tIdGen.generateId();
    f_triangles.emplace(id, MDS::Triangle(id, triangle));
    f_tCount++;
    return id;
}

uint32_t Mesh::TriangleMesh::addTriangle(MDS::Triangle &&triangle) {
    uint32_t id = f_tIdGen.generateId();
    f_triangles.emplace(id, MDS::Triangle(id, std::move(triangle)));
    f_tCount++;
    return id;
}

void Mesh::TriangleMesh::clear() {
    f_vIdGen.reset();
    f_tIdGen.reset();

    f_vCount = 0;
    f_tCount = 0;

    f_vertices.clear();
    f_triangles.clear();
}

std::vector<uint32_t> Mesh::TriangleMesh::getVertexNeighbours(uint32_t vId) const {
    std::vector<uint32_t> neighbourIds;
    for (
        std::unordered_map<uint32_t, MDS::Triangle>::const_iterator iter = f_triangles.begin();
        iter != f_triangles.end();
        iter++
    ) {
        if (iter->second.contains(vId)) {
            if (iter->second.f_v1Id != vId) neighbourIds.push_back(iter->second.f_v1Id);
            if (iter->second.f_v2Id != vId) neighbourIds.push_back(iter->second.f_v2Id);
            if (iter->second.f_v3Id != vId) neighbourIds.push_back(iter->second.f_v3Id);
        }
    }
    return neighbourIds;
}
