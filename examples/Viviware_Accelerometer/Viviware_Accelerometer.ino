#include <ViviLib.h>

const int MAXVAL = 724;
const int MINVAL = 277;
//取得データの平均回数
const int AVGTIME = 50; 
float degval;

unsigned char data[16];
//ButtonのIDが0x02
char idPacket[] = {0x00, 0x12};
char fw_version = 0x01;
const int xaccelPin = A0;
const int yaccelPin = A1;
const int zaccelPin = A2;
int preValue = 0;

ViviLib vivilib(fw_version, idPacket, 0);

void setup() {
  //シリアル通信開始（デバッグ用）
  Serial.begin(9600);
  Serial.println("Hello, I'm Accelerometer module.");
  degval = (float) (MAXVAL - MINVAL) / 180;
  Serial.println(degval);
  
  //ライブラリの初期化
  vivilib.init();
  data[0] = 0x01;
  data[1] = 0x00;
  data[2] = 0x02;
  data[3] = 0x00;
  data[4] = 0x03;
  data[5] = 0x00;

}

void loop() {
  //VIVIWARE COREと接続されているか？
  if (vivilib.connected()) {

  long x_val = 0;
  long y_val = 0;
  long z_val = 0;
  
  //AVGTIME回（50回）データを取得して足し合わせる
  for(int i = 0; i < AVGTIME; i++){
    x_val += analogRead(xaccelPin);
    y_val += analogRead(yaccelPin);
    z_val += analogRead(zaccelPin);
  }
  
  //平均値を出すために、足し合わせた値を取得回数(50回)で割る
  x_val /= AVGTIME;
  y_val /= AVGTIME;
  z_val /= AVGTIME;

/*
  Serial.print("x val: ");
  Serial.print(x_val);
  Serial.print(", y val: ");
  Serial.print(y_val);
  Serial.print(", z val: ");
  Serial.println(z_val);
*/
  
  //センサーの値から水平面上に対しての各軸の傾きを算出
  int x_degree = (x_val - MINVAL) / degval - 90;
  int y_degree = (y_val - MINVAL) / degval - 90;
  int z_degree = (z_val - MINVAL) / degval - 90;

  if(x_degree < -90){
    x_degree = -90;
  }else if(x_degree > 90){
    x_degree = 90;
  }

  if(y_degree < -90){
    y_degree = -90;
  }else if(y_degree > 90){
    y_degree = 90;
  }  

  if(z_degree < -90){
    z_degree = -90;
  }else if(z_degree > 90){
    z_degree = 90;
  }
    
  Serial.print(x_degree);
  Serial.print(", ");
  Serial.print(y_degree);
  Serial.print(", ");
  Serial.println(z_degree);

  //角度範囲を[-90 - +90]から[0 - 180]に変換
  data[1] = x_degree + 90; 
  data[3] = y_degree + 90;
  data[5] = z_degree + 90;
  vivilib.sendData(data, 6);

  }
  delay(50);
}

