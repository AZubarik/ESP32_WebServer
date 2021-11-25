#include <main.h>

// для хранения текущего состояния выходных GPIO-контактов:
String output26State = "off";
String output27State = "off";

// Задаем номера для выходных GPIO-контактов:
const int output26 = 26;
const int output27 = 27;

void setup() {
  Serial.begin(115200);
  // Переключаем выходные GPIO-контакты в режим «OUTPUT»:
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Задаем для выходных GPIO-контактов состояние «LOW»:
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Настраиваем статический IP-адрес:
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");                     //  "Не удалось задать статический IP-адрес"
  }
  
  // Подключаемся к WiFi-сети с помощью заданных выше SSID и пароля: 
  Serial.print("Connecting to ");  //  "Подключаемся к "
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Печатаем локальный IP-адрес и запускаем веб-сервер:
  Serial.println("");
  Serial.println("WiFi connected.");  //  "Подключились к WiFi."
  Serial.println("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();  // Запускаем прослушку входящих клиентов.

  if (client) {                            // Если подключился новый клиент,
    Serial.println("New Client.");         // пишем в монитор порта сообщение об этом.
    String currentLine = "";               // Создаем строку для хранения данных, пришедших от клиента.
    while (client.connected()) {           // Цикл while() будет работать, пока клиент подключен.
      if (client.available()) {            // Если у клиента есть байты для чтения,
        char c = client.read();            // считываем байт
        Serial.write(c);                   // и пишем его в мониторе порта.
        header += c;
        if (c == '\n') {                   // если считанный байт – это символ новой строки,
          // и если получили два символа новой строки подряд, то это значит, что текущая строка пуста.
          // Это конец HTTP-запроса клиента, поэтому шлем ответ:
          if (currentLine.length() == 0) {
            // HTTP-заголовки всегда начинаются с кода ответа (например, с «HTTP/1.1 200 OK») и типа контента,
            // чтобы клиент знал, что получает. Затем пишем пустую строку.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");    //  "Соединение: отключено"
            client.println();
            
            // Включаем и выключаем GPIO-контакты:
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");  //  "GPIO26 включен"
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");  //  "GPIO26 выключен"
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");  //  "GPIO27 включен"
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");  //  "GPIO27 выключен"
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            // Показываем HTML-страницу:
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // При помощи CSS задаем стили для кнопок «ON» и «OFF». 
            // Не бойтесь экспериментировать, поиграйтесь
            // с атрибутами «background-color» и «font-size»,
            // чтобы настроить дизайн кнопок по своему вкусу.
            client.println("<style> body { background: #333; /* Цвет фона */}");
            client.println("h1 {color: rgb(0, 191, 255);font: italic small-caps bold 40px Helvetica Neue, Helvetica, Arial, sans-serif; font-size: 30px; text-shadow: 3px 6px 2px rgba(0, 0, 0, .3); word-spacing: .1em; left: 14px; position: relative;}  ");
            client.println(".buttonEnable {background-color: #109ff1; border: none; border-radius: 10px; color: white; padding: 14px 18px; text-shadow: 2px 2px 2px rgba(0, 0, 0, .3); text-decoration: none; font-size: 20px; margin: 10px; cursor: pointer; }");
            client.println(".buttonDisable { background-color: #abafb1; border: none; border-radius: 10px; color: white; padding: 14px 18px; text-shadow: 2px 2px 2px rgba(0, 0, 0, .3); text-decoration: none; font-size: 20px; margin: 10px; cursor: pointer;}");
            client.println("</style> </head>");
            // Заголовок веб-страницы:
            client.println("<body><h1>ESP32 LED</h1>");
            
            // Показываем текущее состояние
            // и кнопку «ON»/«OFF» для контакта GPIO26:
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // Если значением в «output26State» является «off»,
            // то показываем кнопку «ON»: 
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class = buttonEnable>Enable</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class = buttonDisable>Disable</button></a></p>");
            } 
               
           // Показываем текущее состояние
            // и кнопку «ON»/«OFF» для контакта GPIO27: 
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // Если значением в «output27State» является «off»,
            // то показываем кнопку «ON»: 
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=buttonEnable>Enable</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=buttonDisable>Disable</button></a></p>");
            }
            client.println("</body></html>");
            
            // HTTP-ответ заканчивается еще одной пустой строкой:
            client.println();
            // выходим из цикла while():
            break;
          } else {  // Если получили символ новой строки, то очищаем переменную «currentLine»
            currentLine = "";
          }
        } else if (c != '\r') {  // если получили что-либо, кроме символа возврата каретки,
          currentLine += c;      // добавляем это что-то в конец переменной «currentLine» 
        }
      }
    }
    // Очищаем переменную «header»:
    header = "";
    // Отключаем соединение:
    // client.stop();
    // Serial.println("Client disconnected."); //  "Клиент отключился."
    Serial.println("");
  }
}