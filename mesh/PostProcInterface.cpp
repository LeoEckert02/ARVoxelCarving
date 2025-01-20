#include "PostProcInterface.h"

void PostProcInterface::setVoxelGrid(VoxelGrid* voxelGrid) {
    f_voxelGrid = voxelGrid;
}

bool PostProcInterface::loadMesh(std::string path)
{
    try {
    MeshLoader::loadTriangleMeshOff(path, f_mesh);
    } catch (std::invalid_argument ex) {
        std::cout << "\t[MPP] Error: Failed to open file.";
        return false;
    }
    f_smoother.clearNb();
    return true;
}

bool PostProcInterface::writeMesh(std::string path) {
    try {
    MeshLoader::writeTriangleMeshOff(path, f_mesh);
    } catch (std::invalid_argument ex) {
        std::cout << "\t[MPP] Error: Failed to open file.";
        return false;
    }
    return true;
}

void PostProcInterface::clearMesh() {
    f_mesh.clear();
    f_smoother.clearNb();
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
        std::cout << "\t[MPP] Vertex neighbour data is not yet calculated for current mesh." << std::endl
                  << "\t[MPP] Precalculating neighbours..." << std::endl;
        f_smoother.precalculateNeighbours(f_mesh);
    }
    f_smoother.setRho(rho);
    std::cout << "\t[MPP] Smoothing mesh..." << std::endl << std::endl;
    f_smoother.smoothenMesh(f_mesh, iterations, wide);
}

void PostProcInterface::generateMCMesh(const SDF::SDFCSample &sdf, double level) {
    MarchingCubes::MarchingCubesMeshGenerator generator;
    f_smoother.clearNb();
    generator.generateMCMesh(f_mesh, sdf, level);
}

void PostProcInterface::generateMCMeshVG(double level) {
    if (f_voxelGrid == nullptr) {
        std::cout << "\t[MPP] Error: The voxel grid was not passed to the post processing module." << std::endl;
    }
    f_smoother.clearNb();
    MarchingCubes::MarchingCubesMeshGenerator generator;
    generator.generateMCMesh(f_mesh, SDF::SDFCSample::convertVoxelGridToSDFC(*f_voxelGrid), level);
    return;
}

bool PostProcInterface::isExitString(const std::string& str) {
    return str.compare("Exit") == 0 
        || str.compare("Ex") == 0 
        || str.compare("exit") == 0 
        || str.compare("ex") == 0 
        || str.compare("E") == 0 
        || str.compare("e") == 0
        || str.compare("menu") == 0;
}

void PostProcInterface::loadMeshMenu() {
    std::string input;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Input mesh .off file path:" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        std::cout << "\t[MPP] Loading mesh..." << std::endl;
        if (loadMesh(input)) {
            std::cout << "\t[MPP] Loaded mesh from " << input << " successfully." << std::endl << std::endl;
            exit = true;
        }
    }
}

void PostProcInterface::writeMeshMenu() {
    std::string input;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Input mesh .off file name without extension:" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        input.append(".off");
        std::cout << "\t[MPP] Writing mesh..." << std::endl;
        if (writeMesh(input)) {
            std::cout << "\t[MPP] Written mesh to " << input << " successfully." << std::endl << std::endl;
            exit = true;
        }
    }
}

void PostProcInterface::clearMeshMenu() {
    std::string input;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Do you want to clear currently loaded mesh (irreversible)? [Y/N]" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input) || input.compare("N") == 0 || input.compare("n") == 0) {
            exit = true;
            continue;
        }
        if (input.compare("Y") == 0 || input.compare("y") == 0) {
            std::cout << "\t[MPP] Clearing mesh..." << std::endl;
            clearMesh();
            std::cout << "\t[MPP] Cleared mesh successfully." << std::endl << std::endl;
            exit = true;
            continue;
        }
        std::cout << "\t[MPP] Invalid input." << std::endl << std::endl;
    }
}

void PostProcInterface::addNormalNoiseMenu() {
    std::string input;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Do you want to add normal noise to currently loaded mesh (irreversible)? [Y/N]" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input) || input.compare("N") == 0 || input.compare("n") == 0) {
            exit = true;
            continue;
        }
        else if (input.compare("Y") == 0 || input.compare("y") == 0) {
            std::cout << "\t[MPP] Input the standard deviation:" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
            std::cin >> input;
            std::cout << std::endl;
            if (isExitString(input)) {
                exit = true;
                continue;
            }
            std::cout << "\t[MPP] Adding normal noise..." << std::endl;
            addNoise(std::stod(input));
            std::cout << "\t[MPP] Added normal noise successfully." << std::endl << std::endl;
            exit = true;
            continue;
        }
        std::cout << "\t[MPP] Invalid input." << std::endl << std::endl;
    }
}

