#include <MeccaBrain.h>
const int pwmPin1 = 9; //Designacion de Pin para la cadena 1. Brazo Izquierdo
const int pwmPin2 = 10; //Designacion de Pin para la cadena 2. Cabeza.
const int pwmPin3 = 11; //Designacion de Pin para la cadena 3. Brazo derecho
//Medicion de tiempo
unsigned long tant = 0;
byte seg = 0;
int con_a = 0;
//Motores DC
int giro_PI = 52; //giro pierna izquierda
int contrario_PI = 50; //giro inverso pierna izquierda
int giro_PD = 26; // Giro pierna deracha
int contrario_PD = 28; //Giro inverso pierna derecha
int velocidad_PI = 7; //Pin de control de velocidad pwm
int velocidad_PD = 3; // pin de control de velocidad pwm
int stby = 30; // pin en ALTO para puente H, sin esto nada funciona en el puente.
bool posInicial = true;
char c; // entrada de caracteres del monitor serial.
String readString; // cadena de caracteres para el control del servo en modo libre.
// Desigancion de cadenas
MeccaBrain chain1(pwmPin1);
MeccaBrain chain2(pwmPin2);
MeccaBrain chain3(pwmPin3);

void setup() {
  Config_Tarjeta();
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Programa principal
  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void loop() {
  if (posInicial == true) {
    PosicionInicial(25, 230, 215, 25, 120, 120, 120);
    posInicial = false;
  }
  if (Serial.available() > 0) {
    c = Serial.read();
    if (c == '1') { //abrazo
      con_a ++;
      if (con_a >= 50) {
        abrazo();
        con_a = 0;
      }
    }
    if (c == '2') { //box
      con_a ++;
      if (con_a >= 50) {
        box();
        con_a = 0;
      }
    }
    if (c == '3') { //caminar
      con_a ++;
      if (con_a >= 50) {
        caminar();
        con_a = 0;
      }
    }
    if (c == '4') { //movimientos con funciones preestablesidas, en este caso  ESTIRANDO dormir!
      con_a ++;
      if (con_a >= 50){
        estirando();
        con_a =0;
      }
    }
    if (c == '5') { //movimientos con funciones preestablesidas, en este caso SALUDA! Hola
      con_a ++;
      if (con_a >= 50) {
        saludar();
        con_a = 0;
      }
    }
    if (c == '6') { //movimientos con funciones preestablesidas, en este caso DICE YO! Participar
      con_a ++;
      if (con_a >= 50) {
        yo();
        con_a = 0;
      }
    }
    if (c == '7') { //asustado
      con_a ++;
      if (con_a >= 50) {
        susto();
        con_a = 0;
      }
    }
    if (c == '8') {
      con_a ++;
      if (con_a >= 50) {
        saludo_de_mano();
        con_a = 0;
      }
    }
  }
}

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Funciones de Inicializacion
  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void Config_Tarjeta() {
  // designacion como salidas de los canales.
  pinMode (pwmPin1, OUTPUT);
  pinMode (pwmPin2, OUTPUT);
  pinMode (pwmPin3, OUTPUT);
  // moteres DC
  pinMode(giro_PI, OUTPUT);
  pinMode(contrario_PI, OUTPUT);
  pinMode(giro_PD, OUTPUT);
  pinMode(contrario_PD, OUTPUT);
  pinMode(velocidad_PI, OUTPUT);
  pinMode(velocidad_PD, OUTPUT);
  pinMode(stby, OUTPUT);
  Serial.begin(57600);
  delay(100);
  BuscarServos();
  Serial.println("Tarjeta Configurada");
}

void BuscarServos() {
  for (int k  = 0; k <= 50; k++) {
    chain1.communicate();
    chain2.communicate();
    chain3.communicate();
  }
  delay(1900);
  Serial.println("Busqueda completa");
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Funciones de basicas
  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void HombroDerRotar(byte _posicion) {
  chain3.setServoPosition(0, _posicion);
  chain3.communicate();
}

void HombroIzqRotar(byte _posicion) {
  chain1.setServoPosition(0, _posicion);
  chain1.communicate();
}

void HombroDer(byte _posicion) {
  chain3.setServoPosition(1, _posicion);
  chain3.communicate();
}

void HombroIzq(byte _posicion) {
  chain1.setServoPosition(1, _posicion);
  chain1.communicate();
}

void CodoDer(byte _posicion) {
  chain3.setServoPosition(2, _posicion);
  chain3.communicate();
}

void CodoIzq(byte _posicion) {
  chain1.setServoPosition(2, _posicion);
  chain1.communicate();
}

void GirarRostro(byte _posicion) {
  chain2.setServoPosition(0, _posicion);
  chain2.communicate();
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Funciones de movimiento de los brazos
  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void PosicionInicial(byte _HomDerRotar, byte _HomIzqRotar, byte _HomDer, byte _HomIzq, byte _CodoDer, byte _CodoIzq, byte _Rostro) {
  HombroDerRotar(_HomDerRotar);
  HombroIzqRotar(_HomIzqRotar);
  HombroDer(_HomDer);
  HombroIzq(_HomIzq);
  CodoDer(_CodoDer);
  CodoIzq(_CodoIzq);
  GirarRostro(_Rostro);
}

void FlexionCodo(char _Mano){
  //Flexionar codo a posicion inicial
  for (int k = 200; k >= 120; k -= 10) {
    if(_Mano == 'D'){
      CodoDer(k);  
    }
    else{
      CodoIzq(k);  
    }
  }
  //Flexionar codo
  for (int k = 120; k <= 200; k += 10) {
    if(_Mano == 'D'){
      CodoDer(k);  
    }
    else{
      CodoIzq(k);  
    }
  }    
}

void SeleccionarMano(char _Mano) {
  if (_Mano == 'D' ) {
    for (int k = 120; k <= 200; k += 10) {
      CodoDer(k);
    }
    //Subir brazo
    for (int k = 20; k <= 150; k += 10) {
      HombroDerRotar(k);
    }
    //Abrir brazo
    for (int k = 215; k >= 150; k -= 5) {
      HombroDer(k);
    }
    FlexionCodo('D');
    //Ver Derecha
    for (int k = 120; k <= 150; k += 10) {
      GirarRostro(k);
    }
    FlexionCodo('D');
    //Ver Izquierda
    for (int k = 150; k >= 60; k -= 10) {
      GirarRostro(k);
    }
    FlexionCodo('D');
    //Ver Derecha
    for (int k = 60; k <= 150; k += 10) {
      GirarRostro(k);
    }
    //Rostro a posicion inicial
    for (int k = 150; k >= 120; k -= 5) {
      GirarRostro(k);
    }
    //Cerrar brazo a posicion inicial
    for (int k = 150; k <= 215; k += 5) {
      HombroDer(k);
    }
    //Bajar brazo a posicion inicial
    for (int k = 150; k >= 25; k -= 5) {
      HombroDerRotar(k);
    }
    //Flexionar codo a posicion inicial
    for (int k = 200; k >= 120; k -= 5) {
      CodoDer(k);
    }
  }
  if (_Mano == 'I') {
    for (int k = 120; k <= 200; k += 10) {
      CodoIzq(k);
    }
    //Subir brazo
    for (int k = 230; k >= 40; k -= 10) {
      HombroIzqRotar(k);
    }
    //Abrir brazo
    for (int k = 25; k <= 90; k += 5) {
      HombroIzq(k);
    }
    FlexionCodo('I');
    //Ver Derecha
    for (int k = 120; k <= 150; k += 10) {
      GirarRostro(k);
    }
    FlexionCodo('I');
    //Ver Izquierda
    for (int k = 150; k >= 60; k -= 10) {
      GirarRostro(k);
    }
    FlexionCodo('I');
    //Ver Derecha
    for (int k = 60; k <= 150; k += 10) {
      GirarRostro(k);
    }
    //Rostro a posicion inicial
    for (int k = 150; k >= 120; k -= 5) {
      GirarRostro(k);
    }
    //Cerrar brazo a posicion inicial
    for (int k = 90; k >= 25; k -= 5) {
      HombroIzq(k);
    }
    //Bajar brazo a posicion inicial
    for (int k = 40; k <= 215; k += 5) {
      HombroIzqRotar(k);
    }
    //Flexionar codo a posicion inicial
    for (int k = 200; k >= 120; k -= 5) {
      CodoIzq(k);
    }
  }
}
//Bueno
void saludar() {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  //Avanzar
  adelante();
  delay(2000);
  //Parar
  detener();
  //Seleccionar mano con la cual se hara el saludo
  SeleccionarMano('D');
  //Retroceder
  atras();
  delay(2000);
  //Parar
  detener();
  posInicial = true;
}
//Se requiere Mejorar
void estirando() {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  //Subir brazos
  for (int der = 25, izq = 230; der <= 230; der += 5, izq -= 5) {
    HombroDerRotar(der);
    HombroIzqRotar(izq);
  }
  //Abrir brazos
  for (int der = 215, izq = 25; der >= 180; der --, izq ++) {
    HombroDer(der);
    HombroIzq(izq);
  }
  //Cerrar brazos
  for (int der = 180, izq = 60; der <= 215; der ++, izq --) {
    HombroDer(der);
    HombroIzq(izq);
  }
  //Bajar brazos
  for (int der = 230, izq = 25; der >= 25; der -= 5, izq += 5) {
    HombroDerRotar(der);
    HombroIzqRotar(izq);
  }
  delay(1000);
  posInicial = true;
}

void participarManoDer(){
  HombroDerRotar(250);
  delay(600);
  //Flexionar codo
  for (int k = 120; k <= 180; k +=10 ) {
    CodoDer(k);
  }
  delay(2000);
  //Bajar brazo a posicion inicial
  for (int k = 250; k >= 25; k -= 5) {
    HombroDerRotar(k);
  }
  //Flexionar codo a posicion inicial
  for (int k = 180; k >= 120; k --) {
    CodoDer(k);
  }

}

void participarManoIzq(){
  //Subir brazo
  HombroIzqRotar(0);
  delay(600);
  //Flexionar codo
  for (int k = 160; k >= 110; k --) {
    CodoIzq(k);
  }
  delay(2000);
  //Bajar brazo a posicion inicial
  for (int k = 0; k <= 230; k += 5) {
    HombroIzqRotar(k);
  }
  //Flexionar codo a posicion inicial
  for (int k = 110; k <= 160; k ++) {
    CodoIzq(k);
  }
}

//Excelente con mano derecha
void yo() {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  //participarManoIzq();
  participarManoDer();
  posInicial = true;
}
//Excelente
void saludo_de_mano () {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  for (int k = 120; k <= 180; k += 10) {
    CodoDer(k);
  }
  delay(200);
  for (int k = 25; k <= 120; k += 10) {
    HombroDerRotar(k);
  }
  delay(2500);
  for (int k = 120; k >= 80; k -= 5) {
    HombroDerRotar(k);
  }
  for (int k = 80; k <= 120; k += 5) {
    HombroDerRotar(k);
  }
  for (int k = 120; k >= 80; k -= 5) {
    HombroDerRotar(k);
  }
  for (int k = 80; k <= 120; k += 5) {
    HombroDerRotar(k);
  }
  for (int k = 120; k >= 80; k -= 5) {
    HombroDerRotar(k);
  }
  for (int k = 80; k <= 120; k += 5) {
    HombroDerRotar(k);
  }
  for (int k = 120; k >= 80; k -= 5) {
    HombroDerRotar(k);
  }
  for (int k = 80; k <= 120; k += 5) {
    HombroDerRotar(k);
  }
  for (int k = 120; k >= 25; k -= 5) {
    HombroDerRotar(k);
  }
  for (int k = 180; k >= 120; k -= 5) {
    CodoDer(k);
  }
  posInicial = true;
}
//Bueno
void susto () {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);

  for (int j = 0, k = 220; j <= 220; j += 110, k -= 110) {
    HombroIzqRotar(k);
    HombroDerRotar(j);
  }
  for (int j = 220, k = 20; j >= 20; j -= 5, k += 5) {
    HombroDerRotar(j);
    HombroIzqRotar(k);
  }
  posInicial = false;
}
//Bueno
void abrazo () {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  //Extender brazos
  for (int i = 220, j = 20 , k = 0, l = 230; i >= 100 ; i -= 10, j += 10, k += 10, l -= 10 ) {
    HombroIzqRotar(i);
    HombroIzq(j);
    HombroDerRotar(k);
    HombroDer(l);
  }
  delay(2000);
  //Abrazar
  for (int i = 120, j = 120; i >= 0; i -= 10, j += 10 ) {
    HombroIzq(i);
    HombroDer(j);
  }
  delay(2000);
  //Regresar a posicion de abrazo
  for (int i = 0, j = 240; i <= 120; i += 10, j -= 10 ) {
    HombroIzq(i);
    HombroDer(j);
  }
  //Regresar a posicion inicial
  for (int i = 120, j = 140, k = 120, l = 110; i <= 220 ; i += 10, j -= 10, k -= 10, l += 10 ) {
    HombroIzqRotar(i);
    HombroIzq(j);
    HombroDerRotar(k);
    HombroDer(l);
  }
  posInicial = true;
}

void GolpeDerecho() {
  for (int i = 20, j = 200; i <= 180; i += 20, j -= 10) {
    if (j <= 120) {
      j = 120;
    }
    HombroDerRotar(i);
    CodoDer(j);
  }
  //Posicion original
  for (int i = 180, j = 120; i >= 20; i -= 20, j += 10) {
    if (j >= 200) {
      j = 200;
    }
    HombroDerRotar(i);
    CodoDer(j);
  }
}

void GolpeIzquierdo() {
  for (int i = 230, j = 200; i >= 70; i -= 20, j -= 10) {
    if (j <= 120) {
      j = 120;
    }
    HombroIzqRotar(i);
    CodoIzq(j);
  }
  //Posicion original
  for (int i = 70, j = 120; i <= 230; i += 20, j += 10) {
    if (j >= 200) {
      j = 200;
    }
    HombroIzqRotar(i);
    CodoIzq(j);
  }
}
//Mejorar
void box () {
  PosicionInicial(25, 230, 215, 25, 120, 120, 120);
  for (int i = 120, j = 120; i <= 200; i += 10, j += 10) {
    CodoDer(i);
    CodoIzq(j);
  }
  delay(1000);
  for (int i = 1; i <= 2; i++) {
    GolpeDerecho();
    GolpeIzquierdo();
  }
  for (int i = 200, j = 200; i >= 120; i -= 5, j -= 5) {
    CodoDer(i);
    CodoIzq(j);
  }
  posInicial = true;
}

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Funciones de desplazamiento
  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void adelante() {
  digitalWrite(stby, HIGH);
  digitalWrite(giro_PI, HIGH);
  digitalWrite(contrario_PI, LOW);
  digitalWrite(giro_PD, HIGH);
  digitalWrite(contrario_PD, LOW);
  analogWrite(velocidad_PI, 127);
  analogWrite(velocidad_PD, 131);
}

void atras() {
  digitalWrite(stby, HIGH);
  digitalWrite(giro_PI, LOW);
  digitalWrite(contrario_PI, HIGH);
  digitalWrite(giro_PD, LOW);
  digitalWrite(contrario_PD, HIGH);
  analogWrite(velocidad_PI, 127);
  analogWrite(velocidad_PD, 127);
}

void atrasD() {
  digitalWrite(stby, HIGH);
  digitalWrite(giro_PI, LOW);
  digitalWrite(contrario_PI, LOW);
  digitalWrite(giro_PD, LOW);
  digitalWrite(contrario_PD, HIGH);
}

void derecha() {
  digitalWrite(stby, HIGH);
  digitalWrite(giro_PI, HIGH);
  digitalWrite(contrario_PI, LOW);
  digitalWrite(giro_PD, LOW);
  digitalWrite(contrario_PD, HIGH);
  analogWrite(velocidad_PI, 127);
  analogWrite(velocidad_PD, 127);
}

void izquierda() {
  digitalWrite(stby, HIGH);
  digitalWrite(giro_PI, LOW);
  digitalWrite(contrario_PI, HIGH);
  digitalWrite(giro_PD, HIGH);
  digitalWrite(contrario_PD, LOW);
  analogWrite(velocidad_PI, 127);
  analogWrite(velocidad_PD, 127);
}

void detener() {
  digitalWrite(giro_PI, LOW);
  digitalWrite(contrario_PI, LOW);
  digitalWrite(giro_PD, LOW);
  digitalWrite(contrario_PD, LOW);
  Serial.println("detenido");
}

void caminar() {
  CodoDer(180);
  CodoIzq(180);
  delay(100);
  adelante();
  int ca = 1;
  seg = 0;
  while (ca == 1) {
    unsigned long tact = millis();
    if (tact >= 1000) {
      seg ++;
      tant = tact;
    }
    if (seg >= 3) {
      ca += 1;
      detener();
      seg = 0;
      posInicial = true;
      break;
    }
    for (int i = 40, j = 190; i >= 0; i -= 10, j -= 10) {
      HombroDerRotar(i);
      HombroIzqRotar(j);
    }
    for (int i = 0, j = 150; i <= 90; i += 10, j += 10) {
      HombroDerRotar(i);
      HombroIzqRotar(j);
    }
    for (int i = 90, j = 240; i >= 40; i -= 10, j -= 10) {
      HombroDerRotar(i);
      HombroIzqRotar(j);
    }
  }
  Serial.println("terminado");
}
