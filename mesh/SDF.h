#pragma once

#include <Eigen/Dense>
#include <cstdint>
#include <vector>
#include "../VoxelGrid.h"

using namespace Eigen;



namespace SDF {

    typedef std::function<double(Vector3d)> ScalarFunction3D;

    struct SDFCSample {
        // SDF smaple grid dimensions
        // Along Ox
        uint32_t f_n;
        // Along Oy
        uint32_t f_m;
        // Along Oz
        uint32_t f_p;

        // SDF sample grid origin point
        Vector3d f_origin;
        // SDF sample grid size
        Vector3d f_size;

        // SDF sample grid steps
        double f_dx;
        double f_dy;
        double f_dz;

        // SDF sample values
        std::vector<std::vector<std::vector<double>>> values;
        // SDF sample colors
        std::vector<std::vector<std::vector<Vector4uc>>> colors;

        SDFCSample():
        f_n(0),
        f_m(0),
        f_p(0),
        f_origin(0., 0., 0.),
        f_size(0., 0., 0.),
        f_dx(0.),
        f_dy(0.),
        f_dz(0.),
        values(),
        colors()
        {}

        SDFCSample(
            uint32_t n,
            uint32_t m,
            uint32_t p,
            const Vector3d& origin,
            const Vector3d& size
        ):
        f_n(n),
        f_m(m),
        f_p(p),
        f_origin(origin),
        f_size(size)
        {
            calculateGridSteps();
            values.resize(n);
            colors.resize(n);
            for (unsigned i = 0; i < n; i++) {
                values.at(i).resize(m);
                colors.at(i).resize(m);
                for (unsigned j = 0; j < m; j++) {
                    values.at(i).at(j).resize(p);
                    colors.at(i).at(j).resize(p);
                    for (unsigned k = 0; k < p; k++) {
                        colors.at(i).at(j).at(k) = Vector4uc(255, 255, 255, 255);
                    }
                }
            }
        }

        void calculateGridSteps() {
            f_dx = f_size.x() / (double)(f_n - 1);
            f_dy = f_size.y() / (double)(f_m - 1);
            f_dz = f_size.z() / (double)(f_p - 1);
        }

        Vector3d getPoint(uint32_t i, uint32_t j, uint32_t k) const {
            if (i >= f_n || j >= f_m || k >= f_p) return f_origin;
            return f_origin + Vector3d(f_dx * i, f_dy * j, f_dz * k);
        }

        double getValue(uint32_t i, uint32_t j, uint32_t k) const {
            return values.at(i).at(j).at(k);
        }

        Vector4uc getColor(uint32_t i, uint32_t j, uint32_t k) const {
            return colors.at(i).at(j).at(k);
        }

        double& value(uint32_t i, uint32_t j, uint32_t k) {
            return values.at(i).at(j).at(k);
        }

        Vector4uc& color(uint32_t i, uint32_t j, uint32_t k) {
            return colors.at(i).at(j).at(k);
        }

        void fillSDF(const ScalarFunction3D& field) {
            for (uint32_t i = 0; i < f_n; i++) {
                for (uint32_t j = 0; j < f_m; j++) {
                    for (uint32_t k = 0; k < f_p; k++) {
                        value(i, j, k) = field(getPoint(i, j, k));
                    }
                }
            }
        }

        static SDFCSample convertVoxelGridToSDFC(const VoxelGrid& voxelGrid) {
            uint32_t n, m, p;
            n = voxelGrid.getVoxels().size();
            m = voxelGrid.getVoxels().at(0).size();
            p = voxelGrid.getVoxels().at(0).at(0).size();
            Vector3d origin = voxelGrid.getBoundingBox().minCorner.cast<double>();
            Vector3d size = (voxelGrid.getBoundingBox().maxCorner - voxelGrid.getBoundingBox().minCorner).cast<double>();
            SDFCSample sdfcs(n, m, p, origin, size);
            for (uint32_t i = 0; i < n; i++) {
                for (uint32_t j = 0; j < m; j++) {
                    for (uint32_t k = 0; k < p; k++) {
                        bool occupied = voxelGrid.isVoxelOccupied(i, j, k);
                        sdfcs.value(i, j, k) = occupied ? -1. : 1.;
                        // TODO: Add color copy if necessary
                        // if (occupied) {
                        //     sdfcs.color(i, j, k) = voxelGrid.getColors().at(i).at(j).at(k);
                        // }
                    }
                }
            }
            return sdfcs;
        }
    };

}