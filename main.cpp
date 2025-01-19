#include <iostream>

#include "mesh/PostProcInterface.h"
//https://www.researchgate.net/publication/44298376_3D-Mesh_denoising_using_an_improved_vertex_based_anisotropic_diffusion
//https://link.springer.com/chapter/10.1007/978-3-642-31254-0_9

int main(int argc, char const *argv[]) {
    srand (static_cast <unsigned> (time(0)));




    PostProcInterface interface;
    interface.mainMenu();




    // Mesh::TriangleMesh triMesh;
    // std::string inPath("../data/features_normal_noise.off");

    // std::cout << "Loading mesh..." << std::endl;
    // MeshLoader::loadTriangleMeshOff(inPath, triMesh);
    // std::cout << "Mesh loaded!" << std::endl;

    // // triMesh.addNoise(0.005);

    // // +++++++++++++++++++++++++++
    // // +++++++++SMOOTHING+++++++++
    // // +++++++++++++++++++++++++++
    // Mesh::MeshSmoother smoother(
    //     new Kernel::LaplaceKernel(1., 0.8),
    //     2.
    // );
    // std::cout << "Precalculating neighbours..." << std::endl;
    // smoother.precalculateNeighbours(triMesh);
    // std::cout << "Smoothing mesh..." << std::endl;
    // smoother.smoothenMesh(triMesh, 10, false);

    // // +++++++++++++++++++++++++++
    // // +++++NEIGHBOUR  SEARCH+++++
    // // +++++++++++++++++++++++++++
    // std::cout << "Finding neighbours of vertex 15..." << std::endl;
    // auto n15 = triMesh.getVertexNeighbours(15);
    // std::cout << "Setting 15 as blue, neighbours as red..." << std::endl;
    // triMesh.vertex(15).f_color = Vector4uc(0, 0, 255, 255);
    // for (auto nb: n15) {
    //     triMesh.vertex(nb).f_color = Vector4uc(255, 0, 0, 255);
    // }
    // std::cout << "Finding neighbours of vertex 83..." << std::endl;
    // auto n83 = triMesh.getVertexNeighbours(83);
    // std::cout << "Setting 83 as blue, neighbours as red..." << std::endl;
    // triMesh.vertex(83).f_color = Vector4uc(0, 0, 255, 255);
    // for (auto nb: n83) {
    //     triMesh.vertex(nb).f_color = Vector4uc(255, 0, 0, 255);
    // }

    // // +++++++++++++++++++++++++++
    // // +++++++++SDF MESH++++++++++
    // // +++++++++++++++++++++++++++
    // std::cout << "Making SDF..." << std::endl;
    // SDF::SDFCSample sdfcs(
    //     100,
    //     100,
    //     100,
    //     Vector3d(-2, -2, -2),
    //     Vector3d(4., 4., 4.)
    // );
    // std::cout << "Filling SDF..." << std::endl;
    // sdfcs.fillSDF(
    //     // [](Vector3d point) {
    //     //     double x = point.x();
    //     //     double y = point.y();
    //     //     double z = point.z();
    //     //     return (x - 2) * (x - 2) * (x + 2) * (x + 2)
    //     //         + (y - 2) * (y - 2) * (y + 2) * (y + 2)
    //     //         + (z - 2) * (z - 2) * (z + 2) * (z + 2)
    //     //         + 3 * (x * x * y * y + x * x * z * z + y * y * z * z)
    //     //         + 6 * x * y * z - 10 * (x * x + y * y + z * z) + 22;
    //     // }
    //     // [](Vector3d point) {
    //     //     double x = point.x();
    //     //     double y = point.y();
    //     //     double z = point.z();
    //     //     return 2 * y * (y * y - 3 * x * x) * (1 - z * z)
    //     //     + (x * x + y * y) * (x * x + y * y)
    //     //     - (9 * z * z - 1) * (1 - z * z);
    //     // }
    //     // [](Vector3d point) {
    //     //     double x = point.x();
    //     //     double y = point.y();
    //     //     double z = point.z();
    //     //     return sin(x) * sin(y) - z;
    //     // }
    //     // [](Vector3d point) {
    //     //     double x = point.x();
    //     //     double y = point.y();
    //     //     double z = point.z();
    //     //     return z - std::max(1.8 * sin(10 * x * x) * sin(10 * y * y) / (1 + 2 * abs(x) * abs(y)), 0.);
    //     // }
    //     [](Vector3d point) {
    //         double x = point.x();
    //         double y = point.y();
    //         double z = point.z();
    //         return (x * x + y * y + z * z - 1) <= 0 ? -1 : 1;
    //     }
    // );
    // std::cout << "Done!" << std::endl;
    // MarchingCubes::MarchingCubesMeshGenerator generator;
    // std::cout << "Generating mesh..." << std::endl;
    // generator.generateMCMesh(triMesh, sdfcs, 0.);
    // std::cout << "Done!" << std::endl;

    // std::string outPath("../data/TSDF_Sphere.off");
    
    // std::cout << "Writing mesh..." << std::endl;
    // MeshLoader::writeTriangleMeshOff(outPath, triMesh);
    // std::cout << "Mesh written!" << std::endl;
    return 0;
}
