#pragma once
#ifndef GAMEFILE_CPP
#define GAMEFILE_CPP

#include "Game.h"

// Constants
constexpr auto NONE_GLYPH    = 0x193;   // 0% Glyph.
constexpr auto SOME_GLYPH    = 0x198;   // 25% Glyph.
constexpr auto HALF_GLYPH    = 0x199;   // 50% Glyph.
constexpr auto MOST_GLYPH    = 0x19b;   // 75% Glyph.
constexpr auto FULL_GLYPH    = 0x195;   // 100% Glyph.
constexpr auto FAILURE_GLYPH = 0x1e3;   // Skull and Crossbones Glyph.
/// @todo update as needed.
constexpr auto GAME_DURATION = 60 * SECOND;
constexpr auto INIT_WAIT = 500;         // in ms.

namespace game {

  GameInterface::GameInterface():
    player_score_(0),
    start_game_(false),
    start_time_(0),
    min_points(1),
    /// @todo update as necessary.
    max_points(20),
    lcd_(U8G2_R0, U8X8_PIN_NONE),  // See: (https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#rotation), (https://github.com/olikraus/u8g2/wiki/u8x8setupcpp#wiring)
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
      // Draw 50% glyph.
      lcd_.drawGlyphX2(x_pos_, y_pos_, HALF_GLYPH);
    }else{
      // Draw skull and crossbones glyph.
      lcd_.drawGlyphX2(x_pos_, y_pos_, FAILURE_GLYPH);
    }
    delay(INIT_WAIT);

    // Visual verification required.
    port_ifc_.flashLEDs();
    lcd_.clear();
    // Draw 75% glyph.
    lcd_.drawGlyphX2(x_pos_, y_pos_, MOST_GLYPH);
    delay(INIT_WAIT);


    // External action required.
    port_ifc_.verifyTargets();
    lcd_.clear();
    // Draw 100% glyph.
    lcd_.drawGlyphX2(x_pos_, y_pos_, FULL_GLYPH);
    delay(INIT_WAIT);

    Serial.println("--------- All Verifications Completed ---------");
    Serial.println("");
    Serial.println("");

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
    Serial.println("--------- Begin LCD Configuration ---------");
    // Configure LCD with default library params.
    lcd_.begin();

    // Set font for initializations tasks.
    // Provided by: (https://intercom.help/streamlinehq/en/articles/5354403-how-to-create-an-attribution-link)
    // Reference: https://github.com/olikraus/u8g2/wiki/fntgrpstreamline
    lcd_.setFont(u8g2_font_streamline_all_t);

    // Draw 0% glyph.
    lcd_.drawGlyphX2(x_pos_, y_pos_, NONE_GLYPH);
    
    Serial.println("Configuration completed.");
    Serial.println("");
    Serial.println("");

    // Create time for visual verification.
    delay(INIT_WAIT);
  }

  void GameInterface::randomizePoints(){

    // Assign each target a random point value from ranging from min_points -> max_points.
    for (auto i = 0; i< TOTAL_TARGETS; i++){
      target_values_[i] = random(min_points, max_points+1);
    }

    // Clear lcd contents.
    lcd_.clear();
    // Draw 25% glyph.
    lcd_.drawGlyphX2(x_pos_, y_pos_, SOME_GLYPH);
    // Create time for visual verification.
    delay(INIT_WAIT);

  }

  void GameInterface::updateScore(Targets t_hit){
    // Update stored player score by target's value.

    if(t_hit != Targets::TOTAL)
    { 
      player_score_ += target_values_[static_cast<uint8_t>(t_hit)];
      // Refresh display.
      updateDisplay();
    }
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