//
//  Input.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/26/21.
//

#ifndef Input_h
#define Input_h

#include <map>
#include <queue>

struct KeyEvent
{
    int key, action, mods;
    
    KeyEvent(int key, int action, int mods) : key(key), action(action), mods(mods) {};
};


class Input
{
    
public:
    std::map<int, bool> m_keyPress;
    std::queue<KeyEvent> m_unhandledKeys;
    
    static Input& instance()
    {
        static Input* instance = new Input();
        return *instance;
    }
};

#endif /* Input_h */
