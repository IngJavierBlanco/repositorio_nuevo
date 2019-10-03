//LIBRERIAS UNCLUIDAS
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

//VARIABLES CON NOMBRE DE RED WIFI Y CLAVE
const char* ssid = "Pacific-Logging";
const char* password = "P@c1f1c-L0gg1ng";

//VARIABLES PARA GUARDAR LOS DATOS INGRESADOS EN EL FORMULARIO HTML
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

//ESCRIBIMOS LA INSTRUCCION PARA EMPEZAR A ESCRIBIR CODIGO HTML
const char index_html[] PROGMEM = R"rawliteral( 

<!--A CONTINUACION PODEMOS PEGAR NUESTRO CODIGO HTML -->

<!DOCTYPE HTML><html>
<head>
  <title>Formulario para ingreso de datos al ESP8266</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>


<h1>
FORMULARIO DE INGRESO DE DATOS
</h1>

<p>ingrese los datos para actualizar variables en la  MATCH BOX</p>

<!-- El atributo de acción especifica dónde enviar los datos insertados en el formulario después de presionar enviar. En este caso, realiza una solicitud HTTP GET -->
<form action="/get">
  
  <!-- La siguiente línea define un campo de entrada de texto de una línea. El atributo type especifica que queremos un campo de entrada de texto, y el atributo name especifica el nombre del elemento de entrada. -->
  input1: <input type="text" name="input1">
  
 <!--La siguiente línea define un botón para enviar los datos del formulario HTML. En este caso, el atributo type especifica que desea un botón de envío, y el valor especifica el texto en el botón.--> 
  <input type="submit" value="OK">
  
  
</form><br>

<form action="/get">
  
  input2: <input type="text" name="input2">
  <input type="submit" value="OK">
</form><br>

<form action="/get">
  
  input3: <input type="text" name="input3">
  <input type="submit" value="OK">
</form>

<!--HASTA AQUI LLEGA EL CODIGO HTML-->
)rawliteral";


//FUNCION NOT FOUND EN CASO DE HACER UNA SOLICITUD A UNA URL NO VALIDA
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
//DEFINIMOS EL LED DE LA ESP12 COMO SALIDA
 pinMode(LED_BUILTIN, OUTPUT);

//CONFIGURAMOS LA VELOCIDAD DE COMUNICACION EN BAUDIOS POR USB
  Serial.begin(115200);

  //CONFIGURAMOS EL MODO DE CONEXION WIFI
  WiFi.mode(WIFI_STA);
  
  //CON ESTA INSTRUCCION AGREGAMOS LOS NOMBRES DE LAS VARIABLES DONDE ESTEN GUARDADAS "NOMBRE LA RED" Y "CLAVE"
  WiFi.begin(ssid, password);

  //SI EL RESULTADO DE INTERNAR CONECTARSE A LA RED FALLA SE INTENTA CONECTAR DE NUEVO
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;

}
//INSTRUCCION PARA IMPRIMIR EN LA SIGUIENTE LINEA
Serial.println();

//IMPRIMIMOS EN EL PUERTO SERIAL "IP Address: " 
Serial.print("IP Address: ");
  
//IMPRIMIMOS LA IP ASIGNADA AL ESP8266 POR EL ROUTER EN LA MISMA LINEA
Serial.println(WiFi.localIP());

//MANEJO DE SOLICITUD HTTP GET: Cuando accede a la URL de la ruta, envia la pagina HTML al cliente.
//En este caso el texto HTML se guarda en la variable index_html.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

//AQUI CONTROLAMOS LO QUE SUCEDE CUANDO LA ESP RECIBE UNA SOLICITUD DE LAS RUTAS /GET
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

//CREAMOS ESTAS DOS VARIABLES PARA GUARDAR LO QUE INGRESAMOS AL FORMULARIO
    String inputMessage;
    String inputParam;

//SI EL VALOR RECIBIDO ES IGUAL A PARAM_INPUT_1 LO ALMACENA EN ESA VARIABLE SUSTITUYENDO SU VALOR PREVIO
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }

//SI EL VALOR RECIBIDO ES IGUAL A PARAM_INPUT_2 LO ALMACENA EN ESA VARIABLE SUSTITUYENDO SU VALOR PREVIO
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
//SI EL VALOR RECIBIDO ES IGUAL A PARAM_INPUT_3 LO ALMACENA EN ESA VARIABLE SUSTITUYENDO SU VALOR PREVIO
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
//SI NO SE GUARDA EL DATO EN LAS VARIABLES CORRESPONDIENTES SE IGUALAN A LOS SIGUIENTES VALORES 
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    //IMPRIMIMOS EL DATO RECIBIDO EN EL MONITOR SERIAL
    Serial.println(inputMessage);

//Ahora tiene el valor que acaba de insertar en el primer formulario guardado en la variable inputMessage .
//Luego, establezca la variable inputParam en PARAM_INPUT_1 para que sepamos de dónde proviene el valor de entrada.
//Cuando envía el formulario, recibe un mensaje que dice el valor que ha insertado y en qué campo. También mostramos un enlace para volver a la URL de la ruta (Página de inicio).
   
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
////Si realiza una solicitud en una URL no válida, llamamos a la función notFound () , definida al comienzo del boceto.  
  server.onNotFound(notFound);

  //Finalmente, inicie el servidor para manejar clientes.
  server.begin();
}

void loop() {
digitalWrite(LED_BUILTIN, LOW);   
  delay(250);                      
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(250);
  

}
