#pragma once
#ifndef PORTACCESSFILE_CPP
#define PORTACCESSFILE_CPP

#include "PortAccess.h"

constexpr auto POST_READ_WAIT = 10; // Helps avoid false positive reads.

namespace port_access {

  // Constructor
  PortAccessInterface::PortAccessInterface():
    tdp_(static_cast<uint8_t>(InputPorts::Targets_Data_Pin)),
    tcp_(static_cast<uint8_t>(OutputPorts::Targets_Clock_Pin)),
    tlp_(static_cast<uint8_t>(OutputPorts::Targets_Latch_Pin)),
    ldp_(static_cast<uint8_t>(OutputPorts::LEDs_Data_Pin)),
    lcp_(static_cast<uint8_t>(OutputPorts::LEDs_Clock_Pin)),
    llp_(static_cast<uint8_t>(OutputPorts::LEDs_Latch_Pin))
  {
    led_register_.fill(EnaDis::Disabled);
    initializePorts();
  }

  EnaDis PortAccessInterface::getTargetState(Targets target){
    // Compare desired target to actual target "hit" detected, if any.
    return (target == sampleInputs())? EnaDis::Enabled : EnaDis::Disabled;
  }


  void PortAccessInterface::setLedState(LEDs led, EnaDis state){
    // Update state in LED array.
    led_register_[static_cast<uint8_t>(led)] = state;
    // Send information to physical components.
    updateLeds();
  }

  bool PortAccessInterface::targetHit(Targets& t_hit){
    // Read target inputs. Store first target "hit" detected or Targets::TOTAL
    // if no "hits" detected.
    t_hit =  sampleInputs();

    // Return whether any target "hit" was detected.
    return (t_hit == Targets::TOTAL);
  }

  bool PortAccessInterface::sampleStartButton(){
    // Read and return signal from start button. 
    return (digitalRead(static_cast<uint8_t>(InputPorts::Start_Button)));
  }

  /// @todo update Outport verification to be a single if statement.
  bool PortAccessInterface::ioSet(){

    // ALL CREDIT FOR THIS FUNCTION GOES TO ARDUINO STACK EXCHANGE USER: MAJENKO.
    // See ref here: (https://arduino.stackexchange.com/a/13173).
    // See profile here: (https://arduino.stackexchange.com/users/4143/majenko).

    Serial.println("--------- Begin Port Config Verification ---------");
    Serial.println("");

    // Verify all ports in types::InputPorts are in pinMode Input.
    for (auto& pin: input_ports){
      uint8_t bit = digitalPinToBitMask(static_cast<uint8_t>(pin));
      uint8_t port = digitalPinToPort(static_cast<uint8_t>(pin));
      volatile uint8_t *reg = portModeRegister(port);

      if (*reg & bit) {
        // It's an output; Invalid configuration. Return false.
        return false;
      }
    }

    Serial.println("Input ports correctly configured. ");
    // Verify all ports in types::InputPorts are in pinMode Input.
    for (auto& pin: input_ports){
      uint8_t bit = digitalPinToBitMask(static_cast<uint8_t>(pin));
      uint8_t port = digitalPinToPort(static_cast<uint8_t>(pin));
      volatile uint8_t *reg = portModeRegister(port);

      if (*reg & bit) {
        // do nothing.
      }else{
        // It's an input. Invalid configuration. Return false.
        return false;
      }
    }
    Serial.println("Output ports correctly configured.");
    Serial.println("");
    Serial.println("");
    return true;
  }

  void PortAccessInterface::flashLEDs(){
    constexpr auto flashes = 3;
    constexpr auto quarter_second = 250; // defined in ms.

    Serial.println("--------- Begin LED Verification ---------");
    Serial.println("");
    Serial.println("All LEDs should flash 3 times.");

    for (auto i =0; i < flashes; i++){
      // Turn on all LEDs.
      led_register_.fill(EnaDis::Enabled);
      // Send signal to physical components.
      updateLeds();
      // Allow time for LEDs to display new signal.
      delay(quarter_second);

      // Turn off all LEDs.
      led_register_.fill(EnaDis::Disabled);
      // Send signal to physical components.
      updateLeds();
      // Allow time for LEDs to display new signal.
      delay(quarter_second);

      Serial.print("Flash count: ");
      Serial.println(i);     
    }

    Serial.println("");
    Serial.println("");

  }

  void PortAccessInterface::verifyTargets(){
    uint8_t remaining_targets = 5;
    Serial.println("--------- Begin Target Verification ---------");
    Serial.println("Please hit 5 targets with laser");
    do {
      Targets test;
      if(targetHit(test)){
        Serial.print("Target Hit Detected; Target Identifier: ");
        Serial.println(static_cast<uint8_t>(test));
        remaining_targets-= 1;

        Serial.print("Remaining Targets: ");
        Serial.println(remaining_targets);
      };
    
    }while (remaining_targets!= 0);
    Serial.println("Test completed.");
    Serial.println("");
    Serial.println("");
  }

// Private Functions
  void PortAccessInterface::initializePorts(){
    // Set Input Ports <- Types::InputPorts.
    for (auto& in_port: input_ports){
      pinMode(static_cast<uint8_t>(in_port), INPUT);
    }

    // Set Output Ports <- Types::OutputPorts.
    for (auto& out_port: output_ports){
      pinMode(static_cast<uint8_t>(out_port), OUTPUT);
    }

  }

  Targets PortAccessInterface::sampleInputs()
  {
    // Read in all target input at once.
    digitalWrite(tlp_, LOW);
    digitalWrite(tlp_, HIGH);

    // Read inputs one bit at a time. LSB -> MSB.
    for (auto i = 0; i < TOTAL_TARGETS; i++){

      // Return first HIGH ("hit") signal detected.
      if(digitalRead(tdp_)){
        return static_cast<Targets>(i);
      }

      // Pop read bit. Continue iteration with next
      // input reading.
      digitalWrite(tcp_, HIGH);
      digitalWrite(tcp_, LOW);
    }

    // Ensure registers have time to clear before next read.
    delay(POST_READ_WAIT);

    // Return invalid identifier if no "hit" detected.
    return Targets::TOTAL;
  };

  void PortAccessInterface::updateLeds()
  {
    // Freeze updates to LEDs via shift register output.
    digitalWrite(llp_, LOW);

    // Push in states for all LEDs. MSB -> LSB.
    for (auto i = TOTAL_LEDS - 1; i >= 0; i--) {
      // Prep SR to receive bit.
      digitalWrite(lcp_, LOW);
      // Send bit to SR.
      digitalWrite(ldp_, static_cast<uint8_t>(led_register_[i]));
      // Finalize bit transfer.
      digitalWrite(lcp_, HIGH);
    }

    // Update signal to physical LEDs.
    digitalWrite(llp_, HIGH);
  }

} // namespace port_access

#endif