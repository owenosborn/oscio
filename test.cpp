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

    GPIOIn key1(oscOut, "/key1", 16);
    GPIOIn key2(oscOut, "/key2", 27);
    GPIOIn key3(oscOut, "/key3", 25);
    GPIOIn key4(oscOut, "/key4", 17);

    GPIOOut ledr(oscIn, "/ledr", 5);
    GPIOOut ledg(oscIn, "/ledg", 4);
    GPIOOut ledb(oscIn, "/ledb", 6);

    ADC knob1(oscOut, "/knob1", 0);

    // start the server thread
    oscIn.start();

    int count = 0;

    // loop
    for(;;) {
	    if (count > 20) {
	        count = 0;
	        knob1.poll();
	    }
	    count++;
        //lo_server_recv_noblock(oscIn, 0); // 10ms timeout
	    key1.poll();
	    key2.poll();
	    key3.poll();
	    key4.poll();
	    usleep(2000);
    }
    
    return 0;
}

