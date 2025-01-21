#include "VoxelCarveTest.h"
#include "VoxelGrid.h"
#include "mesh/PostProcInterface.h"
#include "sam_segmentation_generation.h"

int main() {
    // VoxelCarveTest voxelCarveTest;
    // VoxelGrid carved_voxel_grid = voxelCarveTest.Test();
    auto matrixes = SamSegmentationGenerator::grabSegmentedImages();

    //std::vector<cv::Mat> &silluoetse = sam_segmentation_genereation();
    // PostProcInterface postProcInterface;
    // postProcInterface.setVoxelGrid(&carved_voxel_grid);
    // postProcInterface.generateMCMeshVG(0.);
    // postProcInterface.smoothenMesh(1., 5);
    // postProcInterface.writeMesh("result/output_mesh.off");
}
