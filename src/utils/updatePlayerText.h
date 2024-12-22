#pragma once


#ifndef finalCheckpoint_H
#define finalCheckpoint_H
extern int finalCheckpoint;
#endif 

void updatePlayerText(Player& player, TextRenderer& text) {
    switch (player.getPlayerY()) {
    case 0:
        // Initial message
        text.setText("Text instructions");
        text.restoreOriginalPosition(); // Reset text position to its original location
        text.setAnimated(false);       // Disable animation
        break;
    case 1:
        // Start fading out animation
        text.setAnimated(true);
        text.startAnimation(false);
        break;

    case 1000:
        // Display a philosophical message
        text.setText("You opened your eyes, but did you see anything new?");
        text.centerTextOnScreen();     // Center the text on the screen
        text.startAnimation(true);     // Start fading in animation
        break;
    case 1500:
        // Stop animation
        text.startAnimation(false);
        break;

    case 2000:
        text.setText("Every day is like the one before, yet you search for differences");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 2500:
        text.startAnimation(false);
        break;

    case 3000:
        text.setText("People chase dreams, but who said dreams hold value?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 3500:
        text.startAnimation(false);
        break;

    case 4000:
        text.setText("How many questions have you asked, and how many answers have you received?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 4500:
        text.startAnimation(false);
        break;

    case 5000:
        text.setText("The world moves in circles, but where is its beginning and where is its end?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 5500:
        text.startAnimation(false);
        break;

    case 6000:
        text.setText("You strive to find a purpose, but what is it even for?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 6500:
        text.startAnimation(false);
        break;

    case 7000:
        text.setText("Stones lie on the ground for millennia, yet you live for only a moment.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 7500:
        text.startAnimation(false);
        break;

    case 8000:
        text.setText("What matters more: your thoughts or the sound of the wind?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 8500:
        text.startAnimation(false);
        break;

    case 9000:
        text.setText("The history of the world is full of heroes, but no one remembers them.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 9500:
        text.startAnimation(false);
        break;

    case 10000:
        text.setText("If something disappears tomorrow, what changes today?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 10500:
        text.startAnimation(false);
        break;

    case 11000:
        text.setText("The sun rises every day, but not for you.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 11500:
        text.startAnimation(false);
        break;

    case 12000:
        text.setText("Your heart beats, but who cares?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 12500:
        text.startAnimation(false);
        break;

    case 13000:
        text.setText("Everything you build will one day turn to dust.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 13500:
        text.startAnimation(false);
        break;

    case 14000:
        text.setText("You search for truth, but in this world, there is no law of truth.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 14500:
        text.startAnimation(false);
        break;

    case 15000:
        text.setText("Who created this world if even they had no plan?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 15500:
        text.startAnimation(false);
        break;

    case 16000:
        text.setText("Joy and pain alternate, but both eventually fade.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 16500:
        text.startAnimation(false);
        break;

    case 17000:
        text.setText("You want to be needed, but by whom?");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 17500:
        text.startAnimation(false);
        break;

    case 18000:
        text.setText("The stars shine, but not to show you the way.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 18500:
        text.startAnimation(false);
        break;

    case 19000:
        text.setText("Eternity is a word that both frightens and frees.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 19500:
        text.startAnimation(false);
        break;

    case 20000:
        text.setText("In this chaos, you seek meaning, but chaos demands no explanation.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case 20500:
        text.startAnimation(false);
        break;

    case finalCheckpoint:
        text.setText("THE UNIVERSE DOESN'T MAKE SENSE.");
        text.centerTextOnScreen();
        text.startAnimation(true);
        break;
    case finalCheckpoint + 500:
        // End the game
        window.running = false;
        break;
    }
}
