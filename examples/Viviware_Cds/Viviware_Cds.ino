#include <ViviLib.h>

unsigned char data[3];
//ButtonのIDが0x02
char idPacket[] = {0x00, 0x11};
char fw_version = 0x01;
const int detectPin = A0;
int preValue = 0;

ViviLib vivilib(fw_version, idPacket, 0);

void setup() {
  //シリアル通信開始（デバッグ用）
  Serial.begin(9600);
  Serial.println("Hello, I'm Cds module.");

  //ライブラリの初期化
  vivilib.init();
  data[0] = 0x01;
  data[1] = 0x00;
  data[2] = 0x00;

}

void loop() {
  
  //VIVIWARE COREと接続されているか？
  if (vivilib.connected()) {

    //Cdsセンサー（照度）の値を読む
    int curValue =  analogRead(detectPin);

    //照度に変化があるか？
    if (curValue != preValue) {
      Serial.print("Send: ");
      Serial.println(curValue);

      //照度をVIVIWARE COREに送信
      data[1] = (curValue >> 8) & 0xFF;
      data[2] = curValue & 0xFF;
      vivilib.sendData(data, 3);

      //今回の照度を記録しておく
      preValue = curValue;
    }
  }
  
  delay(50);
  
}

