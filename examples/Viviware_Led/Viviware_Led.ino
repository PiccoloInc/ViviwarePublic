#include <ViviLib.h>

unsigned char data[64];
//LEDのIDが0x01
char idPacket[] = {0x00, 0x01};
char fw_version = 0x01;
const int ledPin = 5;
bool flag = LOW;
int brightness = 0;

ViviLib vivilib(fw_version, idPacket, 0);

void setup() {
  //シリアル通信開始（デバッグ用）
  Serial.begin(9600);
  Serial.println("Hello, I'm LED module.");

  //ライブラリの初期化
  vivilib.init();

  //D5ピンを出力に設定
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

}

void loop() {
  //VIVIWARE COREと接続されているか？
  if(vivilib.connected()){
    
    //VIVIWARE COREからデータが来ているか？
    if(vivilib.dataAvailable()){
      Serial.println("Data Available");
      
      //受け取ったデータの長さを取得
      int datalen = vivilib.recvData(data);
      int i = 0;
      while(i < (datalen - 1)){
        //受け取ったデータの種類(Data Code)を取得
        //dc = 01：ONOFF情報
        //dc = 02：輝度情報
        int dc = data[i];
        i++;

        switch(dc){
          case 0x01:
            // ONOFF情報, LED ONならflag = 1
            flag = data[i];
            i++;
            Serial.print("flag: ");
            Serial.println(flag);
            break;

          case 0x02:
            // 輝度情報: 大きいほうが明るい（0から255)
            brightness = data[i];
            i++;
            Serial.print("brightness: ");
            Serial.println(brightness);
            break;
            
          default:
            Serial.println("Data Code Error");
            i += datalen;
            break;   
        }
      }
      
      if(flag){
        //LEDを輝度に合わせて点灯
        analogWrite(ledPin, brightness);
      }else{
        //消灯（ = 輝度0）
        analogWrite(ledPin, 0);
      }
      
    }  
  }
}
