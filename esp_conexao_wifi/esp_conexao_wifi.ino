//Bibilioteca necessária para conectar o wifi
#include <WiFi.h> 

//Definido o pino 32
#define lampada 32
 
//string de mensagem enviada pelo client
String ClientRequest;
 
//IP estático da rede
IPAddress staticIP(192,168,xx,xxx);
//Gateway da rede
IPAddress gateway(192,168,xx,xxx);
//Máscara da rede
IPAddress subnet(255,255,255,0);
 
//Porta de COM server/client
WiFiServer server(80);
 
// Wi-Fi Client
WiFiClient client;
 
//Variável utilizada para solicitar o client 
String myresultat;
String ReadIncomingRequest()
{
  //Enquanto houver dados enviados pelo client 
  while(client.available()) 
  {
    //Atribui para a variável string o comando enviado pelo client
    ClientRequest = (client.readStringUntil('\r'));
 
    //Se existir "HTTP/1.1" na string então recebe comando
    if ((ClientRequest.indexOf("HTTP/1.1")>0))
      myresultat = ClientRequest;
  }
  //Retorna o resultado
  return myresultat;
}
 
void setup()
{
  //Inicializar a varíavel sem dados
  ClientRequest = "";
   
  //Define os pinos de saída
  pinMode(lampada,OUTPUT);
   
  //Inicializa a porta serial com taxa de transmissao 115200 bps
  Serial.begin(115200);
  //aguarda 10ms
  delay(10);
 
  //Conexão do WiFi
  Serial.println("PRONTO!");
 
  //Configuração do SSID e senha da rede
  WiFi.begin("ssid_da_rede", "senha_do_wifi");
 
  //Enquanto não houver conexão exibe "."
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
 
  //Após a conexão exibe "conectado"
  Serial.println("Conectado!");
 
  //Configura ip estático, gateway e máscara da rede (definidos no início do código)
  WiFi.config(staticIP, gateway, subnet);
 
  //Exibe o IP utilizado pelo ESP32
  Serial.println("Seu IP é");
  Serial.println((WiFi.localIP()));
 
  //Inicia COM com servidor
  server.begin();
}
 
void loop()
{
    //Obtém cliente
    client = server.available();
 
    //Se não houver, retorna até que ele realmente exista
    if (!client) 
      return; 
 
    //Enquanto não existir COM com client ele aguarda
    while(!client.available())
      delay(1); 
 
    //Obtém as respostas utilizando a função local ReadIncomingRequest
    ClientRequest = (ReadIncomingRequest());
     
    //Retira os dados da página e obtém apenas o comando enviado
    ClientRequest.remove(0, 5);
    ClientRequest.remove(ClientRequest.length()-9,9);
 
    //Controla as portas conforme o comando recebido
    if (ClientRequest == "acender") 
      digitalWrite(lampada,HIGH);
    if (ClientRequest == "apagar")
      digitalWrite(lampada,LOW);
 
    //Exibe na página a palavra "OK", caso acessado por um navegador
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("OK");
    client.println("</html>");
    client.flush();
    client.stop();
    delay(1);
}
