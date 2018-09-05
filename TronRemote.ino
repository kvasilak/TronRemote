#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example LED Remote
 *
 * This is an example of how to use the RF24 class to control a remote
 * bank of LED's using buttons on a remote control.
 *
 * On the 'remote', connect any number of buttons or switches from
 * an arduino pin to ground.  Update 'button_pins' to reflect the
 * pins used.
 *
 * On the 'led' board, connect the same number of LED's from an
 * arduino pin to a resistor to ground.  Update 'led_pins' to reflect
 * the pins used.  Also connect a separate pin to ground and change
 * the 'role_pin'.  This tells the sketch it's running on the LED board.
 *
 * Every time the buttons change on the remote, the entire state of
 * buttons is send to the led board, which displays the state.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 15 & 14 (CE & CS)
RF24 radio(15, 14); 

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2,3,4,5,6,7 };
const uint8_t num_button_pins = sizeof(button_pins);

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t button_states[num_button_pins];
static uint8_t state = 0xff;

void setup(void)
{
  Serial.begin(115200);
  printf_begin();
  printf("ROLE: Remote\n\r");

  radio.begin();

  //radio.setPALevel(RF24_PA_MIN); 

    radio.openWritingPipe(pipe);

  radio.printDetails();

    int i = num_button_pins;
    while(i--)
    {
      pinMode(button_pins[i],INPUT);
      digitalWrite(button_pins[i],HIGH);
    }
}

void loop(void)
{
  int i;
  //printf("Switch states: ");

  //Find first off switch
  //switches are expected to be turned on ( and left on) in sequence
  for(i-0; i<num_button_pins; i++)
  {

    //printf("%d, ",digitalRead(button_pins[i]));
    
    if(digitalRead(button_pins[i]) == 1)
    {
      if(state != i)
     {
        state = i;
     
         printf("New state %d\n", state);
        //the current value of i is the state
        //if no swtched are on state is zero, first switch on state is 1...
        bool ok = radio.write( &state, 1 );
        
        if (ok)
          printf("ok\n\r");
        else
          printf("failed\n\r");
        
        
      }
      break; //found it
    }
  }
    //printf("\n");
    delay(1000);
}


  
//    // Get the current state of buttons, and
//    // Test if the current state is different from the last state we sent
//    int i = num_button_pins;
//    bool different = false;
//    while(i--)
//    {
//      uint8_t state = ! digitalRead(button_pins[i]);
//
//      if ( state != button_states[i] )
//      {
//        different = true;
//        button_states[i] = state;
//      }
//    }
//
//    // Send the state of the buttons to the LED board
//    if ( different )
//    {
//      printf("Now sending...");
//      bool ok = radio.write( button_states, num_button_pins );
//      if (ok)
//        printf("ok\n\r");
//      else
//        printf("failed\n\r");
//    }

    // Try again in a short while
//    delay(200);
//

