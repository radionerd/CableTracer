/* CableTracer.ino
 *  
# Cable Tracer

## Summary

This project was designed to trace multiple unmarked TV aerial cables by simultaneously outputting 1-12 tone pulses on pins D13,D2-D12 of an Arduino Nano.

* Pulses are approx 250ms long with 250ms gap and 261Hz (Middle C)
* A missing pulse identifies each pulse sequence
* Pulses may be observed with an LED and / or speaker
* Use 220R resistors to connect Port pins and 0V to cables under test

For your own safety ensure that wires under investigation are safe before connecting this device

## Scope display
<img src="ScopeTrace.bmp" />

Outputs D3 and D4
 */
/* MIT License 
 *  
 *  Copyright(c) 2023 Richard Jones
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 *  associated documentation files (the "Software"), to deal in the Software without restriction, 
 *  including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 *  subject to the following conditions:
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

const int8_t pins[] = { 13,2,3,4,5,6,7,8,9,10,11,12 }; // Enter port pin numbers here
const int NUM_PINS = sizeof(pins) ;
uint8_t mask[NUM_PINS];
const unsigned long half_period_us = 1916; // Tone freq 1/261Hz/2 = 1916 microseconds, approx middle C

void setup() {
  // initialize outputs
  for ( int i = 0 ; i < NUM_PINS ; i++ ) {
    pinMode( pins[i], OUTPUT);
    //mask[i] = 1;
  }
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  static uint8_t led_state;
  static unsigned int count=0;
  static unsigned long last_time_us=0;

  unsigned long time_us = micros();
  if ( ( time_us - last_time_us ) < half_period_us ) return;
  last_time_us += half_period_us;
  
  for ( int i = 0 ; i < NUM_PINS ; i++ )
    digitalWrite( pins[i], led_state & mask[i] ); // toggle output pin if required

  if ( ( ++led_state & 0x7f ) == 0 ) { // 128 cycles on, 128 cycles off
    if ( led_state == 0 ) count++ ;
    for ( unsigned i = 0 ; i < NUM_PINS ; i++ ) {
      mask[i] = 0;
      if ( led_state < 128 )
        if ( i >= ( count % (i+2) ) ) 
          mask[i] = 1; // turn output cycles on
    }
  }
}
