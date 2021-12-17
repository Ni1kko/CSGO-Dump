#pragma once

#include <string>

class c_menu
{
public:
    void start_draw ( );
    void end_draw ( );

    void initialize ( );
    void shutdown ( ) const;
    void show ( );
    void hide ( );
    void toggle ( );

    bool is_visible ( ) const
    {
        return _visible;
    }

private:
    bool _visible;
};

extern c_menu menu;
