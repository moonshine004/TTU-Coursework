#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() :
    modelListener(0)
{
}

void Model::tick()
{
    /*
     * GUI updates are mostly handled directly
     * inside the screen views for this project.
     */
}
