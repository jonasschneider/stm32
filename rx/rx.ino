
#include <VirtualWire.h>

void setup()
{
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");
    delay(1);

    // Initialise the IO and ISR
//    vw_set_tx_pin(8);
    pinMode(13, OUTPUT);
    vw_set_rx_pin(3);
//    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(8*8*8);	 // Bits per sec
    vw_rx_start();
    

}

void loop()
{
    vw_wait_rx();
    
    uint8_t buf[100];
    uint8_t buflen = 100;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;
	// Message with a good checksum received, dump it.
	Serial.print("\nGot ");
        Serial.print(buflen, DEC);
        Serial.print(":");
	
	for (i = 0; i < buflen; i++)
	{
            char c = (buf[i]);
            Serial.print(c);
	    Serial.print(" ");
	}
       // Serial.print(count);
	Serial.println("");
    }
}

