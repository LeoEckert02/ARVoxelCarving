#include "VoxelCarveTest.h"
#include "VoxelGrid.h"
#include "mesh/PostProcInterface.h"
#include "sam_segmentation_generation.h"

int main() {
    std::cout<<"Silhouette creation\n";
    auto silhouettes = SamSegmentationGenerator::grabSegmentedImages();
    std::cout<<"Created voxel carve test\n";
    VoxelCarveTest* voxelCarveTest = new VoxelCarveTest(silhouettes, false);
    VoxelGrid carved_voxel_grid = voxelCarveTest->Test();

    PostProcInterface postProcInterface;
    postProcInterface.setVoxelGrid(&carved_voxel_grid);
    postProcInterface.generateMCMeshVG(0.);
    postProcInterface.smoothenMesh(1., 5);
    postProcInterface.writeMesh("output_mesh.off");
}
