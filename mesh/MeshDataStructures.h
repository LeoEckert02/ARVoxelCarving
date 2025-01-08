#pragma once

#include <Eigen/Dense>
#include <vector>

typedef Eigen::Matrix<unsigned char, 4, 1> Vector4uc;

using namespace Eigen;

namespace ID {

    class IdGenerator {
    private:

        uint32_t f_idGen;

    public:

        IdGenerator():
        f_idGen(0)
        {}

        uint32_t generateId() {
            return f_idGen++;
        }

        void reset() {
            f_idGen = 0;
        }
    };

}

namespace MDS {
    // Vertex data structure
    struct Vertex {
        // Vertice id
        uint32_t f_id;
        // Vertice coordinates
        Vector3d f_coordinates;

        // Vertex color
        Vector4uc f_color;

    #pragma region Constructors
        
        Vertex():
        f_id(0),
        f_coordinates(0., 0., 0.),
        f_color(255, 255, 255, 255)
        {}

        Vertex(const Vertex& other):
        f_id(other.f_id),
        f_coordinates(other.f_coordinates),
        f_color(other.f_color)
        {}

        Vertex(Vertex&& other):
        f_id(other.f_id),
        f_coordinates(std::move(other.f_coordinates)),
        f_color(std::move(other.f_color))
        {}

        Vertex(uint32_t id, const Vertex& other):
        f_id(id),
        f_coordinates(other.f_coordinates),
        f_color(other.f_color)
        {}

        Vertex(uint32_t id, Vertex&& other):
        f_id(id),
        f_coordinates(std::move(other.f_coordinates)),
        f_color(std::move(other.f_color))
        {}
        
        Vertex(uint32_t id, const Vector3d& coordinates, const Vector4uc& color = Vector4uc(255, 255, 255, 255)):
        f_id(id),
        f_coordinates(coordinates),
        f_color(color)
        {}

        Vertex(uint32_t id, Vector3d&& coordinates, Vector4uc&& color = Vector4uc(255, 255, 255, 255)):
        f_id(id),
        f_coordinates(std::move(coordinates)),
        f_color(std::move(color))
        {}

    #pragma endregion

    };

    // Triangle data structure
    struct Triangle {        
        // Triangle id
        uint32_t f_id;
        
        // Vertex ids
        uint32_t f_v1Id;
        uint32_t f_v2Id;
        uint32_t f_v3Id;

        #pragma region Constructors

        Triangle():
        f_id(0),
        f_v1Id(0),
        f_v2Id(0),
        f_v3Id(0)
        {}

        Triangle(const Triangle& other):
        f_id(other.f_id),
        f_v1Id(other.f_v1Id),
        f_v2Id(other.f_v2Id),
        f_v3Id(other.f_v3Id)
        {}

        Triangle(uint32_t id, const Triangle& other):
        f_id(id),
        f_v1Id(other.f_v1Id),
        f_v2Id(other.f_v2Id),
        f_v3Id(other.f_v3Id)
        {}

        Triangle(uint32_t id, uint32_t v1Id, uint32_t v2Id, uint32_t v3Id):
        f_id(id),
        f_v1Id(v1Id),
        f_v2Id(v2Id),
        f_v3Id(v3Id)
        {}

        #pragma endregion

        bool contains(uint32_t id) const {return f_v1Id == id || f_v2Id == id || f_v3Id == id;}
    };

}