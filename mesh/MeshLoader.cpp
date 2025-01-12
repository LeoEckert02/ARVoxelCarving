#include "MeshLoader.h"

bool MeshLoader::loadTriangleMeshOff(const std::string& path, Mesh::TriangleMesh &mesh) {
    // Clear current mesh
    mesh.clear();

    // Open file
    std::ifstream ifStream(path);

    if (!ifStream.is_open()) {
        throw std::invalid_argument("Failed to open file.");
        return false;
    }

    // Read header
    std::string header;
    ifStream >> header;

    uint32_t vCount = 0;
    uint32_t tCount = 0;
    uint32_t eCount = 0;

    ifStream >> vCount;
    ifStream >> tCount;
    ifStream >> eCount;

    mesh.reserve(vCount, tCount);

    // Read vertices
    // If color information is provided
    if (header.compare("COFF") == 0) {
        // Start for cycle for number of vertices
        for (unsigned i = 0; i < vCount; i++) {
            // Read coordinates
            Vector3d coordinates;
            ifStream >> coordinates.x();
            ifStream >> coordinates.y();
            ifStream >> coordinates.z();

            // Read color
            Vector4i colorI;
            ifStream >> colorI.x();
            ifStream >> colorI.y();
            ifStream >> colorI.z();
            ifStream >> colorI.w();

            Vector4uc color(
                (unsigned char)colorI.x(),
                (unsigned char)colorI.y(),
                (unsigned char)colorI.z(),
                (unsigned char)colorI.w()
            );
            // Add vertex
            mesh.addVertex(std::move(coordinates), std::move(color));
        }
    }
    else if (header.compare("OFF") == 0) {
        // Start for cycle for number of vertices
        for (unsigned i = 0; i < vCount; i++) {
            Vector3d coordinates;
            // Read coordinates
            ifStream >> coordinates.x();
            ifStream >> coordinates.y();
            ifStream >> coordinates.z();

            // Add vertex
            mesh.addVertex(std::move(coordinates));
        }
    }
    else if (header.compare("OFFTC") == 0) {
        // Start for cycle for number of vertices
        for (unsigned i = 0; i < vCount; i++) {
            Vector3d coordinates;
            // Read coordinates
            ifStream >> coordinates.x();
            ifStream >> coordinates.y();
            ifStream >> coordinates.z();

            // Add vertex
            mesh.addVertex(std::move(coordinates));
        }
    }
    else {
        throw std::invalid_argument("Invalid file structure: wrong .off header.");
        return false;
    }

    // Read triangles
    for (unsigned i = 0; i < tCount; i++) {
        // Read number of vertices per face
        uint32_t m = 0;
        ifStream >> m;

        if (m != 3) {
            throw std::invalid_argument("Invalid file structure: file contains non-triangle faces.");
            return false;
        }
        // Read vertex indicies
        uint32_t v1Id = 0;
        uint32_t v2Id = 0;
        uint32_t v3Id = 0;

        ifStream >> v1Id;
        ifStream >> v2Id;
        ifStream >> v3Id;
        // Add triangle
        mesh.addTriangle(v1Id, v2Id, v3Id);

        if (header.compare("OFFTC") == 0) {
            ifStream >> v1Id;
            ifStream >> v2Id;
            ifStream >> v3Id;
        }
    }

    return true;
}

bool MeshLoader::writeTriangleMeshOff(const std::string& path, Mesh::TriangleMesh &mesh) {
    // Open file
    std::ofstream ofStream(path);

    if (!ofStream.is_open()) {
        throw std::invalid_argument("Failed to open file.");
        return false;
    }

    // Write header
    ofStream << "COFF" << std::endl;
    ofStream 
        << mesh.getVertexCount() << " "
        << mesh.getTriangleCount() << " "
        << "0" << std::endl;
    
    // Write vertices
    for (
        std::vector<MDS::Vertex>::const_iterator iter = mesh.getVertices().begin();
        iter != mesh.getVertices().end();
        iter++
    ) {
        // Write coordinates
        ofStream
            << iter->f_coordinates.x() << " "
            << iter->f_coordinates.y() << " "
            << iter->f_coordinates.z() << " ";
        
        // Write color
        ofStream 
            << (unsigned)iter->f_color.x() << " "
            << (unsigned)iter->f_color.y() << " "
            << (unsigned)iter->f_color.z() << " "
            << (unsigned)iter->f_color.w() << std::endl;
    }

    // Write triangles
    for (
        std::unordered_map<uint32_t, MDS::Triangle>::const_iterator iter = mesh.getTriangles().begin();
        iter != mesh.getTriangles().end();
        iter++
    ) {
        // Write 3 as number of vertices
        ofStream << "3 ";
        // Write vertex indicies
        ofStream
            << iter->second.f_v1Id << " "
            << iter->second.f_v2Id << " "
            << iter->second.f_v3Id << std::endl;
    }

    // Close file
    ofStream.close();

    return true;
}
