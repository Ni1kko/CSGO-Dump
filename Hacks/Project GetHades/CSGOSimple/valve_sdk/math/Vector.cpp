#include "Vector.hpp"

void Vector::vector_cross_product ( const Vector& a, const Vector& b, Vector& result ) const
{
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
}
