#include "BoundingBox.h"

BoundingBox::BoundingBox(const Eigen::Vector3f& min, const Eigen::Vector3f& max)
    : minCorner(min), maxCorner(max) {}

bool BoundingBox::isInside(const Eigen::Vector3f& point) const {
    return (point.x() >= minCorner.x() && point.x() <= maxCorner.x()) &&
           (point.y() >= minCorner.y() && point.y() <= maxCorner.y()) &&
           (point.z() >= minCorner.z() && point.z() <= maxCorner.z());
}
