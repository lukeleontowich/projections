#ifndef PERSPECTIVEONEPOINTFACTORY_H
#define PERSPECTIVEONEPOINTFACTORY_H

#include "AbstractProjectionFactory.h"

class PerspectiveOnePointFactory : public AbstractProjectionFactory {
public:
    PerspectiveOnePointFactory();
    ~PerspectiveOnePointFactory();

    glm::mat4 createProjection() override;
};

#endif // PERSPECTIVEONEPOINTFACTORY_H
