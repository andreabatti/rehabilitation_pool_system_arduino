#include <LiquidCrystal.h>

#define SENSNUM 8   // numero sensori temperatura
#define TMIN 26     // temperatura minima
#define TMAX 28     // temperatura massima
#define PHSENS 7    // pin ph-metro
#define PHMIN 6.8   // ph minimo
#define PHMAX 7.4   // ph massimo
#define RELRISC 6   // pin rele riscaldatore
#define RELDOS 8    // pin rele dosatore

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    // pin display

// variabili globali
float mediaTemp = 0;  // temperatura media
float ph = 0;         // valore ph

void setup() {
  Serial.begin(9600);        // inizializzazione monitor seriale
  pinMode(PHSENS, OUTPUT);   // inizializzazione ph-metro
  pinMode(RELRISC, OUTPUT);  // inizializzazione riscaldatore
  pinMode(RELDOS, OUTPUT);   // inizializzazione dosatore
}

void loop() {
  misuroTemperatura();
  stampoTemperatura();
  delay(2000);

  if(mediaTemp < TMIN){
    do{
      temperaturaBassa();         // stampa temperatura bassa
      digitalWrite(RELRISC, LOW); // accensione riscaldatore
      delay(5000);
      misuroTemperatura();
      stampoTemperatura();
    }while(mediaTemp < TMIN);
  }
  else if(mediaTemp > TMAX){
    temperaturaAlta();            // stampa temperatura alta
    digitalWrite(RELRISC, LOW);   // spegnimento riscaldatore
  }
  else{}

  misuroPh();
  stampoPh();
  delay(2000);

  if(ph > PHMAX){
    do{
      phAlto();                   // stampa ph alto
      digitalWrite(RELDOS, HIGH); // accensione dosatore
      misuroPh();
      stampoPh();
      delay(500);
    }while(ph > PHMAX);

    digitalWrite(RELDOS, LOW);  // spegnimento dosatore
    misuroPh();
    stampoPh();
    delay(500);
  }
}

/**********
* FUNZIONI *
***********/
double misuroTemperatura(){
  int i = 0;
  int ypin = 54;
  float val_adc[SENSNUM], temp[SENSNUM];  // array temperature

  for(i=0;i<SENSNUM;i++){
    val_adc[i] = analogRead(ypin);                   // lettura tensione su ypin ypin
    temp[i] = ((val_adc[i] * 0.00488) - 0.5) / 0.01; // tensione in decimale
    ypin++;                                          // incremento ypin
  }

  for(i=0;i<SENSNUM;i++){
    mediaTemp += temp[i];           // somma di tutte le temperature
  }

  mediaTemp = mediaTemp / SENSNUM;  // media temperature
  return mediaTemp;
}

double misuroPh(){
  float phValue, voltage = 0;

  phValue = analogRead(PHSENS);         // lettura tensione sensore pH
  voltage = phValue * (5.0 / 1023.0);   // conversione del valore in volt
  ph = 3.5 * voltage + 7.0;             // valore di pH in formato decimale 

  return ph;
}

// stampa valore medio temperatura
void stampoTemperatura(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp acqua:\t");
  lcd.setCursor(0, 1);
  lcd.print(mediaTemp);
  lcd.print(" ^C");
}

// stampa errore temperatura bassa
void temperaturaBassa(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T acqua bassa:");
  lcd.setCursor(0, 1);
  lcd.print("Accendo riscald");
}

// stampa errore temperatura alta
void temperaturaAlta(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T acqua elevata:");
  lcd.setCursor(0, 1);
  lcd.print("Spengo riscald");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Spengo riscald");
  lcd.setCursor(0, 1);
  lcd.print("Aggiungere acqua");
}

void stampoPh(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valore PH:\t");
  lcd.setCursor(0, 1);
  lcd.print(ph);
}

void phAlto(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valore PH alto");
  lcd.setCursor(0, 1);
  lcd.print("Accendo dosatore");
}