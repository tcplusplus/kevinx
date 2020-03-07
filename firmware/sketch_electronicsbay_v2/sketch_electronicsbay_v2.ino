const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
boolean newData = false;
float cycle = 0.0;
float ack_message = 0.0;
float start = 0.0;
float failcount = 0.0;


void setup()
{ 
  Serial.begin(9600); // connect serial
  Serial1.begin(9600); // connect serial
} 


void loop()
{ 
  cycle++;

  Serial.print("C: ");
  Serial.print(cycle);

  //delay(500);
  
  Serial1.write('<');
  Serial1.print(cycle);
  Serial1.write(",");
  Serial1.print(failcount);
  Serial1.write(",");
  Serial1.print(failcount);
  Serial1.write(",");
  Serial1.print(failcount);
  Serial1.write(",");
  Serial1.print(failcount);
  Serial1.write(",");
  Serial1.print(failcount);
  Serial1.write('>'); 
  
  recvWithStartEndMarkers();
  
  Serial.print(" - F: ");
  Serial.print(failcount);
 
  if (newData == true) 
  {
     strcpy(tempChars, receivedChars);
     parseData();
     showParsedData();
     newData = false;
  }

  //delay(1000);
}


void recvWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    start = millis();

    while (newData == false && millis()-start < 1000) 
    {      
      while (Serial1.available() > 0)
      {
        rc = Serial1.read();

        //Serial.print((String)rc);
        
        if (recvInProgress == true) 
        {
            if (rc != endMarker) 
            {
                receivedChars[ndx] = rc;
                //Serial.println((String)rc);  //debug
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
           
            else 
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
                //Serial.print(" Data fully arrived");
            }
         }

         else if (rc == startMarker) 
         {
            recvInProgress = true;
            //Serial.print(" - Data start - ");
         }
       }
    }

    if(newData == false)
    {
      failcount++;
    }
}


void parseData() 
{  
    char * strtokIndx; // this is used by strtok() as an index
    strtokIndx = strtok(tempChars,">");      // get the first part - the string
    ack_message = atof(strtokIndx);     // convert this part to a float
}


void showParsedData() 
{
  Serial.print(" - Ack: ");
  Serial.println((String)ack_message);
}
