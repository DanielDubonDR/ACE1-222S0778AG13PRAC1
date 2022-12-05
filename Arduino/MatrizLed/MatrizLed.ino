#include <LedControl.h>

//Potenciometro
#define pot A0

//Filas y Columnas
const int columna[8] = {22, 23, 24, 25, 26, 27, 28, 29};
const int fila[8] = {30, 32, 32, 33, 34, 35, 36, 37};

//Pines de botones
int contador=0;
int btnDown=38;
int btnUp=39;
int btnRotate=40;
int btnStart=41;

//Variables de estado
int velocidad;
bool direccion = false;
bool activo = false;


//Declaracion de matriz con driver
LedControl lc=LedControl(12, 11, 10,0);

int position=0;

//TIEMPO PRESIONADO EL BOTON
unsigned long tiempo = 0;
boolean cambio = false;

//VARIABLE PARA SABER ENTORNO, 0->LETRERO, 1->JUEGO, 2-> PAUSA
int entorno = 0;

// Variables De Juego
int i = 0; // to manipulate up and down
int rotIndex = 0; // 0 = 0; 1 = 90; 2 = 180; 3 = 270; Degress
int figura;
int siguiente;
boolean pausa = false;
boolean error = false;
boolean pp = false;
int punteo = 0;

//Frase a mostrar
  int Frase [8] [176] ={
    {0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
 };

 int Frase2 [8] [184] ={
    {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
 };
 
void setup() {
  Serial.begin(9600);
  pinMode(btnDown, INPUT);
  pinMode(btnUp, INPUT);
  pinMode(btnRotate, INPUT);
  pinMode(btnStart, INPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(columna[i], OUTPUT);
    pinMode(fila[i], OUTPUT);
    digitalWrite(fila[i], HIGH);
    digitalWrite(columna[i], LOW);
  }

  lc.shutdown(0,false);
  lc.setIntensity(0,5);
  lc.clearDisplay(0);

  //Seleccionar Figura Aleatoria
  figura = random(0,6);
  siguiente = 4;
}

void SeleccionarFila(int f)
{
  if (f==1) digitalWrite(fila[0],LOW); else digitalWrite(fila[0],HIGH);
  if (f==2) digitalWrite(fila[1],LOW); else digitalWrite(fila[1],HIGH);
  if (f==3) digitalWrite(fila[2],LOW); else digitalWrite(fila[2],HIGH);
  if (f==4) digitalWrite(fila[3],LOW); else digitalWrite(fila[3],HIGH);
  if (f==5) digitalWrite(fila[4],LOW); else digitalWrite(fila[4],HIGH);
  if (f==6) digitalWrite(fila[5],LOW); else digitalWrite(fila[5],HIGH);
  if (f==7) digitalWrite(fila[6],LOW); else digitalWrite(fila[6],HIGH);
  if (f==8) digitalWrite(fila[7],LOW); else digitalWrite(fila[7],HIGH);
}

void ActivarLedColumna(int c, int estado)
{
  if (c==1) digitalWrite(columna[0],estado); 
  if (c==2) digitalWrite(columna[1],estado); 
  if (c==3) digitalWrite(columna[2],estado); 
  if (c==4) digitalWrite(columna[3],estado); 
  if (c==5) digitalWrite(columna[4],estado); 
  if (c==6) digitalWrite(columna[5],estado); 
  if (c==7) digitalWrite(columna[6],estado); 
  if (c==8) digitalWrite(columna[7],estado);
}

void loop() {
  velocidad = analogRead(pot)/4;
  if(entorno == 0){
    // *************************** LETRERO **************************  
    if(digitalRead(btnDown) == HIGH)
    {
      Serial.println("btnDown Presionado");
      direccion = false;
    }
    
    if(digitalRead(btnUp) == HIGH)
    {
      Serial.println("btnUp Presionado");
      if(direccion)
      {
        direccion=false;
      }
      else
      {
        direccion=true;
      }
    }
  
    if(digitalRead(btnStart) == HIGH)
    {
      Serial.println("btnStart Presionado");
      tiempo = millis();    
      while(digitalRead(btnStart) == HIGH){
        if(millis() > tiempo + 3000){
          tiempo = millis();
          Serial.println("Ya pasaron 3 seg");
          entorno = 1;
          cambio = true;         
          break;
        }
      }
      
      if(activo){
        activo=false;
      }else{
        activo=true;
      }
    }

    if(digitalRead(btnRotate) == HIGH)
    {
      Serial.println("btnRotate Presionado");
      //entorno = 1;
    }
    
    for (int j=0;j<8;j++)
    {
      for (int i=0;i<8;i++)
      {
        lc.setLed(0,j,i,Frase2[j][(i+position)-abs((i+position)/176)*176]);
      }
    }
      
    for (int j=0;j<8;j++)
    {
      SeleccionarFila(j+1);
      for (int i=0;i<8;i++)
      {
        ActivarLedColumna(i+1,Frase[j][(i+position)-abs((i+position)/176)*176]);
      }
      delay(1);
    }
   
    if(direccion==true && activo == true)
    {
      position = position-1;
    }
    else if (direccion==false && activo == false)
    {
      position=position+1;
    }
    else
    {
      if(direccion)
      {
        position=position+1;
      }
      else
      {
        position=position-1;
      }
    }
    delay(velocidad);
    // ****************************FIN DEL LETRERO*****************
  }else if(entorno == 1){
    // ****************************INICIO JUEGO*****************      
    resetTotal();
    if(cambio){
      delay(1000);
      cambio = false;
      Serial.println("Start Para Empezar Juego");
    } 
    if(digitalRead(btnStart) == HIGH){
      Serial.println("Iniciando Juego");
      entorno = 2;
    }
    delay(velocidad);
  }else if(entorno == 2){
    // **************************** TETRIX *****************
    for (int i = 7; i > 0 ; i--) {
      if(entorno != 2){
        break;
      }
      createFigures(figura, i, false);    
    }
  
    for(int j = 7; j > 0; j--){
      if(entorno != 2){
        break;
      }
      createFigures(figura, j, true);
    }
    figura = siguiente;
    siguiente = random(0,6);
  }
  
}

//**************************** Creador De Figuras **************************
void createFigures(int fNumber, int j, bool hasDriver) {
  int t = 0;
  while ( t < 400) {
    validate();
    if (hasDriver) {
      fillLedsWithDriver(fNumber, j);
    } else {
      fillLedsNoDriver(fNumber, j);
    }
    t += 100;
  }
}

//**************************** Juego Sin Driver **************************
void fillLedsNoDriver(int figure, int j) {
  switch (figure) {
    case 0:
    /*   ******************************
     *    Large line area 
     */
      if (rotIndex == 0) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(fila[4 + i], LOW);
        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(125);
      } else if (rotIndex == 1) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-2], HIGH);
        digitalWrite(columna[j-3], HIGH);
        delay(125);
      } else if (rotIndex == 2) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(fila[4 + i], LOW);
        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j-3], HIGH);
        delay(125);
      } else if (rotIndex == 3) {
        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-2], HIGH);
        digitalWrite(columna[j-3], HIGH);
        delay(125);
      }
      break;
      /*   
       *    ******************************
     *         END Large line area 
     */
      /*
       *       **
       *       **
       *   **********  
       *   **********
       *   Area
       */
    case 1:
      if (rotIndex == 0) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);

        delay(20);
        reset();

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j - 1], HIGH);

        delay(80);
        reset();

        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(20);
      } else if (rotIndex == 1) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j-1], HIGH);
        delay(60);
        reset();
        
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-2], HIGH);
        delay(60);        
      
      } else if (rotIndex == 2) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j-2], HIGH);

        delay(20);
        reset();

        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j-2], HIGH);
        digitalWrite(columna[j - 1], HIGH);

        delay(80);
        reset();

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j-2], HIGH);
        delay(20);
        
      } else if (rotIndex == 3) {
        
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-2], HIGH);
        delay(60);
        reset();
        
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j-1], HIGH);
        delay(60);
        reset();
        
        
      }
      break;
      /*
       *       **
       *       **
       *   ***********  
       *   ***********
       *  END Area
       */

       /*
       *   **
       *   *******  
       *   Area shape L
       */
    case 2:
      if (rotIndex == 0) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(10);
        reset();        

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(10);
        reset();

        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j - 1], HIGH);
        delay(10);
        reset();
        delay(90);
        
      } else if (rotIndex == 1) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j - 2], HIGH);
        delay(10);
        reset();

        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-1], HIGH);
        delay(10);
        reset();

      } else if (rotIndex == 2) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(10);
        reset();        

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(20);
        reset();

        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j - 1], HIGH);
        delay(20);
        reset();
        delay(70);

        
      } else if (rotIndex == 3) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j-1], HIGH);
        digitalWrite(columna[j-2], HIGH);
        delay(60);
        reset();
        
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        delay(60);
        reset();
      }
      break ;
      /*
       *   **
       *   *******  
       *   End Area shape L
       */

       /*
       *   **
       *   ****  
       *     **
       *   Area
       */
    case 3:
      if (rotIndex == 0) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j - 1], HIGH);

        delay(10);
        reset();

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j - 1], HIGH);

        delay(100);
        reset();

        digitalWrite(fila[5 + i], LOW);
        digitalWrite(columna[j], HIGH);

        delay(10);
      } else if (rotIndex == 1) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j - 1], HIGH);
        digitalWrite(columna[j - 2], HIGH);
        delay(10);
        reset();

        
        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j - 2], HIGH);
        digitalWrite(columna[j - 3], HIGH);
        delay(10);
        reset();

        
      } else if (rotIndex == 2) {
        digitalWrite(fila[2 + i], LOW);
        digitalWrite(columna[j], HIGH);

        delay(10);
        reset();

        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j], HIGH);
        digitalWrite(columna[j - 1], HIGH);

        delay(100);
        reset();

        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j-1], HIGH);

        delay(10);
      } else if (rotIndex == 3) {
        digitalWrite(fila[3 + i], LOW);
        digitalWrite(columna[j - 2], HIGH);
        digitalWrite(columna[j - 3], HIGH);
        delay(10);
        reset();

        
        digitalWrite(fila[4 + i], LOW);
        digitalWrite(columna[j - 1], HIGH);
        digitalWrite(columna[j - 2], HIGH);
        delay(10);
        reset();
      }
      break;
       /*
       *   **
       *   ****  
       *     **
       *   End Area
       */

       /*
        *  *****
        *  *****
        *  *****
        *  
        *  Cube area
        */
    case 4:
      digitalWrite(fila[4 + i], LOW);
      digitalWrite(fila[5 + i], LOW);
      digitalWrite(columna[j], HIGH);
      digitalWrite(columna[j - 1], HIGH);
      delay(125);
      break;
       /*
        *  *****
        *  *****
        *  *****
        *  
        *  End Cube area
        */

         /*
        *   ***
        *  *****
        *   ***
        *  
        *  Dot Area
        */
    case 5:
      digitalWrite(fila[4 + i], LOW);
      digitalWrite(columna[j], HIGH);
      delay(125);
      break;

      /*
        *   ***
        *  *****
        *   ***
        *  
        *  End Dot Area
        */     
  }
  reset();

}
//**************************** Juego Con Driver **************************
void fillLedsWithDriver(int figure, int j) {
  switch (figure) {
    /*   ******************************
     *    Large line area 
     */
    case 0:
      if (rotIndex == 0) {
     
        if ((2 + i) < 0) {
          i = -2;
        } else if ((5 + i) > 7) {
          i = 2;
        }
        lc.setLed(0, 2 + i, j, true);
        lc.setLed(0, 3 + i, j, true);
        lc.setLed(0, 4 + i, j, true);
        lc.setLed(0, 5 + i, j, true);
        delay(125);
        lc.setLed(0, 2 + i, j, false);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 4 + i, j, false);
        lc.setLed(0, 5 + i, j, false);

      } else if (rotIndex == 1) {
        lc.setLed(0, 2 + i, j, true);
        lc.setLed(0, 2 + i, j-1, true);
        lc.setLed(0, 2 + i, j-2, true);
        lc.setLed(0, 2 + i, j-3, true);
        delay(125);
        lc.setLed(0, 2 + i, j, false);
        lc.setLed(0, 2 + i, j-1, false);
        lc.setLed(0, 2 + i, j-2, false);
        lc.setLed(0, 2 + i, j-3, false);
      } else if (rotIndex == 2) {
        lc.setLed(0, 2 + i, j-3, true);
        lc.setLed(0, 3 + i, j-3, true);
        lc.setLed(0, 4 + i, j-3, true);
        lc.setLed(0, 5 + i, j-3, true);
        delay(125);
        lc.setLed(0, 2 + i, j-3, false);
        lc.setLed(0, 3 + i, j-3, false);
        lc.setLed(0, 4 + i, j-3, false);
        lc.setLed(0, 5 + i, j-3, false);
      } else if (rotIndex == 3) {
        lc.setLed(0, 5 + i, j, true);
        lc.setLed(0, 5 + i, j-1, true);
        lc.setLed(0, 5 + i, j-2, true);
        lc.setLed(0, 5 + i, j-3, true);
        delay(125);
        lc.setLed(0, 5 + i, j, false);
        lc.setLed(0, 5 + i, j-1, false);
        lc.setLed(0, 5 + i, j-2, false);
        lc.setLed(0, 5 + i, j-3, false);
      }
      break;
      /*   
       *    ******************************
     *         END Large line area 
     */
      /*
       *       **
       *       **
       *   **********  
       *   **********
       *   Area
       */
    case 1:
      if (rotIndex == 0) {
        if ((3 + i) < 0) {
          i = -3;
        } else if ((5 + i) > 7) {
          i = 2;
        }
        lc.setLed(0, 3 + i, j, true);
        lc.setLed(0, 4 + i, j, true);
        lc.setLed(0, 4 + i, j - 1, true);
        lc.setLed(0, 5 + i, j, true);
        delay(125);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 4 + i, j, false);
        lc.setLed(0, 4 + i, j - 1, false);
        lc.setLed(0, 5 + i, j, false);
        
      } else if (rotIndex == 1) {
        lc.setLed(0, 3 + i, j, true);
        lc.setLed(0, 3 + i, j - 1, true);
        lc.setLed(0, 2 + i, j - 1, true);
        lc.setLed(0, 3 + i, j - 2, true);
        delay(125);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 3 + i, j -1, false);
        lc.setLed(0, 2 + i, j - 1, false);
        lc.setLed(0, 3 + i, j - 2, false);

      } else if (rotIndex == 2) {
        lc.setLed(0, 2 + i, j-2, true);
        lc.setLed(0, 3 + i, j-2, true);
        lc.setLed(0, 3 + i, j-1, true);
        lc.setLed(0, 4 + i, j-2, true);
        delay(125);
        lc.setLed(0, 2 + i, j-2, false);
        lc.setLed(0, 3 + i, j-2, false);
        lc.setLed(0, 3 + i, j-1, false);
        lc.setLed(0, 4 + i, j-2, false);
        
      } else if (rotIndex == 3) {
        lc.setLed(0, 5 + i, j, true);
        lc.setLed(0, 5 + i, j - 1, true);
        lc.setLed(0, 6 + i, j - 1, true);
        lc.setLed(0, 5 + i, j-2, true);
        delay(125);
        lc.setLed(0, 5 + i, j, false);
        lc.setLed(0, 5+ i, j -1, false);
        lc.setLed(0, 6 + i, j - 1, false);
        lc.setLed(0, 5 + i, j -2, false);
      }
      break;
      /*
       *       **
       *       **
       *   ***********  
       *   ***********
       *  END Area
       */

       /*
       *   **
       *   *******  
       *   Area shape L
       */
      
    case 2:
      if (rotIndex == 0) {
        if ((3 + i) < 0) {
          i = -3;
        } else if ((5 + i) > 7) {
          i = 2;
        }
        lc.setLed(0, 3 + i, j, true);
        lc.setLed(0, 4 + i, j, true);
        lc.setLed(0, 5 + i, j, true);
        lc.setLed(0, 5 + i, j - 1, true);
        delay(125);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 4 + i, j, false);
        lc.setLed(0, 5 + i, j, false);
        lc.setLed(0, 5 + i, j - 1, false);
      } else if (rotIndex == 1) {
        lc.setLed(0, 2 + i, j-2, true);
        lc.setLed(0, 3 + i, j-2, true);
        lc.setLed(0, 3 + i, j-1, true);
        lc.setLed(0, 3 + i, j, true);
        delay(125);
        lc.setLed(0, 2 + i, j-2, false);
        lc.setLed(0, 3 + i, j-2, false);
        lc.setLed(0, 3 + i, j-1, false);
        lc.setLed(0, 3 + i, j, false);
        
        
      } else if (rotIndex == 2) {
        lc.setLed(0, 3 + i, j-1, true);
        lc.setLed(0, 3 + i, j, true);
        lc.setLed(0, 4 + i, j-1, true);
        lc.setLed(0, 5 + i, j-1, true);
        delay(125);
        lc.setLed(0, 3 + i, j-1, false);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 4 + i, j-1, false);
        lc.setLed(0, 5 + i, j-1, false);
      } else if (rotIndex == 3) {
        lc.setLed(0, 2 + i, j-2, true);
        lc.setLed(0, 2 + i, j-1, true);
        lc.setLed(0, 2 + i, j, true);
        lc.setLed(0, 3 + i, j, true);
        delay(125);
        lc.setLed(0, 2 + i, j-2, false);
        lc.setLed(0, 2 + i, j-1, false);
        lc.setLed(0, 2  + i, j, false);
        lc.setLed(0, 3 + i, j, false);
        
      }
      break ;

      /*
       *   **
       *   *******  
       *   End Area shape L
       */

       /*
       *   **
       *   ****  
       *     **
       *   Area
       */
    case 3:
      if (rotIndex == 0) {
        if ((3 + i) < 0) {
          i = -3;
        } else if ((5 + i) > 7) {
          i = 2;
        }
        lc.setLed(0, 3 + i, j - 1, true);
        lc.setLed(0, 4 + i, j - 1, true);
        lc.setLed(0, 4 + i, j, true);
        lc.setLed(0, 5 + i, j, true);
        delay(125);
        lc.setLed(0, 3 + i, j - 1, false);
        lc.setLed(0, 4 + i, j - 1, false);
        lc.setLed(0, 4 + i, j, false);
        lc.setLed(0, 5 + i, j, false);
      } else if (rotIndex == 1) {
        lc.setLed(0, 3 + i, j - 2, true);
        lc.setLed(0, 3 + i, j - 1, true);
        lc.setLed(0, 4 + i, j - 3, true);
        lc.setLed(0, 4 + i, j - 2, true);
        delay(125);
        lc.setLed(0, 3 + i, j - 2, false);
        lc.setLed(0, 3 + i, j - 1, false);
        lc.setLed(0, 4 + i, j - 3, false);
        lc.setLed(0, 4 + i, j - 2, false);
      } else if (rotIndex == 2) {
        lc.setLed(0, 3 + i, j , true);
        lc.setLed(0, 4 + i, j , true);
        lc.setLed(0, 4 + i, j-1, true);
        lc.setLed(0, 5 + i, j-1, true);
        delay(125);
        lc.setLed(0, 3 + i, j, false);
        lc.setLed(0, 4 + i, j, false);
        lc.setLed(0, 4 + i, j -1, false);
        lc.setLed(0, 5 + i, j -1, false);
      } else if (rotIndex == 3) {
        lc.setLed(0, 3 + i, j - 3, true);
        lc.setLed(0, 3 + i, j - 2, true);
        lc.setLed(0, 4 + i, j - 2, true);
        lc.setLed(0, 4 + i, j - 1, true);
        delay(125);
        lc.setLed(0, 3 + i, j - 3, false);
        lc.setLed(0, 3 + i, j - 2, false);
        lc.setLed(0, 4 + i, j - 2, false);
        lc.setLed(0, 4 + i, j - 1, false);
      }
      break;
      /*
       *   **
       *   ****  
       *     **
       *   End Area
       */

       /*
        *  *****
        *  *****
        *  *****
        *  
        *  Cube area
        */
    case 4:
      if ((4 + i) < 0) {
        i = -4;
      } else if ((5 + i) > 7) {
        i = 2;
      }
      lc.setLed(0, 4 + i, j, true);
      lc.setLed(0, 4 + i, j - 1, true);
      lc.setLed(0, 5 + i, j, true);
      lc.setLed(0, 5 + i, j - 1, true);
      delay(125);
      lc.setLed(0, 4 + i, j, false);
      lc.setLed(0, 4 + i, j - 1, false);
      lc.setLed(0, 5 + i, j, false);
      lc.setLed(0, 5 + i, j - 1, false);
      break;

      /*
        *  *****
        *  *****
        *  *****
        *  
        *  End Cube area
        */

         /*
        *   ***
        *  *****
        *   ***
        *  
        *  Dot Area
        */
    case 5:
      if ((4 + i) < 0) {
        i = -4;
      } else if ((4 + i) > 7) {
        i = 3;
      }
      lc.setLed(0, 4 + i, j, true);
      delay(125);
      lc.setLed(0, 4 + i, j, false);
      break;
       /*
        *   ***
        *  *****
        *   ***
        *  
        *  End Dot Area
        */

  }
}

