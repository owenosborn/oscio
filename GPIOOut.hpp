
class GPIOOut
{
    public:
        
	lo::Server *oscIn;
	const char *oscAddr;
	int pinNumber;

	GPIOOut (lo::Server &loserv, const char *addr, int pn) {
	    oscIn = &loserv;
	    oscAddr = addr;
	    pinNumber = pn;
	    pinMode(pn, OUTPUT);
	    digitalWrite(pinNumber, 1);
	    oscIn->add_method(oscAddr, "i", handler, this);
	}

	static int handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
	{
	    int i;
	    int state = argv[0]->i;
	    int pin = ((GPIOOut *)user_data)->pinNumber;
	    digitalWrite(pin, state);
	    //printf("pin: %d, state %d \n", pin, state);
	    return 1;
	}
        
};


