package com.anarchy.sense;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity
{
    @Override
    protected String[] getLibraries() {
        return new String[]{
            "SDL2",
            "SDL2_image",
            "SDL2_ttf",
            "SENSE_THE_GAME",
        };
    }
}
