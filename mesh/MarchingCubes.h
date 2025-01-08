#pragma once

#include <Eigen/Dense>
#include <cstdint>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "SDF.h"

using namespace Eigen;

namespace MarchingCubes {
    
    struct MarchingCubesCell {
        Vector3d vertices[8];
	    double values[8];
        Vector4uc colors[8];
    };
    
    
    class MarchingCubesMeshGenerator {
    private:
        static const int edgeTable[256];

        static const int triangleTable[256][16];
    
        // Temp edge occupancy
        // An int vector of length 12 for every grid cell
        std::vector<std::vector<std::vector<std::vector<int>>>> f_edgeOccupancy;
        

    public:
        static Vector3d vertexInterpolate(
            const Vector3d& v1, 
            const Vector3d& v2,
            double level,
            double value1, 
            double value2
        );

        void prepareEdgeOccupancy(uint32_t n, uint32_t m, uint32_t p);

        int lookupEdgeOccupancy(uint32_t i, uint32_t j, uint32_t k, uint32_t edgeId, uint32_t n, uint32_t m, uint32_t p) const;

        void updateOccupancy(uint32_t i, uint32_t j, uint32_t k, uint32_t edgeId, uint32_t vertexId, uint32_t n, uint32_t m, uint32_t p);

        void handleEdge(unsigned i, unsigned j, unsigned k,
            uint32_t n, uint32_t m, uint32_t p,
            uint32_t edgeId, uint32_t v1Id, uint32_t v2Id,
            const MarchingCubesCell& cell, double level,
            uint32_t* vertexIds,
            Mesh::TriangleMesh& mesh
        );

        void generateMCMesh(Mesh::TriangleMesh& mesh, const SDF::SDFCSample& sdf, double level);
    };
}
