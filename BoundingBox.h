#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <Eigen/Dense>

struct BoundingBox {
    Eigen::Vector3f minCorner;
    Eigen::Vector3f maxCorner;

    BoundingBox(const Eigen::Vector3f& min, const Eigen::Vector3f& max);
    bool isInside(const Eigen::Vector3f& point) const;
};

#endif // BOUNDING_BOX_H
