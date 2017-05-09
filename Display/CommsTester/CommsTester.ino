// --- Communications ----
// Calculate based on max input size expected for one command
#define INPUT_SIZE 30


typedef enum {
  Disconnected,
  Connected,
  Sync
} ConnectionState;

// Serial communication
ConnectionState commState = Disconnected;
char data[INPUT_SIZE + 1];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

//void loop() {
//      memcpy(data, 0, INPUT_SIZE);
//      strcpy(data, "101:101&");
//      Serial.print(data);
//}

void loop() {
  // put your main code here, to run repeatedly:

  switch (commState)
  {
    case Disconnected:
    {
      memcpy(data, 0, INPUT_SIZE);
      strcpy(data, "101:101&");
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
      memcpy(data, 0, INPUT_SIZE);
      strcpy(data, "102:102&");
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
      memcpy(data, 0, INPUT_SIZE);
      strcpy(data, "103:0&104:98.6&118:55&");
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

            }
            // Find the next command in input string
            command = strtok(0, "&");
        }
      }
      
      break;
    }
  }


}
