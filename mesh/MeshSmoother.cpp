#include "MeshSmoother.h"

const Mesh::Kernel& Mesh::MeshSmoother::getKernel() const {
    return f_kernel;
}

double Mesh::MeshSmoother::getRho() const {
    return f_rho;
}

void Mesh::MeshSmoother::setKernel(const Mesh::Kernel& kernel) {
    f_kernel = kernel;
}

void Mesh::MeshSmoother::setKernel(Mesh::Kernel&& kernel) {
    f_kernel = std::move(kernel);
}

void Mesh::MeshSmoother::setRho(double rho) {
    f_rho = rho;
}

double Mesh::MeshSmoother::calculateDelta(Mesh::TriangleMesh& mesh, uint32_t id) const {
    double deltaSq = 0;
    uint16_t di = f_neighbours.at(id).size();
    double sqrtDi = sqrt((double)(di));
    Vector3d vi = mesh.getVertex(id).f_coordinates;

    // Iterate over neighbours
    for (auto nb: f_neighbours.at(id)) {
        uint16_t dj = f_neighbours.at(nb).size();
        Vector3d vj = mesh.getVertex(nb).f_coordinates;
        double sqrtDj = sqrt((double)(dj));

        deltaSq += 
              (vi.x() / sqrtDi - vj.x() / sqrtDj) * (vi.x() / sqrtDi - vj.x() / sqrtDj)
            + (vi.y() / sqrtDi - vj.y() / sqrtDj) * (vi.y() / sqrtDi - vj.y() / sqrtDj)
            + (vi.z() / sqrtDi - vj.z() / sqrtDj) * (vi.z() / sqrtDi - vj.z() / sqrtDj);
    }

    return sqrt(deltaSq);
}

void Mesh::MeshSmoother::precalculateNeighbours(const Mesh::TriangleMesh &mesh) {
    f_neighbours.resize(mesh.getVertexCount());
    // Vertex local id
    uint32_t id = 0;
    for (
        std::vector<MDS::Vertex>::const_iterator iter = mesh.getVertices().begin();
        iter != mesh.getVertices().end();
        iter++
    ) {
        f_neighbours.at(id) = mesh.getVertexNeighbours(iter->f_id);
        id++;
    }
}

void Mesh::MeshSmoother::smoothenMesh(Mesh::TriangleMesh &mesh, uint16_t iterations) const {
    for (uint16_t i = 0; i < iterations; i++) {
        // Make new mesh coordinate vector
        std::vector<Vector3d> vertexCoordinates(mesh.getVertexCount());
        // Vertex local id
        uint32_t id = 0;
        // For cycle on vertices
        for (
            std::vector<MDS::Vertex>::const_iterator iter = mesh.getVertices().begin();
            iter != mesh.getVertices().end();
            iter++
        ) {
            Vector3d vi = iter->f_coordinates;
            // Copy previous value
            vertexCoordinates.at(id) = iter->f_coordinates;

            // Get neighbours count
            uint16_t di = f_neighbours.at(id).size();

            double delta = calculateDelta(mesh, iter->f_id);

            double kerMultiplier = f_kernel(delta) / f_rho;

            // Divide multiplier by di for implicit averaging
            kerMultiplier /= (double)(di);

            // Iterate over neighbours
            for (auto nb: f_neighbours.at(id)) {
                Vector3d vj = mesh.getVertex(nb).f_coordinates;
                // Add displacement vector
                vertexCoordinates.at(id) += kerMultiplier * (vj - vi);
            }

            id++;
        }

        // Propagate mesh
        id = 0;
        for (
            std::vector<MDS::Vertex>::iterator iter = mesh.vertices().begin();
            iter != mesh.vertices().end();
            iter++
        ) {
            iter->f_coordinates = vertexCoordinates.at(id);
            id++;
        }
    }
}
