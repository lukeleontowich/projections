#include "PerspectiveProjection.h"

PerspectiveProjection::PerspectiveProjection(const float& l, const float& r,
                                             const float& b, const float& t,
                                             const float& n, const float& f,
                                             const float& fovy,
                                             const float& aspect) {
    left = l;
    right = r;
    bottom = b;
    top = t;
    near = n;
    far = f;
}

PerspectiveProjection::~PerspectiveProjection() {}

glm::mat4 PerspectiveProjection::createProjection() {
    //  Initialize matrix to identity matrix
    glm::mat4 m(1.0f);

    m[0][0] = (2.0f * near) / (right - left);
    m[2][0] = (right + left) / (right - left);
    m[1][1] = (2.0f * near) / (top - bottom);
    m[2][1] = (top + bottom) / (top -bottom);
    m[2][2] = -(far + near) / (far - near);
    m[3][2] = (-2.0f * far * near) / (far - near);
    m[2][3] = -1.0f;

    return m;
}

void PerspectiveProjection::print() {
    std::cout << "PERSPECTIVE PROJECTION DONT USE\n";
}
