#include <sys/time.h>
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <wiringPi.h>

// the control elements
#include "ADC.hpp"
#include "GPIOIn.hpp"
#include "GPIOOut.hpp"

int main()
{
    // setup the osc objects that we use to send and recieve messages
    // sender
    lo::Address oscOut("localhost", "9000");
    
    // receiver	
    lo::ServerThread oscIn(9001);
    if (!oscIn.is_valid()) {
        std::cout << "Problem starting osc server, probably the port is in use" << std::endl;
        return 1;
    }

    // setup gpio
    wiringPiSetupGpio();

    // setup some gpio inputs with pin number and osc message
    GPIOIn key1(oscOut, "/key1", 16);
    GPIOIn key2(oscOut, "/key2", 27);
    GPIOIn key3(oscOut, "/key3", 25);
    GPIOIn key4(oscOut, "/key4", 17);

    // setup some gpio outputs
    GPIOOut ledr(oscIn, "/ledr", 5);
    GPIOOut ledg(oscIn, "/ledg", 4);
    GPIOOut ledb(oscIn, "/ledb", 6);

    // setup ADC input from a MCP3008 adc with adc channel and osc message
    ADC knob1(oscOut, "/knob1", 0);

    // start the server thread
    oscIn.start();

    int count = 0;

    for(;;) {
	    
        // check knobs every 40 ms
        if (count > 20) {
	        count = 0;
	        knob1.poll();
	    }
	    count++;
	    
        key1.poll();
	    key2.poll();
	    key3.poll();
	    key4.poll();
	    
        // sleep for 2 ms
        usleep(2000);
    }
    
    return 0;
}

