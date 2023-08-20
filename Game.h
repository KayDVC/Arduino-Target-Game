#pragma once
#ifndef GAMEFILE_H
#define GAMEFILE_H

// Arduino Libs
#include <Arduino.h>
#include <Array.h>
// #include <U8g2lib.h>
#include <U8x8lib.h>

// Custom Libs
#include "Types.h"
#include "stdint.h"
#include "PortAccess.h"

using Targets             = types::Targets;
using LEDs                = types::LEDs;
using EnaDis              = types::EnaDis;
using OutputPorts         = types::OutputPorts;
using InputPorts          = types::InputPorts;
using PortAccessInterface = port_access::PortAccessInterface;
using GameResult          = types::GameResult;

using namespace types;

namespace game { class GameInterface{

  public:
  // Constructor
  GameInterface();

  // Destructor
  ~GameInterface() = default;  


  //////////////////////////////////////////////////////////////////////////////
  /// @details    Configure and validate all necessary components for game.
  /// @note       Uses a series of processor delays, serial logging, and
  ///             display printing to allow visual confirmation.
  ///             "0%"   : LCD was configured sucessful.
  ///             "50%"  : System and neccessary variables are in their
  ///                              expected state.
  ///             "75%"  : Flash Tests has completed. Visual verification
  ///                      required. Note this only guarantees that SW-based
  ///                      output functionality works as expected.
  ///             "PASS" : Target input tests has been completed. Note this 
  ///                      only guarantees SW-based input functionality
  ///                      works as expected.
  ///             "FAIL" : Some system component is not in its expected state.
  ///                      Debugging required.
  //////////////////////////////////////////////////////////////////////////////
  void setupGame();

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Set/Update player score and target trackers.
  //////////////////////////////////////////////////////////////////////////////
  void runGame();

  
  //
  // Private Functions
  //
  private:
  //////////////////////////////////////////////////////////////////////////////
  /// @details    Configure LCD for use during game.
  /// @note       "0%" should be printed to lcd at the end of configuration.
  //////////////////////////////////////////////////////////////////////////////
  void setupLcd();

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Determine eligibility of target hit;
  /// @param[in]  t_hit - Hit Target Identifier.
  /// @param[in]  now - Time when parent `updateScore` called in ms.
  /// @return     Whether the required "cooldown" time has passed since
  ///             target last hit.
  //////////////////////////////////////////////////////////////////////////////
  bool validHit(Targets t_hit, unsigned long now);

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Update player score based on target "hit" and target's value.
  /// @param[in]  t_hit - Hit Target Identifier.
  //////////////////////////////////////////////////////////////////////////////
  void updateScore(Targets t_hit);

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Update player score and remaining time on display.
  /// @param[in]  update_score - Whether the LCD needs to update score.
  //////////////////////////////////////////////////////////////////////////////
  void updateDisplay(bool update_score);

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Post-game visual cues.
  /// @param[in]  res - Result of game based on "Win" and "Lose" criteria.
  /// @note       Forces system reset to play again.
  //////////////////////////////////////////////////////////////////////////////
  void endGame(GameResult res);

  //
  // Member Variables
  //

  // Score
  uint8_t player_score_;
  uint8_t target_value_;
  Array<unsigned long, types::TOTAL_TARGETS> last_hit_time_;

  // Timing
  bool start_game_;
  unsigned long start_time_;
  
  // Enhancements
  bool multiply_points_;
  uint8_t point_multiplier_;
  unsigned long bonus_time_start_;
  unsigned long bonus_time_end;

  // LCD
  U8X8_SH1106_128X64_NONAME_HW_I2C lcd_;                     // See: (https://github.com/olikraus/u8g2/wiki/u8x8setupcpp#sh1106-128x64_noname-1); Uses MUCH less dynamic mem.
  uint8_t start_pos_, offset_pos_, label_pos_, value_pos_;   // Positions for text-based LCD graphics.
  bool first_score_update_;

  // Port Access
  PortAccessInterface port_ifc_;


};} // namespace game
#endif