#include<SoftwareSerial.h>
SoftwareSerial client(2, 3); //RX, TX
const int VAL_PROBE = 0; //Analog pin 0
const int MOISTURE_LEVEL = 600;
float percent;
String webpage = "";
int i = 0, k = 0;
String readString;
int x = 0;
boolean No_IP = false;
String IP = "";
char temp1 = '0';
String dat = "<p>Data Received Successfully.....</p>";   //21

void check4IP(int t1)
{
  int t2 = millis();
  while (t2 + t1 > millis())
  {
    while (client.available() > 0)
    {
      if (client.find("WIFI GOT IP"))
      {
        No_IP = true;
      }
    }
  }
}
void get_ip()
{
  IP = "";
  char ch = 0;
  while (1)
  {
    client.println("AT+CIFSR");
    while (client.available() > 0)
    {
      if (client.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while (client.available() > 0)
        {
          ch = client.read();
          if (ch == '+')
            break;
          IP += ch;
        }
      }
      if (ch == '+')
        break;
    }
    if (ch == '+')
      break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}
void connect_wifi(String cmd, int t)
{
  int temp = 0, i = 0;
  while (1)
  {
    Serial.println(cmd);
    client.println(cmd);
    while (client.available())
    {
      if (client.find("OK"))
        i = 8;
    }
    delay(t);
    if (i > 5)
      break;
    i++;
  }
  if (i == 8)
    Serial.println("OK");
  else
    Serial.println("Error");
}
void wifi_init()
{
  connect_wifi("AT", 100);

  connect_wifi("AT+CWMODE=3", 100);
  connect_wifi("AT+RST", 5000);
  connect_wifi("AT+CWQAP", 100);

  check4IP(5000);
  if (!No_IP)
  {
    Serial.println("Connecting Wifi....");
    connect_wifi("AT+CWJAP=\"hemanth\",\"12345678\"", 7000);        //provide your WiFi username and password here
    // connect_wifi("AT+CWJAP=\"vpn address\",\"wireless network\"",7000);
  }
  else
  {
  }
  Serial.println("Wifi Connected");
  get_ip();
  connect_wifi("AT+CIFSR", 100);
  connect_wifi("AT+CIPMUX=1", 100);
  connect_wifi("AT+CIPSERVER=1,80", 100);
  // delay(40000);
}
void sendwebdata(String webPage)
{
  int ii = 0;
  while (1)
  {
    unsigned int l = webPage.length();
    Serial.print("AT+CIPSEND=0,");
    client.print("AT+CIPSEND=0,");
    Serial.println(l + 2);
    client.println(l + 2);
    delay(100);
    Serial.println(webPage);
    client.println(webPage);
    while (client.available())
    {
      //Serial.print(Serial.read());
      if (client.find("OK"))
      {
        ii = 11;
        break;
      }
    }
    if (ii == 11)
      break;
    delay(100);
  }
}
void LedState(int state)
{
  digitalWrite(13,state);
}
void setup()
{
  Serial.begin(9600);
  client.begin(9600);
   pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(12,OUTPUT);
  wifi_init();
  Serial.println("System Ready..");
}
void loop()
{
  //*************************************************

int level=analogRead(A2);
if(level>780)
{
digitalWrite(12,HIGH);
digitalWrite(13,LOW);
digitalWrite(7,LOW);
delay(5000);
loop();
}

  else
  {
    digitalWrite(12,LOW);
  //**************************************************

int moisture = analogRead(VAL_PROBE);
  percent=(moisture/1023.0)*100.0;
  percent=100.0-percent;
  
  
 // Serial.print("Moisture = ");
 // Serial.println(moisture);
  
  if(moisture < MOISTURE_LEVEL)
  {
     LedState(LOW);
    digitalWrite(7,LOW);
  }
  else 
  {
     LedState(HIGH);
    digitalWrite(7,HIGH);
  }
 
  //**************************************************
  k = 0;
  Serial.println("Please Refresh your Page");
  while (k < 1000)
  {
    k++;
    while (client.available())
    {
      if (client.find("0,CONNECT"))
      {
        Serial.println("Start Printing");
        Send();
        Serial.println("Done Printing");
        delay(10);
      }
    }
    delay(1);
  }
  }
}
void Send()
{

webpage = "<h1 color=blue>moisture of soil</h1><body bgcolor=f0f0f0>";
      sendwebdata(webpage);
     // int sensor=analogRead(A1);
      int sensor1=analogRead(A0);
     // float temp=(sensor/1024.0)*5000;
      float temp1=(sensor1/1023.0);
      temp1=temp1*100;
     // float cel=temp/10;
     // Serial.println(cel);
     // webpage=cel;
    //  sendwebdata(webpage);
      webpage=100-temp1;
      sendwebdata(webpage);
  int verr=0;
  while(1){
    verr++;
  client.println("AT+CIPCLOSE=0");
  if(verr==100)
  {break;}
  }
   Serial.println("AT+CIPCLOSE=0");
  delay(10);
}

