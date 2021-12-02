#include "PerspectiveThreePointFactory.h"

PerspectiveThreePointFactory::PerspectiveThreePointFactory()
{
    //ctor
}

PerspectiveThreePointFactory::~PerspectiveThreePointFactory()
{
    //dtor
}

glm::mat4 PerspectiveThreePointFactory::createProjection() {
    return glm::mat4(1.0f);
}