void reset() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(fila[i], HIGH);
    digitalWrite(columna[i], LOW);
  }
}

void resetTotal(){
  for (int i = 0; i < 8; i++) {
    digitalWrite(fila[i], HIGH);
    digitalWrite(columna[i], LOW);
    for(int j = 0; j < 8; j++){
      lc.setLed(0,i,j,false);
    }
  }
}

void validate() {
  delay(50);
  if (digitalRead(btnRotate) == 1){
  if (rotIndex < 4) {
      rotIndex++;
    } else{
      rotIndex = 0;
    }
  }
  if (digitalRead(btnUp) == 1) {
    i --;
  }
  else if (digitalRead(btnDown) == 1) {
    i ++;
  }

  //*************************************** MENU DE PAUSA *********************************************
  if( digitalRead(btnStart) == 1){
    tiempo = millis();       
    while(digitalRead(btnStart) == HIGH){
      if(millis() > tiempo + 3000){
        tiempo = millis();
        Serial.println("Ya pasaron 3 seg");
        pausa = true;
        cambio = true; 
        break;      
      }
    }
    while(pausa){
      if(cambio){
        resetTotal();
        cambio = false;
        Serial.println("Estoy En Pausa");
      }

      if(pp){
        mostrarPunteo();
      }else{
        mostrarSiguiente();
      }
      
      if(digitalRead(btnStart) == HIGH){
        tiempo = millis();
        pp = !pp;
        resetTotal();
        while(digitalRead(btnStart) == HIGH){
          if(millis() > tiempo + 2000){
            entorno = 0;            
            Serial.println("Ya pasaron 2 seg -> Regresar A Letrero");
            position=0;
            tiempo = millis();
            while(digitalRead(btnStart) == HIGH){
              if(millis() > tiempo + 1000){
                entorno = 2;            
                Serial.println("Ya pasaron 3 seg -> Continuar Juego");
                tiempo = millis();
                while(digitalRead(btnStart) == HIGH){
                  if(millis() > tiempo + 1000){
                    entorno = 1;
                    cambio = true;            
                    Serial.println("Ya pasaron 4 seg -> Reiniciar Juego");
                    tiempo = millis();
                    while(digitalRead(btnStart) == HIGH){
                      if(millis() > tiempo + 1000){           
                        Serial.println("Ya pasaron 5 seg -> Error");
                        tiempo = millis();
                        error = true;
                        entorno = 2;
                        delay(1000);
                        break;
                      }
                    }
                    break;
                  }
                }             
                break;
              }
            } 
            
            pausa = false;
            while(error){

              mostrarError();
              
              if(digitalRead(btnStart) == HIGH){
                resetTotal();
                error = false; 
                pausa = true;              
                break;
              }
              delay(20);
            }       
            break;   
          }        
        }
      }        
      delay(velocidad);
    }
  }
  
}

