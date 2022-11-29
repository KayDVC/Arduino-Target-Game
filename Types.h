#pragma once
#ifndef TYPESFILE_H
#define TYPESFILE_H

// Arduino Libs
#include <stdint.h>
#include <Array.h>

namespace types {

  // Constants
  constexpr uint16_t SECOND = 1000; // Defined in ms.

  /// @todo Assign when values known.
  // Map Arduino pin connections to physical input components.
  enum class InputPorts: uint8_t {
    Targets_Data_Pin,
    Start_Button,
    TOTAL,
  };


  /// @todo Assign when values known.
  // Map Arduino pin connections to physical output components.
  enum class OutputPorts: uint8_t {
    LEDs_Data_Pin,
    LEDs_Clock_Pin,
    LEDs_Latch_Pin,
    Targets_Clock_Pin,
    Targets_Latch_Pin,
    TOTAL,
  };

  // Map targets to indentifier. Requires 2 shift registers minimum.
  enum class Targets: uint8_t{
    Target1= 0,
    Target2= 1,
    Target3= 2,
    Target4= 3,
    Target5= 4,
    Target6= 5,
    Target7= 6,
    Target8= 7,
    Target9= 8,
    Target10= 9,
    Target11= 10,
    Target12= 10,
    TOTAL,
  };
  constexpr uint8_t TOTAL_TARGETS = static_cast<uint8_t>(Targets::TOTAL);

  // Map LEDs -> Target. Requires 2 shift registers minimum.
  enum class LEDs: uint8_t{
    Target1= 0,
    Target2= 1,
    Target3= 2,
    Target4= 3,
    Target5= 4,
    Target6= 5,
    Target7= 6,
    Target8= 7,
    Target9= 8,
    Target10= 9,
    Target11= 10,
    Target12= 10,
    TOTAL,
  };
  constexpr uint8_t TOTAL_LEDS = static_cast<uint8_t>(LEDs::TOTAL);

  // Expression of state for all IO
  enum class EnaDis: bool {
    Disabled = false,
    Enabled = true,
  };

} // namespace types

#endif