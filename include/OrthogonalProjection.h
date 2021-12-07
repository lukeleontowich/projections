#ifndef ORTHOGONALPROJECTION_H
#define ORTHOGONALPROJECTION_H

#include "AbstractProjectionFactory.h"


class OrthogonalProjection : public AbstractProjectionFactory {
public:
    OrthogonalProjection(const float& l, const float& r,
                         const float& b, const float& t,
                         const float& n, const float& f);
    ~OrthogonalProjection();

    glm::mat4 createProjection() override;

    void print() override;
};

#endif // ORTHOGONALPROJECTION_H
