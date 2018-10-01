#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

// Set up nRF24L01 radio on SPI bus plus pins 15 & 14 (CE & CS)
RF24 radio(15, 14); 

// Pins on the remote for buttons
const uint8_t button_pins[] = { 9,8,7,6,5,4,3,2,16,17 };

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe[2] = {0xE8E8F0F0E1LL, 0xE8E8F0F0E2LL };

uint8_t button_states[sizeof(button_pins)];
static uint8_t state = 1;

void setup(void)
{
    Serial.begin(115200);
    printf_begin();
    printf("ROLE: Remote\n\r");
    
    radio.begin();
    
    radio.setPALevel(RF24_PA_MAX); 

    radio.printDetails();

    int i;
    for(i=0;i<=sizeof(button_pins);i++)
    {
        pinMode(button_pins[i],INPUT);
        digitalWrite(button_pins[i],HIGH);
    }

    //state == 1 on power on
    radio.openWritingPipe(pipe[0]);
    if (radio.write( &state, 1 ))
       printf("ok\n\r");
     else
       printf("failed\n\r");
    
   radio.openWritingPipe(pipe[1]);
   if (radio.write( &state, 1 ))
     printf("ok\n\r");
   else
     printf("failed\n\r");  
}

void loop(void)
{
    int i;

    //Find first off switch
    //switches are expected to be turned on ( and left on) in sequence
    for(i=0; i<=sizeof(button_pins); i++)
    {
        if(digitalRead(button_pins[i]) == 1)
        {
            if(state != i+1)
            {
                state = i+1;
                
                printf("New state %d\n", state);
                //the current value of i is the state
                //if no swtched are on state is zero, first switch on state is 1...
               radio.openWritingPipe(pipe[0]);
               if (radio.write( &state, 1 ))
                   printf("ok\n\r");
                 else
                   printf("failed\n\r");
                
               radio.openWritingPipe(pipe[1]);
               if (radio.write( &state, 1 ))
                 printf("ok\n\r");
               else
                 printf("failed\n\r");  
            }
            break; //found it
        }
    }
    delay(10);
}

