package com.ipoleksenko.sense;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.StrictMode;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity
{
    private int previousOrientation;

    @Override
    protected String[] getLibraries() {
        return new String[]{
            "SDL2",
            "SDL2_image",
            "SDL2_ttf",
            "SENSE_THE_GAME",
        };
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder(StrictMode.getVmPolicy())
            .detectLeakedClosableObjects()
            .build());

        previousOrientation = getResources().getConfiguration().orientation;

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onDestroy() {
        setRequestedOrientation(previousOrientation);
        super.onDestroy();
    }
}
