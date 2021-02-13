#include <wiringPiSPI.h>

#define SPI_CH 1
class ADC
{
    public:
        
	lo::Address *oscOut;
	const char *oscAddr;
	int adcNumber;
	int value;
	int valueLast;

	ADC (lo::Address &loaddr, const char *addr, int num) {
	    oscOut = &loaddr;
	    oscAddr = addr;
	    adcNumber = num;
	    wiringPiSPISetup(SPI_CH, 4*1000*1000);  // ch 1 for adc
	}

    void poll(){
	    oscOut->send(oscAddr, "i", adc_read(adcNumber));
	}

	uint32_t adc_read(uint8_t adcnum) { 
	    uint8_t commandout = 0;

	    // read a channel from the MCP3008 ADC
	    commandout = adcnum & 0x7;  // only 0-7
	    commandout |= 0x18;		// start bit + single-ended bit

	    uint8_t spibuf[3];

	    spibuf[0] = commandout;
	    spibuf[1] = 0;
	    spibuf[2] = 0;

	    wiringPiSPIDataRW(SPI_CH, spibuf, 3);    

	    return ((spibuf[1] << 8) | (spibuf[2])) >> 4;
	}
};


