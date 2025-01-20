#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Mesh.h"

class MeshLoader {
public:

    static bool loadTriangleMeshOff(const std::string& path, Mesh::TriangleMesh& mesh);

    static bool writeTriangleMeshOff(const std::string& path, Mesh::TriangleMesh& mesh);

};