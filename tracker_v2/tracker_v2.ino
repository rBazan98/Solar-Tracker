#include <Servo.h>

//declarando los servos y su respectivo pin de control
Servo horizontal_servo;
Servo vertical_servo;
int h = 9;
int v = 10;


//declarando variables para los LDR
int top_left=100;
int top_rigth=100;
int bottom_left=100;
int bottom_rigth=100;

//declarando variables de posicion horizontal y vertical
int pos_h=90;
int pos_v=90;

//funcion para calcular el promedio de dos entradas
int mean(int a, int b){
  int c = (a + b)/2;
  return c;
}

void horizontal_move(){
  if (mean(top_left,bottom_left) > mean(bottom_rigth,top_rigth)){
  if (pos_h<180) pos_h=+1;
} else {
  if (pos_h>0) pos_h=-1;
}
  horizontal_servo.write(pos_h);  
}

void vertical_move(){
  if (mean(top_left,top_rigth) > mean(bottom_left,bottom_left)){
  if (pos_v<180) pos_v=+1;
} else {
  if (pos_v>0) pos_v=-1;
}
  vertical_servo.write(pos_v);
}


void setup() {

//declaramos el pin de salida para el servo horizontal
pinMode(h,OUTPUT);
horizontal_servo.attach(h);

//declaramos el pin de salida para el servo vertical
pinMode(v,OUTPUT);
vertical_servo.attach(v);

//desplazamiento inicial a la dirección del sol
for (int i=0;i<=90;i++){
  vertical_move();
  horizontal_move();
  delay(15);
  }

}

void loop() {
//leemos los valores de las resistencias
int top_left=analogRead(A0);
int top_rigth=analogRead(A1);
int bottom_left=analogRead(A2);
int bottom_rigth=analogRead(A3);


  vertical_move();
  horizontal_move();
//retraso de 16 segundos
delay(16000);

}
