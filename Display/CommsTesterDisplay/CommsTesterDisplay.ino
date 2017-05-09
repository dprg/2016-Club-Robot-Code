// --- Communications ----
// Calculate based on max input size expected for one command
#include <SPFD5408_TouchScreen.h>     // Touch library
#include "botscreen.h"                // DPRG Bot Screen Library
//
//BotScreen object
BotScreen touchDisplay = BotScreen();

// Serial communication
ConnectionState commState = Disconnected;
char data[INPUT_SIZE + 1];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  touchDisplay.Init();

//  memcpy(data, 0, INPUT_SIZE);
//  strcpy(data, "101:0&");
//  Serial.print(data);
}

void loop() {
  // put your main code here, to run repeatedly:

  switch (commState)
  {
    case Disconnected:
    {
      touchDisplay.UpdateStatus("State", 1, Line1);  
      touchDisplay.UpdateStatus("Disconnected", 1, Line2);
   
      if(Serial.available())
      {
        byte size  = Serial.readBytes(data, INPUT_SIZE);
        data[size]=0;

        // Read each command pair 
        char* command = strtok(data, "&");
        while (command != 0)
        {
            // Split the command in two values
            char* separator = strchr(command, ':');
            if (separator != 0)
            {
                // Actually split the string in 2: replace ':' with 0
                *separator = 0;
                int cmd = atoi(command);
                ++separator;
                int value = atoi(separator);
        
                // Do something with cmd and value
                if(cmd == 101)
                {
                  commState = Connected;
                  break;
                }
            }
            // Find the next command in input string
            command = strtok(0, "&");
        }
      }
      
      break;
    }
    case Connected:
    {
      touchDisplay.UpdateStatus("State", 1, Line1);  
      touchDisplay.UpdateStatus("Connected", 1, Line2);

      memcpy(data, 0, INPUT_SIZE);
      strcpy(data, "101:102&");
      Serial.print(data);

      if(Serial.available())
      {
        byte size  = Serial.readBytes(data, INPUT_SIZE);
        data[size]=0;

        // Read each command pair 
        char* command = strtok(data, "&");
        while (command != 0)
        {
            // Split the command in two values
            char* separator = strchr(command, ':');
            if (separator != 0)
            {
                // Actually split the string in 2: replace ':' with 0
                *separator = 0;
                int cmd = atoi(command);
                ++separator;
                int value = atoi(separator);
        
                // Do something with cmd and value
                if(cmd == 102)
                {
                  commState = Sync;
                  break;
                } 
            }
            // Find the next command in input string
            command = strtok(0, "&");
        }
      }

      break;
    }
    case Sync:
    {
      touchDisplay.UpdateStatus("Syncronized", 1, Line1);  
      touchDisplay.UpdateStatus("Congrats", 1, Line2);
      memcpy(data, 0, INPUT_SIZE);
      strcpy(data, "103:0&");
      Serial.print(data);

      if(Serial.available())
      {
        byte size  = Serial.readBytes(data, INPUT_SIZE);
        data[size]=0;

        // Read each command pair 
        char* command = strtok(data, "&");
        while (command != 0)
        {
            // Split the command in two values
            char* separator = strchr(command, ':');
            if (separator != 0)
            {
                // Actually split the string in 2: replace ':' with 0
                *separator = 0;
                int cmd = atoi(command);
                ++separator;
                float value = atof(separator);

                // if we see any handshake cmd reset the state machine
                if(cmd == 101)
                {
                  commState = Disconnected;
                  break;
                } 

                // Do something with cmd and value
                touchDisplay.UpdateStatus(separator, 1, Line2);

            }
            // Find the next command in input string
            command = strtok(0, "&");
        }
      }
      
      break;
    }


  }
}
