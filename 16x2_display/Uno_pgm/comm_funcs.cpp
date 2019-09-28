/*****************************************************
*
*  communication functions
*
*  author: Doug Paradis, 2017-18
*
* note: The Arduino serial receive and send buffers
*       each hold 64 bytes.
*
******************************************************/

#include "common.h"
#include "comm_funcs.h"

COMM_CMDS comm_cmd;
int comm_data[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t prev_cmd_finished_flg = DONE;


//----------------------- functions -----------------------
void send_cmd(COMM_CMDS cmd)
{
	uint8_t message[] = {0xFF,(uint8_t)cmd};   // form packet (start of packet = 0xFF)
	Serial.write(message, sizeof(message));
}


/*****************************************************
*
*  Expected data packet (min size 4 bytes, add 3 bytes
*                        per additional data value)
*
*      start of packet 0xFF (0xFF reserved for header)
*      for each data value
*        byte 1   7 bits of lower byte of int (least significant)
*        byte 2   8 bits of upper byte of int shifted to be the
                  8th thru 15th bits of the integer
*		 byte 3   sign byte (0xf0 = neg, else pos) 
*
*      Only the header can be 0xFF
*
*  range of values is -32639 to 32639 (-0x7F7F to 0x7F7F) 
*  no chksum 
*
******************************************************/
void  read_data (uint8_t num_of_data_val, int data_array[])
{
	
	
	static uint8_t packetState = 0;  // unsigned 0..255
	//static int val = 0;              // storage
	static uint8_t cntr = 0;
	uint8_t b = 0;
	static uint8_t buf[2];
	
    prev_cmd_finished_flg = NOT_DONE;
	
	while (Serial.available()) {
			b = Serial.read();
			//Serial.write(b);    // <-----------troubleshooting------<<<<
		
		switch (++packetState) {   // state auto advances unless set back
			case 1 : 
				if (b != 0xFF) 
					packetState--; 
			break;  // 0xFF unique to start of packet
			
			case 2 : 
				buf[0] = b & 0x7F;  // least significant (only first 7 bits)
			break;
			
			case 3 : 
				buf[1] = b;
				data_array[cntr] = buf[1]<<7 | buf[0];  // most significant (should not be 0xff)

				buf[0] = 0;
				buf[1] = 0;
			break;
			
			case 4: 
				if (b == 0xF0){
					data_array[cntr] *= -1;
				}
				//Serial.write ((char*)&data_array[cntr]); // <-------troubleshooting------<<<<
				if (cntr < (num_of_data_val - 1)){         // minus 1 to align cntr with data_array
					packetState = 1;   // remember it is incremented when it enters switch
					cntr++;
				}
				else {
					packetState = 0;
					cntr = 0;
					prev_cmd_finished_flg = DONE;
				}
			break;

			//Serial.write('R');  // ack to PC
			
			break;
		}
	}
	
		
		
}
	


void initialize_comm ()
{
	Serial.begin(155200);
}