void PostProcInterface::setKernelMenu() {
    std::string input;
    uint16_t option;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Choose Kernel type:" << std::endl
                  << "\t[MPP] 1) Local averaging;" << std::endl
                  << "\t[MPP] 2) Cauchy;" << std::endl
                  << "\t[MPP] 3) Gaussian;" << std::endl
                  << "\t[MPP] 4) Laplace;" << std::endl
                  << "\t[MPP] 5) Rayleigh;" << std::endl
                  << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                  << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        option = std::stoi(input);
        if (option > 0 and option <= 5) {
            double c, alpha;
            if (option != 1) {
                std::cout << "\t[MPP] Input a value for kernel parameter c (default: 1.):" << std::endl
                    << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                    << "\t[MPP] >> ";
                std::cin >> input;
                std::cout << std::endl;
                if (isExitString(input)) {
                    exit = true;
                    continue;
                }
                c = std::stod(input);
                std::cout << "\t[MPP] Input a value in range (0.0, 1.0] for kernel interpolation with local averaging," << std::endl
                    << "\t[MPP] 0.0 - only local averaging, 1.0 - no local averaging:" << std::endl
                    << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                    << "\t[MPP] >> ";
                std::cin >> input;
                std::cout << std::endl;
                if (isExitString(input)) {
                    exit = true;
                    continue;
                }
                alpha = std::stod(input);
            }
            switch (option) {
            case 1:
                setKernel(Kernel::KernelType::AVG, 0., 0.);
                std::cout << "\t[MPP] Kernel set successfully." << std::endl << std::endl;
                exit = true;
                break;
            case 2:
                setKernel(Kernel::KernelType::CAUCHY, c, alpha);
                std::cout << "\t[MPP] Kernel set successfully." << std::endl << std::endl;
                exit = true;
                break;
            case 3:
                setKernel(Kernel::KernelType::RCGAUSSIAN, c, alpha);
                std::cout << "\t[MPP] Kernel set successfully." << std::endl << std::endl;
                exit = true;
                break;
            case 4:
                setKernel(Kernel::KernelType::LAPLACE, c, alpha);
                std::cout << "\t[MPP] Kernel set successfully." << std::endl << std::endl;
                exit = true;
                break;
            case 5:
                setKernel(Kernel::KernelType::RAYLEIGH, c, alpha);
                std::cout << "\t[MPP] Kernel set successfully." << std::endl << std::endl;
                exit = true;
                break;
            default:
                std::cout << "\t[MPP] Invalid input." << std::endl << std::endl;
                break;
            }
        }
        else {
            std::cout << "\t[MPP] Invalid input." << std::endl << std::endl;
        }
    }
}

void PostProcInterface::smoothenMeshMenu() {
    std::string input;
    bool exit = false;
    while (!exit) {
        std::cout << "\t[MPP] Input a value for smoothing parameter rho (default: 1.):" << std::endl
                << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        double rho = stod(input);
        std::cout << "\t[MPP] Input a number of smoothing iterations:" << std::endl
                << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        uint16_t iterations = std::stoi(input);
        std::cout << "\t[MPP] Do you want to use wide smoothing (default: false) [Y/N]:?" << std::endl
                << "\t[MPP] (Wide smoothing = the smoother takes seond neighbours into account)" << std::endl
                << "\t[MPP] (Input \"Exit\" to exit)" << std::endl << std::endl
                << "\t[MPP] >> ";
        std::cin >> input;
        std::cout << std::endl;
        if (isExitString(input)) {
            exit = true;
            continue;
        }
        bool wide = !(input.compare("y") || input.compare("Y"));
        smoothenMesh(rho, iterations, wide);
        std::cout << "\t[MPP] Smoothened mesh successfully." << std::endl << std::endl;
        exit = true;
        continue;
    }
}

void PostProcInterface::generateMeshFromVoxelGridMenu() {
    std::cout << "\t[MPP] Generating mesh from the voxel grid..." << std::endl;
    generateMCMeshVG(0.);
    std::cout << "\t[MPP] Generated mesh successfully." << std::endl << std::endl;
}

void PostProcInterface::mainMenu() {
    bool exit = false;
    while(!exit) {
        std::cout << "\t[MPP] Options:" << std::endl
                << "\t[MPP] 1) Load mesh;" << std::endl
                << "\t[MPP] 2) Write mesh;" << std::endl
                << "\t[MPP] 3) Clear mesh;" << std::endl
                << "\t[MPP] 4) Add normal noise;" << std::endl
                << "\t[MPP] 5) Set smoothing kernel;" << std::endl
                << "\t[MPP] 6) Smoothen mesh;" << std::endl
                << "\t[MPP] 7) Generate mesh from voxel grid;" << std::endl
                << "\t[MPP] 8) Back." << std::endl << std::endl
                << "\t[MPP] >> ";

        uint16_t option;
        
        std::cin >> option;
        std::cout << std::endl;
        switch (option)
        {
        case 1:
            loadMeshMenu();
            break;
        case 2:
            writeMeshMenu();
            break;
        case 3:
            clearMeshMenu();
            break;
        case 4:
            addNormalNoiseMenu();
            break;
        case 5:
            setKernelMenu();
            break;
        case 6:
            smoothenMeshMenu();
            break;
        case 7:
            generateMeshFromVoxelGridMenu();
            break;
        case 8:
            exit = true;
            break;
        default:
            std::cout << "\t[MPP] Invalid input." << std::endl << std::endl;
            break;
        }
    }
}