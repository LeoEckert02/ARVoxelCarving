# pragma once

#include "Mesh.h"
#include "MeshLoader.h"
#include "MeshSmoother.h"
#include "SDF.h"
#include "MarchingCubes.h"
#include "../VoxelGrid.h"

class PostProcInterface {
    Mesh::TriangleMesh f_mesh;
    Mesh::MeshSmoother f_smoother;
    VoxelGrid* f_voxelGrid;

public:
    PostProcInterface():
    f_mesh(),
    f_smoother()
    {}

    ~PostProcInterface() {
        f_mesh.clear();
        f_smoother.clearNb();
    }

    /// @brief Sets the VoxelGrid* field for further mesh generation with generateMCMeshVG(double)
    /// @param voxelGrid Voxel grid pointer to pass to the PostProcInterface field.
    void setVoxelGrid(VoxelGrid* voxelGrid);

    /// @brief Loads mesh from .off file specified. Throws exception if the file can not be opened.
    /// @param path Full path to .off file to load the mesh from.
    /// @return bool flag, true if load succeded, false otherwise.
    bool loadMesh(std::string path);
    /// @brief Writes mesh to .off file specified. Throws exception if the file can not be opened.
    /// @param path Full path to .off file to write the mesh to.
    /// @return bool flag, true if write succeded, false otherwise.
    bool writeMesh(std::string path);

    /// @brief Clears current mesh.
    void clearMesh();

    /// @brief Adds normal noise to every mesh vertex from normal distribution with standard deviation specified.
    /// @param sigma Standard deviation of noise distribution.
    void addNoise(double sigma);
    /// @brief Sets the smoothing kernel for the mesh smoother.
    /// @param KT Kernel type to set.
    /// @param c Kernel parameter (default 1.).
    /// @param alpha Interpolation with local averaging ratio (alpha = 1 - no local averaging, kernel only).
    void setKernel(Kernel::KernelType KT, double c, double alpha = 1.);
    /// @brief Smothens currently loaded mesh.
    /// @param rho Smoothing kernel multiplier (Ker. / rho) (default 1., bigger values for slower smoothing).
    /// @param iterations Number of smoothing iteration to perform.
    /// @param wide Wide smoothing flag, if true, the smoothing process will consider second neighbours.
    void smoothenMesh(double rho = 1., uint16_t iterations = 1., bool wide = false);
    /// @brief Generates a Marching Cubes mesh for an isosurface at a given level according to a given SDF.
    /// @param sdf SDF to generate the surface from.
    /// @param level Surface level.
    void generateMCMesh(const SDF::SDFCSample& sdf, double level);
    /// @brief Generates a Marching Cubes mesh from a filled voxel grid in f_voxelGrid field.
    /// @param level Surface level (default 0.)
    void generateMCMeshVG(double level = 0.);

    bool isExitString(const std::string& str);

    /// @brief Load mesh submenu.
    void loadMeshMenu();
    /// @brief Write mesh submenu.
    void writeMeshMenu();
    /// @brief Clear mesh submenu.
    void clearMeshMenu();
    /// @brief Normal noise submenu.
    void addNormalNoiseMenu();
    /// @brief Kernel set submenu.
    void setKernelMenu();
    /// @brief Mesh smoothing submenu.
    void smoothenMeshMenu();
    /// @brief Mesh from voxel grid generation submenu.
    void generateMeshFromVoxelGridMenu();

    /// @brief Main bash menu.
    void mainMenu();
};