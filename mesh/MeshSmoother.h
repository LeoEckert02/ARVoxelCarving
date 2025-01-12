#pragma once

#include <functional>
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#include "Mesh.h"

using namespace Eigen;

namespace Kernel {
    class Kernel {
    public:
        virtual double g(double delta) {
            return 1.;
        }
    };

    class CauchyKernel: public Kernel {
    public:
        double f_c;
        // Interpolation with local averaging
        // [0; 1], alpha * Cauchy + (1 - alpha) * LocalAvg
        double f_alpha;

        CauchyKernel(double c, double alpha):
        f_c(c),
        f_alpha(alpha)
        {}

        double g(double delta) override {
            return f_alpha / (1 + delta * delta / (f_c * f_c)) + (1 - f_alpha);
        }
    };

    class RCGaussianKernel: public Kernel {
    public:
        double f_c;
        // Interpolation with local averaging
        // [0; 1], alpha * RCGaussian + (1 - alpha) * LocalAvg
        double f_alpha;

        RCGaussianKernel(double c, double alpha):
        f_c(c),
        f_alpha(alpha)
        {}

        double g(double delta) override {
            double t = delta / f_c;
            return f_alpha * sqrt(0.5 / M_PI) * exp(-t * t / 2) + (1 - f_alpha);
        }
    };

    class LaplaceKernel: public Kernel {
    public:
        double f_c;
        // Interpolation with local averaging
        // [0; 1], alpha * Laplace + (1 - alpha) * LocalAvg
        double f_alpha;

        LaplaceKernel(double c, double alpha):
        f_c(c),
        f_alpha(alpha)
        {}
        
        double g(double delta) override {
            double t = delta / f_c;
            return f_alpha * 0.5 * exp(-abs(t)) + (1 - f_alpha);
        }
    };

    class RayleighKernel: public Kernel {
    public:
        double f_c;
        // Interpolation with local averaging
        // [0; 1], alpha * Rayleigh + (1 - alpha) * LocalAvg
        double f_alpha;

        RayleighKernel(double c, double alpha):
        f_c(c),
        f_alpha(alpha)
        {}

        double g(double delta) override {
            double t = delta / f_c;
            return f_alpha * exp(-t * t / 2) * t + (1 - f_alpha);
        }
    };
};

namespace Mesh {

    class MeshSmoother {
    private:
        // Smoothing Kernel
        std::shared_ptr<Kernel::Kernel> f_kernel;
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
            Kernel::Kernel* kernel,
            double rho
        ):
        f_kernel(kernel),
        f_rho(rho),
        f_neighbours()
        {}

        #pragma endregion

        #pragma region Getters and Setters

        const std::shared_ptr<Kernel::Kernel>& getKernel() const;

        double getRho() const;

        double getAlpha() const;

        void setKernel(Kernel::Kernel* kernel);

        void setRho(double rho);

        #pragma endregion

        double calculateDelta(Mesh::TriangleMesh& mesh, const std::vector<uint32_t>& neighbours, uint32_t id) const;

        void precalculateNeighbours(const Mesh::TriangleMesh &mesh);

        void smoothenMesh(Mesh::TriangleMesh& mesh, uint16_t iterations, bool wide = false) const;
    };

}