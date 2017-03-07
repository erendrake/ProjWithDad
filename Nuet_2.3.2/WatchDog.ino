/*#include <avr/wdt.h> //should be in any adruino IDE

void setup() {
wdt_disable(); //always good to disable it, if it was left 'on' or you need init time
//do some stuff here
wdt_enable(WDTO_8S); //enable it, and set it to 8s
}

void loop() {
//do some stuff here
wdt_delay(5000) //instead of delay, i made my own delay to reset the WDT
}

//this makes sure the WDT is reset immediately when entering the
//function, but we can still benefit from a real 'delay'.
//upon leaving the function, we reset it again.
//i realize timing will be loose, you can always do something with
//millies() if you need strict timing.
//you might also need to adjust the '1000' if you WDT is shorter
*/
void wdt_delay(unsigned long msec) {
wdt_reset();

while(msec > 1000) {
wdt_reset();
delay(1000);
msec -= 1000;
}
delay(msec);
wdt_reset();
}

//to reload the board, you only have to delay > WDT
//another option is to set the WDT very short, and do a short delay

void wdt_reload() {
delay(8001);
}


