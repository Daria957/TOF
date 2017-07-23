    #include <SoftwareSerial.h>
     
    SoftwareSerial gprsSerial(7, 8);
     
    void setup()
    {
        gprsSerial.begin(19200);
        Serial.begin(115200);

        pinMode(LED_BUILTIN, OUTPUT);
     
        // Настраиваем приём сообщений с других устройств
        // Между командами даём время на их обработку
        gprsSerial.print("AT+CMGF=1\r");
        delay(300);
        gprsSerial.print("AT+IFC=1, 1\r");
        delay(300);
        gprsSerial.print("AT+CPBS=\"SM\"\r");
        delay(300);
        gprsSerial.print("AT+CNMI=1,2,2,1,0\r");
        delay(500);
    }
     
    String currStr = "";
    // Переменная принимает значение True, если текущая строка является сообщением
    boolean isStringMessage = false;
     
    void loop()
    {
        if (!gprsSerial.available())
            return;
     
        char currSymb = gprsSerial.read();    
        if ('\r' == currSymb) {
            if (isStringMessage) {
                //если текущая строка - SMS-сообщение,
                //отреагируем на него соответствующим образом
                Serial.println(currStr);
                if (!currStr.compareTo("on"))
                {
                    digitalWrite(LED_BUILTIN, HIGH);
                }
                else if (!currStr.compareTo("off"))
                {
                    digitalWrite(LED_BUILTIN, LOW);
                }
                isStringMessage = false;
            } else {
                if (currStr.startsWith("+CMT")) {
                    //если текущая строка начинается с "+CMT",
                    //то следующая строка является сообщением
                    isStringMessage = true;
                }
            }
            currStr = "";
        } else if ('\n' != currSymb) {
            currStr += String(currSymb);
        }
    }


