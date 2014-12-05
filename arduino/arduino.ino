#include <FlexiTimer2.h>

const int baudRate = 9600; //Vitesse

//Commandes
enum COMMANDS {
  RC_ROUGE = 'a',
  RC_VERT,
  RC_BLEU,
  SCINT_BLEU,
  SAVE_LONG_X
  }; 

const int BUFFER_LIMIT = 3; // 3 Octets dans le buffer

//Définition du paquet
enum PACKET_DETAILS{
  CMD_LOC = 0,
  ITEM_1,
  ITEM_2,
  };
  
//Structure du paquet reçu
struct
{
  byte data[BUFFER_LIMIT]; //Données reçues
  byte curLoc; //compteur des données reçues
} dataPacket; 

bool correctPacket = false; 
unsigned long lastTimerHit; //760ms pour invalider une commande
unsigned int time;


//Définition des leds
int leds[5][3] = {
    {5,6,9}, //Port des leds
    {127,0,0}, //Rapport cyclique - 0 - 255
    {00,00,0}, //Fréquence de scintillement
    {0,0,0}, //Etat
    {0,0,0} //Prochain changement d'état
  };
  
/*
  Port 5 : Rouge
  Port 6 : Vert
  Port 9 : Bleu
*/
  
void setup()
{
  Serial.begin(baudRate); //Initialisation port série
   
  dataPacket.curLoc = 0;
   
  correctPacket = false;
   
  Serial.println("Ready");
  lastTimerHit = 0;
  
  FlexiTimer2::set(20, flash); // Vérification du changement d'état toutes les 20ms
  FlexiTimer2::start();
  
  for (int i=0; i<3; i++) 
  {
    pinMode(leds[0][i], OUTPUT);
  }
  
  analogWrite(leds[0][0], leds[1][0]);
  analogWrite(leds[0][1], leds[1][1]);
  analogWrite(leds[0][2], leds[1][2]);
  
}

void loop()
{
  if ( Serial.available() > 0)
  {
    gestionDonnees();
  }
  
} 

void gestionDonnees()
{
  //Vérification du timeout
  
  if ( (lastTimerHit + 500) < millis() )
  {
    //Reset du compteur de paquets
    dataPacket.curLoc = 0; //reset counter
    correctPacket = false;
    Serial.println("timeout hit");
  }
  
  
  //Mise à jour de l'horloge de timeout
  lastTimerHit = millis();
  
  //Récupérer l'octet depuis le port série et le stocker dans le buffer
  dataPacket.data[dataPacket.curLoc] = Serial.read();
   
  //Mise à jour du compteur
  dataPacket.curLoc++;
  
  //Vérifie si 3 octets ont été lus 
  if ( dataPacket.curLoc == BUFFER_LIMIT)
  {
    //pas de check du paquet (a ajouter)
    correctPacket = true;
    //RAS du compteur
    dataPacket.curLoc = 0;
    //Gestion de la commande
    //
    switch ( dataPacket.data[CMD_LOC] )
    {
      case RC_ROUGE:
      leds[1][0] = dataPacket.data[ITEM_1] - '0';
      Serial.print("rouge : ");
      Serial.println(dataPacket.data[ITEM_1]);
      break;
       
      case RC_BLEU:
      leds[1][1] = dataPacket.data[ITEM_1];
      break;
       
      case RC_VERT:
      leds[1][2] = dataPacket.data[ITEM_1];
      break;
      
      case SCINT_BLEU:
      leds[2][0] = dataPacket.data[ITEM_1];
      break;
      
      default:
      //Mauvaise commande
      correctPacket = false;
      dataPacket.curLoc = 0;
      Serial.println("Erreur");
      break;
    }
    //Acquittement
    if (correctPacket == true )
    {
      Serial.print("ACK\n");
      correctPacket = false;
    }
 
  } 
}

//Vérifie si la LED doit subir un changement d'état
void flash()
{
  time = millis();
  for (int i=0; i<3; i++) 
  {
    if (leds[2][i] != 0)
    {
      if (leds[4][i] <= time)
      {
        if (leds[4][i] == 0)
        {
          analogWrite(leds[0][i], leds[1][i]);
          leds[4][i] = leds[4][i] + leds[1][i];
          leds[3][i] = 1;
        }
        else
        {
          analogWrite(leds[0][i], 0);
          leds[4][i] = leds[4][i] + leds[1][i];
          leds[3][i] = 0;
        }
      }
    }
  analogWrite(leds[0][0], leds[1][0]);
  analogWrite(leds[0][1], leds[1][1]);
  analogWrite(leds[0][2], leds[1][2]);
  }
}
