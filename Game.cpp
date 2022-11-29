#pragma once
#ifndef GAMEFILE_CPP
#define GAMEFILE_CPP

#include "Game.h"

/// @todo update as needed.
constexpr uint16_t GAME_DURATION = 60 * SECOND;
constexpr uint16_t INIT_WAIT = 500;         // in ms.

namespace game {

  GameInterface::GameInterface():
    player_score_(0),
    start_game_(false),
    start_time_(0),
    min_points(1),
    /// @todo update as necessary.
    max_points(20),
    lcd_(U8X8_PIN_NONE),            // See: (https://github.com/olikraus/u8g2/wiki/u8x8setupcpp#wiring)
    x_pos_(0),
    y_pos_(15),
    port_ifc_()
  {
    // do nothing.
  }

  void GameInterface::setupGame(){

    
    // Begin LCD configuration.
    setupLcd();

    // Randomize targets values.
    randomizePoints();

    // Verify Expected System State.
    bool ready = (
      (player_score_ == 0)      &&
      (start_game_   == false)  &&
      (port_ifc_.ioSet())      
    );

    lcd_.clear();    
    if (ready){
       lcd_.print(F(" 50% "));
    }else{
       lcd_.print(F(" FAIL "));
    }
    delay(INIT_WAIT);

    // Visual verification required.
    port_ifc_.flashLEDs();
    lcd_.clear();
    lcd_.print(F(" 75% "));
    delay(INIT_WAIT);


    // External action required.
    port_ifc_.verifyTargets();
    lcd_.clear();
    lcd_.print(F(" PASS "));
    delay(INIT_WAIT);

    Serial.println(F("--------- All Verifications Completed ---------"));
    Serial.println(F(""));
    Serial.println(F(""));

  }

  void GameInterface::runGame(){

    Targets target;

    // Wait for start button to be pressed.
    do{
      start_game_ = port_ifc_.sampleStartButton();

      /// @todo consider different approach. Starts game once target "hit".
      // start_game_ = port_ifc.targetHit(t);
    } while (!start_game_);

    // Once start signal received, start game and store total program run time.
    start_time_ = millis();

    // Compares program run time at evaluation vs game start.
    // Simple timing may lead to discrepencies of +50 ms between games. Not significant in this case.
    while(start_time_ + GAME_DURATION > millis()){

      // Update score and display if valid target "hit" detected.
      if(port_ifc_.targetHit(target)){
        updateScore(target);
      }

      // Loop until the time limit has been reached.
    }

    // Post-game visual cues. Forces system reset to play again.
    endGame();
    
  }

// Private functions.
  void GameInterface::setupLcd(){
    Serial.println(F("--------- Begin LCD Configuration ---------"));
    // Configure LCD with default library params.
    lcd_.begin();



    // Set font for initializations tasks.
    // Reference: https://github.com/olikraus/u8g2/wiki/fntgrpopengameart#victoriabold8
    lcd_.setFont(u8x8_font_victoriabold8_r);

    lcd_.print(F(" 0% "));
    
    Serial.println(F("Configuration completed."));
    Serial.println(F(""));
    Serial.println(F(""));

    // Create time for visual verification.
    delay(INIT_WAIT);
  }

  void GameInterface::randomizePoints(){

    // Assign each target a random point value from ranging from min_points -> max_points.
    for (uint8_t i = 0; i< TOTAL_TARGETS; i++){
      target_values_[i] = random(min_points, max_points+1);
    }

    // // Clear lcd contents.
    // lcd_.clear();
     lcd_.print(F(" 25% "));
    // Create time for visual verification.
    delay(INIT_WAIT);

  }

  void GameInterface::updateScore(Targets t_hit){
    // Update stored player score by target's value. Sanity check.
    player_score_ += target_values_[static_cast<uint8_t>(t_hit)];
    // Refresh display.
    updateDisplay();
  }

  void GameInterface::updateDisplay(){
    /// @todo implement.
  }

  void GameInterface::endGame(){
    /// @todo implement.

    do {
      // absolutely nothing. :)
    } while (1);
  }

} // namespace game
#endif