# pragma once

#include "Mesh.h"
#include "MeshLoader.h"
#include "MeshSmoother.h"
#include "SDF.h"
#include "MarchingCubes.h"

class PostProcInterface {
    Mesh::TriangleMesh f_mesh;
    Mesh::MeshSmoother f_smoother;
    // SDF::VoxelGrid* f_voxelGrid;

public:
    PostProcInterface():
    f_mesh(),
    f_smoother()
    {}

    ~PostProcInterface() {
        f_mesh.clear();
        f_smoother.clearNb();
    }

    void setVoxelGrid(SDF::VoxelGrid* voxelGrid);

    bool loadMesh(std::string path);
    bool writeMesh(std::string path);

    void clearMesh();

    void addNoise(double sigma);
    void setKernel(Kernel::KernelType KT, double c, double alpha = 1.);
    void smoothenMesh(double rho = 1., uint16_t iterations = 1., bool wide = false, bool useDelta = false);
    void generateMCMesh(const SDF::SDFCSample& sdf, double level);
    void generateMCMeshVG(double level);

    bool isExitString(const std::string& str);

    void loadMeshMenu();
    void writeMeshMenu();
    void clearMeshMenu();
    void addNormalNoiseMenu();
    void setKernelMenu();
    void smoothenMeshMenu();
    void generateMeshFromVoxelGridMenu();

    void mainMenu();
};