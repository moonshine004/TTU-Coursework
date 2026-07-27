#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

/*
 * Shared model used between TouchGFX screens.
 * Most playback data comes directly from main.c globals.
 */
class Model
{
public:
    Model();

    // connect the active presenter listener
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    // called every gui tick
    void tick();

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
