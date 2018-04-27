#include <WiFi.h>
 
const char* ssid     = "JAZZTEL_6AD0";
const char* password = "qLQLqDpR4t82G3sU7AfF";
 
WiFiServer server(80);

// Client variables 
char linebuf[80];
int charcount=0;
char *cad;
int aux;

struct package
{
  int i=0;
  char c;
  boolean b;
  double d;
};

typedef struct package Package;
Package data;
 
void setup()
{
    Serial.begin(115200); 
    delay(10);
 
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
 
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
   
    server.begin();
}
 
int value = 0;
 
void loop(){
  
  // listen for incoming clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>ESP32 - Web Server</h1>");
          client.println("<form>Enter integer:<br><input type=\"text\" name=\"int\"><br><input type=\"submit\" value=\"Submit\"></form>");
          client.println("<form>Enter character:<br><input type=\"text\" name=\"char\"><br><input type=\"submit\" value=\"Submit\"></form>");
          client.println("<form>Enter boolean:<br><input type=\"text\" name=\"bool\"><br><input type=\"submit\" value=\"Submit\"></form>");
          client.println("<form>Enter double:<br><input type=\"text\" name=\"double\"><br><input type=\"submit\" value=\"Submit\"></form>");
          client.println("</html>");
          break;
        }
        
        if (c == '\n') {
          Serial.println(linebuf);
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /?int=") > 0){
            cad = &linebuf[10];
            data.i = atoi(cad);            
          }
          
          else if (strstr(linebuf,"GET /?char=") > 0){
            data.c = linebuf[11];            
          }
          
          else if (strstr(linebuf,"GET /?bool=") > 0){
            cad = &linebuf[11];
            aux = atoi(cad);
            if ( aux == 1)
            {
              data.b = true;
            }
            else
            {
              data.b = false;
            }                     
          }
          
          else if (strstr(linebuf,"GET /?double=") > 0){
            cad = &linebuf[13];
            data.d = atof(cad);            
          }
          
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");

    Serial.println("DATA =>");
    Serial.print("INT: ");
    Serial.println(data.i);
    Serial.print("CHAR: ");
    Serial.println(data.c);
    Serial.print("BOOL: ");
    Serial.println(data.b);
    Serial.print("DOUBLE: ");
    Serial.println(data.d);
  }
}
