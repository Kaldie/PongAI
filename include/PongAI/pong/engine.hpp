#ifndef __ENGINE_H__
#define __ENGINE_H__

class Engine {

    public:
        void progress(GameState* state);
    
    private:
        void processActions(GameState* state);
        Line2D calculateBallTrajactory(GameState* state);
        
}


#endif // __ENGINE_H__