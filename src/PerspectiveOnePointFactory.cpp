#include "PerspectiveOnePointFactory.h"

PerspectiveOnePointFactory::PerspectiveOnePointFactory()
{
    //ctor
}

PerspectiveOnePointFactory::~PerspectiveOnePointFactory()
{
    //dtor
}

glm::mat4 PerspectiveOnePointFactory::createProjection() {
    return glm::mat4(1.0f);
}
