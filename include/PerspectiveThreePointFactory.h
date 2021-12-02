#ifndef PERSPECTIVETHREEPOINTFACTORY_H
#define PERSPECTIVETHREEPOINTFACTORY_H

#include "AbstractProjectionFactory.h"

class PerspectiveThreePointFactory : public AbstractProjectionFactory {
public:
    PerspectiveThreePointFactory();
    ~PerspectiveThreePointFactory();

    glm::mat4 createProjection() override;
};

#endif // PERSPECTIVETHREEPOINTFACTORY_H
