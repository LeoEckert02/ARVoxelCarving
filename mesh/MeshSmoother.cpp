#include "MeshSmoother.h"

const std::shared_ptr<Kernel::Kernel>& Mesh::MeshSmoother::getKernel() const {
    return f_kernel;
}

double Mesh::MeshSmoother::getRho() const {
    return f_rho;
}

bool Mesh::MeshSmoother::nbPrecalculated() const {
    return nbpc;
}

void Mesh::MeshSmoother::setKernel(Kernel::Kernel* kernel) {
    f_kernel = std::shared_ptr<Kernel::Kernel>(kernel);
}

void Mesh::MeshSmoother::setRho(double rho) {
    f_rho = rho;
}

void Mesh::MeshSmoother::clearNb() {
    for (auto nbVec: f_neighbours) {
        nbVec.clear();
    }
    f_neighbours.clear();
    nbpc = false;
}

double Mesh::MeshSmoother::calculateDelta(Mesh::TriangleMesh& mesh, const std::vector<uint32_t>& neighbours, uint32_t id) const {
    double deltaSq = 0;
    uint16_t di = neighbours.size();
    double sqrtDi = sqrt((double)(di));
    Vector3d vi = mesh.getVertex(id).f_coordinates;

    // Iterate over neighbours
    for (auto nb: neighbours) {
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
    for (
        std::unordered_map<uint32_t, MDS::Triangle>::const_iterator iter = mesh.getTriangles().begin();
        iter != mesh.getTriangles().end();
        iter++
    ) {
        f_neighbours.at(iter->second.f_v1Id).push_back(iter->second.f_v2Id);
        f_neighbours.at(iter->second.f_v1Id).push_back(iter->second.f_v3Id);
        f_neighbours.at(iter->second.f_v2Id).push_back(iter->second.f_v1Id);
        f_neighbours.at(iter->second.f_v2Id).push_back(iter->second.f_v3Id);
        f_neighbours.at(iter->second.f_v3Id).push_back(iter->second.f_v1Id);
        f_neighbours.at(iter->second.f_v3Id).push_back(iter->second.f_v2Id);
    }
    for (auto nbVec: f_neighbours) {
        std::sort(nbVec.begin(), nbVec.end());
        auto nEnd = std::unique(nbVec.begin(), nbVec.end());
        nbVec.erase(nEnd, nbVec.end());
    }
    nbpc = true;
}

void Mesh::MeshSmoother::smoothenMesh(Mesh::TriangleMesh &mesh, uint16_t iterations, bool wide) const {
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

            // Get neighbours
            std::vector<uint32_t> neighbours = f_neighbours.at(id);
            if (wide) {
                for (auto nb: f_neighbours.at(id)) {
                    for (auto nbnb: f_neighbours.at(nb)) {
                        if (nbnb == id) continue;
                        if(std::find(neighbours.begin(), neighbours.end(), nbnb) == neighbours.end()) {
                            neighbours.push_back(nbnb);
                        }
                    }
                }
            }

            uint16_t di = neighbours.size();

            double delta = calculateDelta(mesh, neighbours, iter->f_id);

            double kerMultiplier = f_kernel->g(delta) / f_rho;

            // Divide multiplier by di for implicit averaging
            kerMultiplier /= (double)(di);

            // Iterate over neighbours
            for (auto nb: neighbours) {
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
