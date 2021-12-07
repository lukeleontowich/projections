#ifndef PERSPECTIVEONEPOINTFACTORY_H
#define PERSPECTIVEONEPOINTFACTORY_H

#include "AbstractProjectionFactory.h"

class PerspectiveOnePointFactory : public AbstractProjectionFactory {
private:
    //  Variables needed for a perspective one point projection

    //  field of view y: vertical viewing angle
    float fovy;

    //  aspect ratio
    float aspect;


public:
    PerspectiveOnePointFactory(const float& fv, const float& asp, const float& n, const float& f);
    ~PerspectiveOnePointFactory();

    virtual glm::mat4 createProjection() override;

    void print() override;
};

#endif // PERSPECTIVEONEPOINTFACTORY_H
