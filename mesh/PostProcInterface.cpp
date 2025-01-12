#include "PostProcInterface.h"

void PostProcInterface::loadMesh(std::string path) {
    try {
    MeshLoader::loadTriangleMeshOff(path, f_mesh);
    } catch (std::exception ex) {
        std::cout << "Error:" << std::endl;
        std::cout << ex.what();
    }
}

void PostProcInterface::writeMesh(std::string path) {
    try {
    MeshLoader::writeTriangleMeshOff(path, f_mesh);
    } catch (std::exception ex) {
        std::cout << "Error:" << std::endl;
        std::cout << ex.what();
    }
}

void PostProcInterface::clearMesh() {
    f_mesh.clear();
}

void PostProcInterface::addNoise(double sigma) {
    f_mesh.addNoise(sigma);
}

void PostProcInterface::setKernel(Kernel::KernelType KT, double c, double alpha) {
    switch (KT)
    {
    case Kernel::KernelType::AVG:
        f_smoother.setKernel(new Kernel::Kernel());
        break;
    case Kernel::KernelType::CAUCHY:
        f_smoother.setKernel(new Kernel::CauchyKernel(c, alpha));
        break;
    case Kernel::KernelType::RCGAUSSIAN:
        f_smoother.setKernel(new Kernel::RCGaussianKernel(c, alpha));
        break;
    case Kernel::KernelType::LAPLACE:
        f_smoother.setKernel(new Kernel::LaplaceKernel(c, alpha));
        break;
    case Kernel::KernelType::RAYLEIGH:
        f_smoother.setKernel(new Kernel::RayleighKernel(c, alpha));
        break;
    default:
        f_smoother.setKernel(new Kernel::Kernel());
        break;
    }
}

void PostProcInterface::smoothenMesh(double rho, uint16_t iterations, bool wide) {
    if (!f_smoother.nbPrecalculated()) {
        f_smoother.precalculateNeighbours(f_mesh);
    }
    f_smoother.setRho(rho);
    f_smoother.smoothenMesh(f_mesh, iterations, wide);
}

void PostProcInterface::generateMCMesh(const SDF::SDFCSample &sdf, double level) {
    MarchingCubes::MarchingCubesMeshGenerator generator;
    generator.generateMCMesh(f_mesh, sdf, level);
}

// void PostProcInterface::generateMCMesh(const SDF::VoxelGrid &vg, double level) {
//     MarchingCubes::MarchingCubesMeshGenerator generator;
//     generator.generateMCMesh(f_mesh, SDF::SDFCSample::convertVoxelGridToSDFC(vg), level);
// }
