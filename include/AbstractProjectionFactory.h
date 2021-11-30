#ifndef ABSTRACTPROJECTIONFACTORY_H
#define ABSTRACTPROJECTIONFACTORY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class AbstractProjectionFactory {
public:
    AbstractProjectionFactory();
    virtual ~AbstractProjectionFactory();

    virtual glm::mat4 createProjection() = 0;
};

#endif // ABSTRACTPROJECTIONFACTORY_H
