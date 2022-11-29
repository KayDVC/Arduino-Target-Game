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
  ///             "25%"  : Target values have been randomly assigned.
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

  ///@todo add descriptions and params.
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
  /// @details    Assign each target a value from min_points to max_points.
  /// @note       Value will be used throughout game update player score.
  /// @note       "25%" should be printed to lcd at the end of configuration.
  //////////////////////////////////////////////////////////////////////////////
  void randomizePoints();

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Update player score based on target "hit" and target's value.
  /// @param[in]  t_hit - Hit Target Identifier.
  //////////////////////////////////////////////////////////////////////////////
  void updateScore(Targets t_hit);

  ///@todo add descriptions and params.
  void updateDisplay();

  ///@todo add descriptions and params.
  void endGame();

  //
  // Member Variables
  //

  // Score
  uint8_t player_score_;

  // Timing
  bool start_game_;
  unsigned long start_time_;

  // Enhancements
  Array<uint8_t,TOTAL_TARGETS> target_values_;
  uint8_t min_points, max_points;

  // LCD
  U8X8_SH1106_128X64_NONAME_HW_I2C lcd_;      // See: (https://github.com/olikraus/u8g2/wiki/u8x8setupcpp#sh1106-128x64_noname-1); Uses MUCH less dynamic mem.
  uint8_t x_pos_, y_pos_;                     // Positions for glyph drawings.

  // Port Access
  PortAccessInterface port_ifc_;


};} // namespace game
#endif