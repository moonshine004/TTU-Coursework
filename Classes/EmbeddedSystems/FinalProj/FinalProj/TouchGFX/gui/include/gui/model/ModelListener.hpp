#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

/*
 * Base listener class used by the presenters.
 */
class ModelListener
{
public:
    ModelListener() :
        model(0)
    {
    }

    virtual ~ModelListener()
    {
    }

    // attach the active model instance
    void bind(Model* m)
    {
        model = m;
    }

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
