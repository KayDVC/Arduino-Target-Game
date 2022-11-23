#pragma once
#ifndef MAIN
#define MAIN

#include "Game.h"

using GameInterface = game::GameInterface;

// Game Logic
GameInterface game_ifc_;

void setup() {
  // Setup
  // Utilizes visual and software verification.
  game_ifc_.setupGame();

  // Debugging functionality.
  Serial.begin(9600);

}

void loop() {
  // Start Game
  game_ifc_.runGame();

}

#endif
