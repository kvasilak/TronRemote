#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

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

