#include <cstdlib>
#include "VoxelCarve.h"
#include "VoxelGrid.h"
#include "mesh/PostProcInterface.h"
#include "sam_segmentation_generation.h"

int main(int argc, char* argv[]) {

    constexpr int DEFAULT_RESOLUTION = 300;
    int resolution = DEFAULT_RESOLUTION;

    if (argc > 1) {
        try {
            resolution = std::stoi(argv[1]); // Convert the argument to an integer
        } catch (const std::exception& e) {
            std::cerr << "Invalid argument: " << argv[1] << ". Using default resolution: " 
                      << DEFAULT_RESOLUTION << std::endl;
            resolution = DEFAULT_RESOLUTION;
        }
    }

    std::cout << "Using resolution: " << resolution << std::endl;

    // Main Chad Segmentation part
    const SegmentationParams params;
    auto silhouettes = SamSegmentationGenerator::grabSegmentedImages(params);

    // Rest of the code
    VoxelCarve voxelCarve(silhouettes, resolution, true); // the number is the highest resolution for a dimension (others will be calculated automatically to create cubic voxels)
    VoxelGrid carved_voxel_grid = voxelCarve.carve_voxel_grid();

    PostProcInterface postProcInterface;
    postProcInterface.setVoxelGrid(&carved_voxel_grid);
    postProcInterface.generateMCMeshVG(0.);
    postProcInterface.setKernel(Kernel::KernelType::INVERSE, 1., 0.75);
    postProcInterface.smoothenMesh(2., 20);
    
    postProcInterface.writeMesh("output_mesh.off");
}