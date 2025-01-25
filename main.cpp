#include "VoxelCarveTest.h"
#include "VoxelGrid.h"
#include "mesh/PostProcInterface.h"
#include "sam_segmentation_generation.h"

int main() {
    auto silhouettes = SamSegmentationGenerator::grabSegmentedImages();
    VoxelCarveTest* voxelCarveTest = new VoxelCarveTest(silhouettes, true);
    VoxelGrid carved_voxel_grid = voxelCarveTest->Test();

    PostProcInterface postProcInterface;
    postProcInterface.setVoxelGrid(&carved_voxel_grid);
    postProcInterface.generateMCMeshVG(0.);
    postProcInterface.setKernel(Kernel::KernelType::LAPLACE, 1., 0.8);
    postProcInterface.smoothenMesh(2., 5);
    postProcInterface.writeMesh("output_mesh.off");
}