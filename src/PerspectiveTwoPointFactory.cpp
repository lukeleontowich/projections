#include "PerspectiveTwoPointFactory.h"

PerspectiveTwoPointFactory::PerspectiveTwoPointFactory()
{
    //ctor
}

PerspectiveTwoPointFactory::~PerspectiveTwoPointFactory()
{
    //dtor
}

glm::mat4 PerspectiveTwoPointFactory::createProjection() {
    return glm::mat4(1.0f);
}
