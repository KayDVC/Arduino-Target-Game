#pragma once
#ifndef GAMEFILE_CPP
#define GAMEFILE_CPP

#include "Game.h"
#include "math.h"
#include <EEPROM.h>

constexpr uint16_t GAME_DURATION = 60 * SECOND;
constexpr uint16_t INIT_WAIT = 500;         // in ms.
constexpr uint8_t  WIN_SCORE = 25;

namespace game {

  GameInterface::GameInterface():
    player_score_(0),
    start_game_(false),
    start_time_(0),
    target_value_(5),
    lcd_(U8X8_PIN_NONE),              // See: (https://github.com/olikraus/u8g2/wiki/u8x8setupcpp#wiring)
    start_pos_(0),                
    offset_pos_(3),                   // 3*Character_Width pixels from left-most pixels of line.
    label_pos_(0),                    // Top line of lcd.
    value_pos_(2),                    // Allows for "Double Spacing" effect.
    first_score_update_(false),
    multiply_points_(false),
    point_multiplier_(2),
    bonus_time_start_(40*SECOND),     // Bonus time starts 40 seconds after game begins.
    bonus_time_end(50*SECOND),        // Bonus time ends 10 seconds after it begins.
    port_ifc_()
  {
    // Ensure all targets increment player score on first hit.
    last_hit_time_.fill(0);
  }

  void GameInterface::setupGame(){

    
    // Begin LCD configuration.
    setupLcd();

    // Verify Expected System State.
    bool ready = (
      (player_score_ == 0)        &&
      (start_game_   == false)    &&
      (multiply_points_ == false) &&
      (target_value_ == 5)        &&
      (point_multiplier_ == 2)    &&
      (port_ifc_.ioSet())      
    );

    lcd_.setCursor(offset_pos_, value_pos_);  
    if (ready){
       lcd_.print(F("50%"));
    }else{
       lcd_.print(F("FAIL"));
    }
    delay(INIT_WAIT);

    // Visual verification required.
    port_ifc_.flashLEDs();
    lcd_.setCursor(offset_pos_, value_pos_);
    lcd_.print(F("75%"));
    delay(INIT_WAIT);


    // External action required.
    port_ifc_.verifyTargets();
    lcd_.setCursor(offset_pos_, value_pos_);
    lcd_.print(F("PASS"));
    delay(INIT_WAIT);

    Serial.println(F("--------- All Verifications Completed ---------"));
    Serial.println(F(""));
    Serial.println(F(""));

  }

  void GameInterface::runGame(){

    Targets target;
    // Setup Stuff.
    updateDisplay(true);

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
    do{

      // Update score and display if valid target "hit" detected.
      if(port_ifc_.targetHit(target)){
        updateScore(target);
      }

      // Loop until the time limit has been reached.
    } while(start_time_ + GAME_DURATION > millis());

    // Check if player has scored necessary points to win game.
    if(player_score_ >= WIN_SCORE){
      endGame(GameResult::Win);
    }
    // Player has not scored necessary points to win game within the time limit.
    endGame(GameResult::Lose);
    
  }

// Private functions.
  void GameInterface::setupLcd(){
    Serial.println(F("--------- Begin LCD Configuration ---------"));
    // Configure LCD with default library params.
    lcd_.begin();
    
    // Set font for initializations tasks.
    // Reference: https://github.com/olikraus/u8g2/wiki/fntgrpopengameart#victoriabold8
    lcd_.setFont(u8x8_font_victoriabold8_r);

    lcd_.setCursor(start_pos_, label_pos_);
    lcd_.print(F("INITIALIZE:"));

    lcd_.setCursor(offset_pos_, value_pos_);
    lcd_.print(F("0%"));
    
    Serial.println(F("Configuration completed."));
    Serial.println(F(""));
    Serial.println(F(""));

    // Create time for visual verification.
    delay(INIT_WAIT);
  }

  bool GameInterface::validHit(Targets t_hit, unsigned long now){
    // Define cooldown period for targets.
    unsigned long min_cooldown = 3 * SECOND;
    uint8_t t_index = static_cast<uint8_t>(t_hit);
    

    // Check recorded time of last hit and compare to now.
    if ((last_hit_time_[t_index] - now ) >= min_cooldown){
      // Update last hit time for target.
      last_hit_time_[t_index] = now;
      return true;
    }

    return false;
  }