void mostrarSiguiente(){
  lc.setLed(0,2,1,true);
  lc.setLed(0,3,1,true);
  lc.setLed(0,4,1,true);
  lc.setLed(0,5,1,true);
  lc.setLed(0,6,1,true);
  lc.setLed(0,2,6,true);
  lc.setLed(0,3,6,true);
  lc.setLed(0,4,6,true);
  lc.setLed(0,5,6,true);
  lc.setLed(0,6,6,true);
  lc.setLed(0,1,6,true);
  lc.setLed(0,1,1,true);
  lc.setLed(0,2,2,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,4,4,true);
  lc.setLed(0,5,5,true);

  switch(siguiente){
    case 0:
      digitalWrite(fila[2],LOW);
      digitalWrite(fila[3],LOW);
      digitalWrite(fila[4],LOW);
      digitalWrite(fila[5],LOW);
      digitalWrite(columna[3],HIGH);
      break;
    case 1:
      digitalWrite(fila[2], LOW);
      digitalWrite(fila[3], LOW);
      digitalWrite(fila[4], LOW);
      digitalWrite(columna[4], HIGH);

      delay(20);
      reset();

      digitalWrite(fila[3], LOW);
      digitalWrite(columna[3], HIGH);
      delay(20);
      reset();

      break;
    case 2:
      digitalWrite(fila[2], LOW);
      digitalWrite(fila[3], LOW);
      digitalWrite(fila[4], LOW);
      digitalWrite(columna[4], HIGH);

      delay(20);
      reset();

      digitalWrite(fila[4], LOW);
      digitalWrite(columna[3], HIGH);
      delay(20);
      reset();
      break;
    case 3:
      digitalWrite(fila[2], LOW);
      digitalWrite(columna[3], HIGH);

      delay(20);
      reset();

      digitalWrite(fila[3], LOW);
      digitalWrite(columna[3], HIGH);
      digitalWrite(columna[4], HIGH);

      delay(20);
      reset();
      
      digitalWrite(fila[4], LOW);
      digitalWrite(columna[4], HIGH);
      
      delay(20);
      reset();
      break;
    case 4:

      digitalWrite(fila[3], LOW);
      digitalWrite(columna[3], HIGH);
      digitalWrite(fila[4], LOW);
      digitalWrite(columna[4], HIGH);
      
      
      break;
    case 5:
      digitalWrite(fila[3], LOW);
      digitalWrite(columna[3], HIGH);
      break;
  }
}

void mostrarPunteo(){
  lc.setLed(0,3,3,true);
  digitalWrite(fila[3], LOW);
  digitalWrite(columna[3], HIGH);
}

void mostrarError(){
  lc.setLed(0,2,2,true);
  lc.setLed(0,2,3,true);
  lc.setLed(0,2,4,true);
  lc.setLed(0,2,5,true);
  lc.setLed(0,3,2,true);
  lc.setLed(0,4,2,true);
  lc.setLed(0,4,3,true);
  lc.setLed(0,4,4,true);
  lc.setLed(0,4,5,true);
  lc.setLed(0,5,2,true);
  lc.setLed(0,6,2,true);
  lc.setLed(0,6,3,true);
  lc.setLed(0,6,4,true);
  lc.setLed(0,6,5,true);

  for (int j=0;j<8;j++){
    SeleccionarFila(j+1); 
    for (int i=0;i<8;i++){
      if(((j==2 || j==4 || j==6) && (i>1 && i<6)) || (j==3 && i==2)|| (j==5 && i==2)){
        ActivarLedColumna(i+1,1);
      }else{
        ActivarLedColumna(i+1,0);
      }     
    }
    delay(1);
  } 
}
