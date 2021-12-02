#ifndef PERSPECTIVETWOPOINTFACTORY_H
#define PERSPECTIVETWOPOINTFACTORY_H

#include "AbstractProjectionFactory.h"

class PerspectiveTwoPointFactory : public AbstractProjectionFactory {
public:
    PerspectiveTwoPointFactory();
    ~PerspectiveTwoPointFactory();

    glm::mat4 createProjection() override;
};

#endif // PERSPECTIVETWOPOINTFACTORY_H
