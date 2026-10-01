/*

vers 5.4
Aparent OK


*/


// Pini liberi accesibili blocuri terminale
// A_Pin = 54;
// B_Pin = 55;
// C_Pin = 24;
// D_Pin = 23;
// E_Pin = 22;
// F_Pin = 20;

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);
//                        0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
const int staPins[18] = {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43};  // pinii pt statie
const int posPins[18] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 69, 68, 67, 66, 65, 64, 63, 62};   // pinii pt pozitie

const int swPin = 10; // switch afisaj
const int testPin = 11; // pin pentru trecerea in regim test(comutatorul rosu frontal)
const int relPin = 56; // pin status placa relee comanda
const int svcPin = 57; // pin service (de la comutatorul de revizie)
const int perPin = 58; // pin de la pod mobil pt persoana in interior
const int occPin = 59; // pin ocupat
const int upPin =  60;  // pin sus
const int dwnPin = 61; // pin jos


int sen[2];   //  array pt statia destinatie
int sta[18];  //  array pt statia destinatie
int pos[18];  //  array pt pozitia curenta

  //int Des = 0 ;
  //int Pos = 0 ;
  //int Sj = 0;      // sus/jos
  //int Ocp = 0;     // ocupat
  //int Per = 0 ;     // cu persoane


void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(swPin, INPUT_PULLUP); 
  pinMode(perPin, INPUT_PULLUP);
  pinMode(occPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(dwnPin, INPUT_PULLUP);
  pinMode(testPin, INPUT_PULLUP);
  pinMode(relPin, INPUT_PULLUP);
  pinMode(svcPin, INPUT_PULLUP);

  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  //pinMode(1, INPUT_PULLUP);
  //pinMode(0, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);  //16 si 17 uart 2
  pinMode(17, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);

pinMode(25, INPUT_PULLUP);




  
    for (int i = 0; i < 18; i++) {
      pinMode(staPins[i], INPUT_PULLUP);
      pinMode(posPins[i], INPUT_PULLUP);
        }
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_streamline_computers_devices_electronics_t);
  u8g2.setCursor(10, 32);
  u8g2.print("\u0035");
  u8g2.setFont(u8g2_font_streamline_interface_essential_setting_t);
  u8g2.setCursor(100, 32);
  u8g2.print("\u0035");
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.setCursor(50, 30);
  u8g2.print("v5.4");
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.setCursor(10, 60);
  u8g2.print("0740.317.707");
  u8g2.sendBuffer();
  delay(5000);
  u8g2.clearBuffer();


}

