#include <ViviLib.h>

unsigned char data[2];
//ButtonのIDが0x02
char idPacket[] = {0x00, 0x02};
char fw_version = 0x01;
const int buttonPin = 2;
int preValue = 0;

ViviLib vivilib(fw_version, idPacket, 0);

void setup() {
  //シリアル通信開始（デバッグ用）
  Serial.begin(9600);
  Serial.println("Hello");

  //ライブラリの初期化
  vivilib.init();
  data[0] = 0x01;
  data[1] = 0x01;

  //D2ピンを入力に設定
  pinMode(buttonPin, INPUT);
}

void loop() {
  //VIVIWARE COREと接続されているか？
  if (vivilib.connected()) {

    //ボタン（D2ピン）の状態をチェックする
    //ボタンが押されている: curValue = 1
    //ボタンが押されていない: preValue = 0
    //int curValue = (0x01 & ~digitalRead(buttonPin));
    int curValue = (digitalRead(buttonPin)) ? 0 : 1;

    //前回からボタンの状態が変更されているか？
    if (curValue != preValue) {
      Serial.print("Send: ");
      Serial.println(curValue);

      //ボタンの状態をVIVIWARE COREに送信
      data[1] = curValue;
      vivilib.sendData(data, 2);

      //今回のボタンの状態を記録しておく
      preValue = curValue;
    }
  }
  delay(50);
}

