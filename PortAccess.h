
#pragma once
#ifndef PORTACCESSFILE_H
#define PORTACCESSFILE_H

// Arduino Libs
#include <Arduino.h>
#include <Array.h>

// Custom Libs
#include "Types.h"
#include "stdint.h"

using Targets     = types::Targets;
using LEDs        = types::LEDs;
using EnaDis      = types::EnaDis;
using OutputPorts = types::OutputPorts;
using InputPorts  = types::InputPorts;

using namespace types;

namespace port_access { class PortAccessInterface{

  public:
  // Constructor
  PortAccessInterface();
  
  // Destructor
  ~PortAccessInterface() = default;

  //
  // Accessors
  //
  //////////////////////////////////////////////////////////////////////////////
  /// @details   Get the state of desired target.
  /// @param[in] target - Target to read.
  /// @return    Whether there was a detected "hit" for desired target. 
  //////////////////////////////////////////////////////////////////////////////
  EnaDis getTargetState(Targets target);

  //////////////////////////////////////////////////////////////////////////////
  /// @details   Set the state of desired LED.
  /// @param[in] led - LED to update.
  /// @param[in] state - State to set.
  //////////////////////////////////////////////////////////////////////////////
  void setLedState(LEDs led, EnaDis state);

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Get the state of all target inputs.
  /// @param[in]  t_hit - Varible to store read data to.
  /// @return     Whether there was a detected "hit" for any target.
  /// @note       t_hit will either be the first target "hit" detected 
  ///             or Targets::TOTAL.
  //////////////////////////////////////////////////////////////////////////////
  bool targetHit(Targets& t_hit);

  //////////////////////////////////////////////////////////////////////////////
  /// @details   Get the state of InputPorts::Start_Button.
  /// @return    Whether the start button was pressed. 
  //////////////////////////////////////////////////////////////////////////////
  bool sampleStartButton();

  //
  // Validation
  //
  //////////////////////////////////////////////////////////////////////////////
  /// @details    Get the state of desired target.
  /// @param[in]  target - Target to read.
  /// @return     Whether there was a detected "hit" for desired target. 
  //////////////////////////////////////////////////////////////////////////////
  bool ioSet();

  //////////////////////////////////////////////////////////////////////////////
  /// @details   Toggle all LEDS 3 times.
  /// @note      Uses processor delays; Should only be used during 
  ///            initialization or post-game functionality.
  //////////////////////////////////////////////////////////////////////////////
  void flashLEDs();

  //////////////////////////////////////////////////////////////////////////////
  /// @details   Test target input accuracy.
  /// @note      Test requires 5 targets to be hit; all 5 should be distinct.
  //////////////////////////////////////////////////////////////////////////////
  void verifyTargets();

  //
  // Private functions
  //
  private:
  //////////////////////////////////////////////////////////////////////////////
  /// @details    Prepare all ports for use.
  /// @note       Should not be used outside of initialization. 
  //////////////////////////////////////////////////////////////////////////////
  void initializePorts();

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Read in all target input states.
  /// @return     Identifier of first target "hit" detected or Targets::TOTAL. 
  //////////////////////////////////////////////////////////////////////////////
  Targets sampleInputs();

  //////////////////////////////////////////////////////////////////////////////
  /// @details    Set all LED states.
  //////////////////////////////////////////////////////////////////////////////
  void updateLeds();

  //
  // Member Variables
  //
  Array<EnaDis, types::TOTAL_LEDS> led_register_; // Avoid unnessecary read ops.
  uint8_t tdp_, tcp_, tlp_;                          // Target data, clock, and latch pins.
  uint8_t ldp_, lcp_, llp_;                          // LED data, clock, and latch pins.

};} // namespace port_access
#endif