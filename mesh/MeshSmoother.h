#pragma once

#include <functional>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

#include "Mesh.h"

using namespace Eigen;

namespace Mesh {

    typedef std::function<double(double)> Kernel;

    class MeshSmoother {
    private:
        // Smoothing Kernel
        Kernel f_kernel;
        // Kernel normalizer value
        double f_rho;
        // Neighbour precalculated data
        std::vector<std::vector<uint32_t>> f_neighbours;
    public:
        
        #pragma region Constructors

        MeshSmoother():
        f_kernel(nullptr),
        f_rho(1.),
        f_neighbours()
        {}

        MeshSmoother(
            const Kernel& kernel,
            double rho
        ):
        f_kernel(kernel),
        f_rho(rho),
        f_neighbours()
        {}

        MeshSmoother(
            Kernel&& kernel,
            double rho
        ):
        f_kernel(std::move(kernel)),
        f_rho(rho),
        f_neighbours()
        {}

        #pragma endregion

        #pragma region Getters and Setters

        const Kernel& getKernel() const;

        double getRho() const;

        double getAlpha() const;

        void setKernel(const Kernel& kernel);

        void setKernel(Kernel&& kernel);

        void setRho(double rho);

        #pragma endregion

        double calculateDelta(Mesh::TriangleMesh& mesh, uint32_t id) const;

        void precalculateNeighbours(const Mesh::TriangleMesh &mesh);

        void smoothenMesh(Mesh::TriangleMesh& mesh, uint16_t iterations) const;
    };

}