  void GameInterface::updateScore(Targets t_hit){
    
    // Determine if player can earn double points.
    unsigned long now = millis();    
    if (now - start_game_ >= bonus_time_start_ && multiply_points_ == false){
      multiply_points_ = true;
    }else if (now - start_game_ >= bonus_time_end && multiply_points_ == true) {
      multiply_points_ = false;
    }

    // Update stored player score by target's value if minimum
    // cooldown period has passed since target last hit.
    if(validHit(t_hit, now)){
      if(multiply_points_){
        player_score_ += (target_value_ * point_multiplier_);
      }else{
        player_score_+= target_value_;
      }
      // Refresh display.
      updateDisplay(true);
    }
  }

  void GameInterface::updateDisplay(bool update_score){

    // Define score/time-specific x,y positioning vars.
    uint8_t time_label_x_y = start_pos_;                      // Left-Most Pixel.
    uint8_t time_value_x   = offset_pos_;                     // 3 character_width offset.
    uint8_t time_value_y   = value_pos_;                      // Line 2.
    uint8_t separator_x    = offset_pos_ - 1;
    uint8_t separator_y    = time_value_y + 1;                // Line 3.
    uint8_t score_label_x  = start_pos_;                      // Left-Most Pixel.
    uint8_t score_label_y  = value_pos_*2;                    // Line 4.
    uint8_t score_value_x  = offset_pos_;                     // 3 character_width offset.
    uint8_t score_value_y  = value_pos_*2;                    // Line 6.
    uint8_t fixed_width    = 2;                               // Of the format "xx"
    uint8_t suffix_pos     = offset_pos_ + (fixed_width) + 1; // Add space between val and suffix.

    // Create labels and divider for all values.
    if(!first_score_update_){
      lcd_.clear();
      lcd_.setCursor(time_label_x_y, time_label_x_y);
      lcd_.print(F("Time Left:"));
      lcd_.setCursor(suffix_pos, time_value_y);
      lcd_.print(F("secs"));
      lcd_.setCursor(separator_x, separator_y);
      lcd_.print(F("--------"));
      lcd_.setCursor(score_label_x, score_label_y);
      lcd_.print(F("SCORE:"));
      lcd_.setCursor(suffix_pos, score_value_y);
      lcd_.print(F("pnts"));

      first_score_update_ = true;
    }
    
    // Update remaining time.
    lcd_.setCursor(time_value_x, time_value_y);
    uint8_t r_time = (start_time_ == 0) ? 60 : floor((millis()-start_time_)/SECOND);
    lcd_.print(u8x8_u8toa(r_time, fixed_width));

    // Update score as necessary.
    if(update_score){
      lcd_.setCursor(score_value_x, score_value_y);
      lcd_.print(u8x8_u8toa(player_score_, fixed_width));
    };
    
  }

  void GameInterface::endGame(GameResult res){
    lcd_.clear();
    uint8_t message_pos_ = value_pos_*2;
    uint8_t high_score_pos_ = value_pos_ * 3;
    uint8_t fixed_width  = 3;             // Of the format "xx"
    uint8_t nv_mem_addr = 0;              // Address in EEPROM of non-volatile memory. Holds higest score.

    lcd_.setCursor(start_pos_, label_pos_);
    lcd_.print(F("Final Score: "));
    lcd_.setCursor(offset_pos_, value_pos_);
    lcd_.print(u8x8_u8toa(player_score_, fixed_width));

    lcd_.setCursor(start_pos_, message_pos_);
    if (res == GameResult::Win){
      lcd_.print(F("YOU WIN!!"));
    }else{
      // Let's be nice for the kiddos.
      lcd_.print(F("Great Try!"));
    }

    /// @todo implement more robust leaderboard system.
    size_t highest_score = EEPROM.read(nv_mem_addr);

    lcd_.setCursor(start_pos_, high_score_pos_);
    if(highest_score < player_score_ || highest_score == 0 ){
      lcd_.print(F("New High Score!"));
      EEPROM.write(nv_mem_addr, player_score_);
    }else{
      lcd_.print(F("High Score: "));
      lcd_.print(player_score_);
    }

    do {
      // absolutely nothing. Force reset. :)
      port_ifc_.flashLEDs();
    } while (1);
  }

} // namespace game
#endif