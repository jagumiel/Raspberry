#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define led_rojo 2
#define led_ambar 0
#define led_verde 7
#define pulsador 4


void gpioInic(void){
    if( wiringPiSetup() == -1 ) {
            printf("Ha habido un error con la inicializacion de la libreria wiringPi. \n");
            exit(1);
    }
    pinMode(led_verde,OUTPUT);
    pinMode(led_ambar,OUTPUT);
    pinMode(led_rojo,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(pulsador,INPUT); //pulsador
    digitalWrite(led_verde, LOW);
    digitalWrite(led_ambar, LOW);
    digitalWrite(led_rojo, LOW);
    digitalWrite(3, LOW);
    
    if(softPwmCreate(led_ambar, 0, 100) != 0)
    {
        fprintf(stderr, "Error while creating PWM pin\n");
    }
}

void estado2(void){
	int tiempo=0;
	int i=0;
	double intensidad=0;

    digitalWrite(led_rojo, LOW);
    while(tiempo<8000){
        digitalWrite(led_verde,HIGH);
        delay(250);
        tiempo+=250;
        softPwmWrite(led_ambar, intensidad);
        intensidad+=3.125;//se suma 250/8000 para llegar a 100 exactos
        if(tiempo>5000){
            for (i=0; i<3; i++){//parpadea cada segundo
                digitalWrite (led_verde, HIGH) ; delay (500) ;
                tiempo+=500;
                softPwmWrite(led_ambar, intensidad);
                intensidad+=6.25;//se suma 500/8000 para llegar a 100 exactos
                digitalWrite (led_verde, LOW) ; delay (500) ;
                tiempo+=500;
                softPwmWrite(led_ambar, intensidad);
                intensidad+=6.25;
            }
        }
    }
}

void estado1(void){
	int tiempo=0;
	digitalWrite(led_rojo, HIGH);
    while(tiempo<12000){
        digitalWrite(led_rojo, HIGH);
        digitalWrite(led_verde, LOW);
        digitalWrite(led_ambar, LOW);
        softPwmWrite(led_ambar, 0);
        if(!digitalRead(pulsador)){
            printf("Paso a verde \n");
            digitalWrite(led_rojo, LOW);
            estado2();
        }
        delay(250);
        tiempo+=250;
    }
}

void semaforo(void){
    estado1();
	estado2();
}

int main (int argc, char**argv) {
    gpioInic();
    while(1){
        semaforo();
    }
	return 0;
}