void loop() {
  static int Des=0 ;
  static int Pos=0 ;
  static int lastPos=0 ;
  int Sj=0;      // sus/jos
  static int Ocp=0;     // ocupat
  //int Per=0;     // cu persoane
  int sumpos = 0; // variabila pt stabilire status defect
  int sumsta = 0; // variabila pt stabilire status defect
  static int Sumsta = 0;
  int Def = 0; // variabila "defect"
  static int Rev = 0; // variabila "revizie"
  int Svc = 0; //variabila "service pt trimitere pe serial"
  int Rel = 0; // variabila pt stare relee comanda 
  int Display = 0;
  int Test =0;
  //char buffer[12];

  for (int i = 0; i < 18; i++) {
    sta[i] = digitalRead(staPins[i]);
    pos[i] = !digitalRead(posPins[i]);
    
    sumpos += pos[i];
    sumsta += sta[i];
    Sumsta = sumsta;
    
    if(sta[i] == 0) {Des = i;}
    if((pos[i] == 1 && sumpos !=18)) {Pos = i;}
  
  }    
//Serial.print( "sumpos  ");
//Serial.println(sumpos);
//Serial.print( "sumsta  ");
//Serial.println(sumsta);
  
  if(digitalRead(upPin) == 1 && digitalRead(dwnPin) == 0) {Sj = 2;}          //in jos (dwn)
  else if(digitalRead(upPin) == 0 && digitalRead(dwnPin) == 1) {Sj = 1;}     //in sus (up)
  else {Sj = 0;}                                                             // in repaus
    
  if(digitalRead(occPin) == 0) {Ocp = 1;}
  else {Ocp = 0;}                                                            //ocupat
    
  if(digitalRead(svcPin) == 0) {Rev = 1;}
  else {Rev = 0;}                                                            // persoana in ascensor

  if(digitalRead(testPin) == 0 /*| digitalRead(svcPin) == 0 */) {Test = 1; /*Svc = 2;*/}
  else {Test = 0;}

  if(digitalRead(relPin) ==0) {Rel = 1;}
  else {Rel = 0;}


// AICI MAI ESTE DE LUCRAT

  if(sumpos==18) { Pos = lastPos; }
  if(36-sumpos-sumsta == 0) {Svc = 1;} // pentru toate intrarile pos sau statie in 0 logic (releele neactionate, brt sarit), afiseaza "Defect"
  else if(Rev==1) {Svc=2;}           //svc=2 pt revizie   afiseaza "revizie, folositi ascensorul 1/2"
  else if(Rel==1) {Svc=3;}           // svc=3 pt stare relee afiseaza "defect, folositi ascensorul 1/2"
  else {Svc=5;}                     //svc=5 pt functionare normala  svc=4 rezervat pt lipsa semnal serial , setat din receptor
lastPos = Pos;

char buffer[12];

sprintf(buffer, "<%02d,%02d,%d,%d,%d>" , Pos, Des, Ocp, Sj, Svc );

Serial.println(buffer);

Serial1.println(buffer);
delay(100);
//Serial1.println(buffer);

//delay(10) ; //// ATENTIE LA DELAY   DE TESTAT, PARE OPTIM LA 250MS CU 9600 BAUD

//                                              == display==

Display = digitalRead(swPin);
//u8g2.clearBuffer();					                  // clear the internal memory
//Serial.print ("test: ");
//Serial.println (Test);

//if (Rev == 0) {                                // daca nu este in revizie
  //if (Display == HIGH) {                                  // daca ecranul este pornit:
    u8g2.clearBuffer(); 
    u8g2.setFont(u8g2_font_logisoso16_tr);
    
    switch (Svc) {
      case 1: {
       if (Test == 0){
         u8g2.setCursor(27, 22);
         u8g2.print("SERVICE");
         u8g2.setCursor(17, 40);
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.println("no signal from");
         u8g2.setCursor(10, 58);
         u8g2.println("Pos & Sta relays");
        }
        else {
         //u8g2.clearBuffer(); 
         u8g2.setFont(u8g2_font_logisoso16_tr);
         u8g2.setCursor(35, 22);
         u8g2.print("TEST svc1"); 
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(12, 40); 
         u8g2.print("pos.st.oc.u/d.svc");
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(18, 60); 
         u8g2.print(buffer);
        }
      }
      break;

      case 2: {
       if (Test == 0) {
         u8g2.clearBuffer(); 
         u8g2.setFont(u8g2_font_logisoso16_tr);
         u8g2.setCursor(35, 22);
         u8g2.print("REVIZIE"); 
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(2, 40); 
         u8g2.print("pos.st.oc.u/d.svc");
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(18, 60); 
         u8g2.print(buffer);
        } 
        else {
         u8g2.clearBuffer(); 
         u8g2.setFont(u8g2_font_logisoso16_tr);
         u8g2.setCursor(35, 22);
         u8g2.print("TEST  rev"); 
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(2, 40); 
         u8g2.print("pos.st.oc.u/d.svc");
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(18, 60); 
         u8g2.print(buffer);
        }
      }
      break;

      case 3: {
       if (Test == 0) {
         u8g2.setCursor(27, 22);
         u8g2.print("SERVICE");
         u8g2.setCursor(17, 40);
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.println("no signal from");
         u8g2.setCursor(10, 58);
         u8g2.println("  Com relays");
        } 
        else {
         u8g2.clearBuffer(); 
         u8g2.setFont(u8g2_font_logisoso16_tr);
         u8g2.setCursor(35, 22);
         u8g2.print("TEST  svc3"); 
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(2, 40); 
         u8g2.print("pos.st.oc.u/d.svc");
         u8g2.setFont(u8g2_font_helvR10_tf);
         u8g2.setCursor(18, 60); 
         u8g2.print(buffer); 
        }
      }
      break;

      case 5: {
       if (Test == 0) {
         if (Display == LOW) {
           u8g2.setCursor(6, 20);
           u8g2.print("Pos:");
           u8g2.print(Pos);
           u8g2.setCursor(77, 20);
           u8g2.print("Dst:");
           //Serial.println(sumsta);
           if (Sumsta==17) {u8g2.print(Des);}
           else {u8g2.print("--");}
           u8g2.setFont(u8g2_font_helvR08_tf);
           u8g2.setCursor(32, 35);
           u8g2.print("ocp.        up / dwn");
           u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
           u8g2.setCursor(32, 58);
           if (Ocp == 1) {u8g2.print("\u0081");}
           else {u8g2.print("\u0079");}   
           u8g2.setCursor(88, 58);
           if(Sj == 1) {u8g2.print("\u008f");}
           else if (Sj == 2) {u8g2.print("\u008e");}
           else {u8g2.print("\u0079");}
           u8g2.setCursor(102, 58);
          }
          else {
           /*u8g2.clearBuffer(); 
           u8g2.setFont(u8g2_font_logisoso16_tr);
           u8g2.setCursor(35, 22);
           u8g2.print("TEST  nor"); 
           u8g2.setFont(u8g2_font_helvR10_tf);
           u8g2.setCursor(2, 40); 
           u8g2.print("pos.st.oc.u/d.svc");
           u8g2.setFont(u8g2_font_helvR10_tf);
           u8g2.setCursor(18, 60); 
           u8g2.print(buffer);*/
          }
        }
        else{
          u8g2.clearBuffer(); 
          u8g2.setFont(u8g2_font_logisoso16_tr);
          u8g2.setCursor(25, 22);
          u8g2.print("TEST  nor"); 
          u8g2.setFont(u8g2_font_helvR10_tf);
          u8g2.setCursor(8, 40); 
          u8g2.print("pos. st. oc. u/d. svc");
          u8g2.setFont(u8g2_font_helvR10_tf);
          u8g2.setCursor(28, 60); 
          u8g2.print(buffer); 
        }
      }
       break;
    }
  //}
  
  /*else {
   u8g2.clearBuffer(); 
   u8g2.setFont(u8g2_font_logisoso16_tr);
   u8g2.setCursor(35, 22);
   u8g2.print("TEST"); 
   u8g2.setFont(u8g2_font_helvR10_tf);
   u8g2.setCursor(2, 40); 
   u8g2.print("pos.st.oc.u/d.svc");
   u8g2.setFont(u8g2_font_helvR10_tf);
   u8g2.setCursor(18, 60); 
   u8g2.print(buffer);

  }*/

  //}  
    /*if(Svc == 1) {                                        // daca status defect:
      u8g2.setCursor(27, 22);
      u8g2.print("SERVICE");
      u8g2.setCursor(17, 40);
      u8g2.setFont(u8g2_font_helvR10_tf);
      u8g2.println("signal '0' from");
      u8g2.setCursor(10, 58);
      u8g2.println("Pos & Sta relays");
    } 
    */
    /*else if (Svc ==2 )  {
       u8g2.clearBuffer(); 
       u8g2.setFont(u8g2_font_logisoso16_tr);
       u8g2.setCursor(35, 22);
       u8g2.print("REVIZIE"); 
       u8g2.setFont(u8g2_font_helvR10_tf);
       u8g2.setCursor(2, 40); 
       u8g2.print("pos.st.oc.u/d.svc");
       u8g2.setFont(u8g2_font_helvR10_tf);
       u8g2.setCursor(18, 60); 
       u8g2.print(buffer); 
      } 
                                 
    else if(Svc == 3) {                                        // daca status defect:
      u8g2.setCursor(27, 22);
      u8g2.print("SERVICE");
      u8g2.setCursor(17, 40);
      u8g2.setFont(u8g2_font_helvR10_tf);
      u8g2.println("signal '0' from");
      u8g2.setCursor(10, 58);
      u8g2.println("  Com relays");
    } */                                   
     
    /*else  {                                                  // daca status ok:
      u8g2.setCursor(5, 20);
      u8g2.print("Pos:");
      u8g2.print(Pos);
      u8g2.setCursor(76, 20);
      u8g2.print("Dst:");
      if (sumsta==17) {u8g2.print(Des);}
      else {u8g2.print("--");}
      
      u8g2.setFont(u8g2_font_helvR08_tf);
      u8g2.setCursor(12, 35);
      u8g2.print("ocp.     up / dwn     ");
      
      u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.setCursor(12, 58);
      if (Ocp == 1) {u8g2.print("\u0081");}
      else {u8g2.print("\u0079");}   
      u8g2.setCursor(57, 58);
      if(Sj == 1) {u8g2.print("\u008e");}
      else if (Sj == 2) {u8g2.print("\u008f");}
      else {u8g2.print("\u0079");}
      u8g2.setCursor(102, 58);
    }                                                    
  }                                                    
}
else {
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.setCursor(35, 22);
  u8g2.print("REVIZIE"); 
  u8g2.setFont(u8g2_font_helvR10_tf);
  u8g2.setCursor(2, 40); 
  u8g2.print("pos.st.oc.u/d.svc");
  u8g2.setFont(u8g2_font_helvR10_tf);
  u8g2.setCursor(18, 60); 
  u8g2.print(buffer); 
}

if (Test==1) {                                                  // daca status revizie:
    
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.setCursor(35, 22);
  u8g2.print("TEST"); 
  u8g2.setFont(u8g2_font_helvR10_tf);
  u8g2.setCursor(2, 40); 
  u8g2.print("pos.st.oc.u/d.svc");
  u8g2.setFont(u8g2_font_helvR10_tf);
  u8g2.setCursor(18, 60); 
  u8g2.print(buffer);
}
 */ 
 u8g2.sendBuffer(); 
  //delay(50) ;
}                                                          // end loop
