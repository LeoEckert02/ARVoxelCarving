# pragma once

#include "Mesh.h"
#include "MeshLoader.h"
#include "MeshSmoother.h"
#include "SDF.h"
#include "MarchingCubes.h"

class PostProcInterface {
    Mesh::TriangleMesh f_mesh;
    Mesh::MeshSmoother f_smoother;

    void loadMesh(std::string path);
    void writeMesh(std::string path);

    void clearMesh();

    void addNoise(double sigma);
    void setKernel(Kernel::KernelType KT, double c, double alpha = 1.);
    void smoothenMesh(double rho = 1., uint16_t iterations = 1., bool wide = false);
    void generateMCMesh(const SDF::SDFCSample& sdf, double level);
    void generateMCMesh(const SDF::VoxelGrid& vg, double level);
};