
class GPIOIn
{
    public:
        
	lo::Address *oscOut;
	const char *oscAddr;
	int pinNumber;
	int state;
	int stateLast;

	GPIOIn (lo::Address &loaddr, const char *addr, int pn) {
	    oscOut = &loaddr;
	    oscAddr = addr;
	    pinNumber = pn;
	    pinMode(pinNumber, INPUT);
	    pullUpDnControl(pinNumber, PUD_UP);
	    state = stateLast = digitalRead(pinNumber);
	}

        void poll(){
	    state = digitalRead(pinNumber);
	    if (state != stateLast) {
		stateLast = state;
		printf("pin change pin %d\n", pinNumber);
		oscOut->send(oscAddr, "i", state);
	    }
	}
};


