#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

ESP8266WebServer server;//webserrver
WebSocketsServer webSocket = WebSocketsServer(81);//websocket server
uint8_t pin=4;//Pin D2 is selected to operate MOSFET
char* ssid = "301_2.4";
char* password = "Rekulapalli";
String message;
char webpage[] PROGMEM = R"=====(
<html>
<head>
<title>Smart-TENS</title>
<script>
var Socket;
function init() {Socket = new WebSocket('ws://' + window.location.hostname + ':81/');}
function conventional(){
document.getElementById("mode").innerHTML="Conventional Mode Activated";
Socket.send("conventional");
}
function acupuncture(){
document.getElementById("mode").innerHTML="Acupuncture Mode Activated";
Socket.send("acupuncture");
}
function intense(){
document.getElementById("mode").innerHTML="Intense Mode Activated";
Socket.send("intense");
}
function actstop(){
document.getElementById("mode").innerHTML="Stop Command Activated";
Socket.send("actstop");
}
</script>
</head>
<body onload="javascript:init()">
<h1>Smart TENS Device</h1>
</hr>
<h2>Device Mode</h2>
<p id="mode" > </p>
<br>
</hr>
<h2>Select Mode</h2>
</hr>
<br>
<div>
<button id="conventional" onclick="javascript:conventional();">Conventional TENS</button>
</div>
<br>
<div>
<button id="acupuncture" onclick="javascript:acupuncture();">Acupuncture TENS</button>
</div>
<br>
<div>
<button id="Intense" onclick="javascript:intense();">Intense TENS</button>
</div>
<br>
<div>
<h2>Stop Action </h2>
<button id="stop" onclick="javascript:actstop();">Stop</button>
</div>
<br>
</body>
</html>
)=====";

void conventional(){
digitalWrite(pin,HIGH);
delay(35);
digitalWrite(pin,LOW);
delay(35);
}

void acupuncture(){
digitalWrite(pin,HIGH);
delay(330);
digitalWrite(pin,LOW);
delay(330);
}

void intense(){
digitalWrite(pin,HIGH);
delay(17);
digitalWrite(pin,LOW);
delay(17);
}

void actstop(){
digitalWrite(pin,LOW); 
delay(100);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    
      message = String((char*)( payload));
      Serial.println(message);         
}
}

void setup()
{
  pinMode(pin, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){server.send_P(200, "text/html", webpage);});
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{  
webSocket.loop();
server.handleClient();
if(message == "conventional"){conventional();}
else if(message == "acupuncture"){acupuncture();}
else if(message == "intense"){intense();}
else{actstop();}  
}
