#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void gpioInic(void){
	if( wiringPiSetup() == -1 ) {
		printf("Ha habido un error con la inicializacion de la libreria wiringPi. \n");
                exit(1);
        }
    pinMode(7,OUTPUT);
    pinMode(0,OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
	pinMode(4,INPUT); //pulsador
    digitalWrite(7, LOW);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
	
}


void conversorBinario(int valor, char* output)
{
    int i;
    output[4] = '\0';
    for (i = 3; i >= 0; --i, valor >>= 1)
    {
        output[i] = (valor & 1) + '0';
    }
}

void representarLeds(char* s){
	int numLed=0;
	int i=0;
	char aux[6]="1111\n";
	char pines[6]="7023";
	for (i=0; i<4; i++){
			numLed=pines[i]- '0'; //Menos cero porque estoy convirtiendo un caracter en numero.
			if (s[i]==aux[i]){
				digitalWrite(numLed, HIGH);
			}else{
				digitalWrite(numLed, LOW);
			}
		}
}


void contadorSimple(){
	int miNumero=0;
	char s[6];
    for(miNumero=0; miNumero<16; miNumero++){
		conversorBinario(miNumero, s);
		representarLeds(s);
		sleep(1); //Queremos que el contador vaya despacio.
    }
	for(miNumero=15; miNumero>-1; miNumero--){
		conversorBinario(miNumero, s);
		conversorBinario(miNumero, s);
		representarLeds(s);
		sleep(1); //Queremos que el contador vaya despacio.
    }
}


void contadorPulsador(){
	int miNumero=0;
	int i=0;
	char a[6];
	while (1){
		if (!digitalRead(4))
			break;
	}
	int aux=0;
	while(1){
		aux=i;
		for (i=aux; i<16; i++){
			conversorBinario(i, a);
			representarLeds(a);
			sleep(1);
			if(i==15){
				i=-1;
				miNumero=i;
			}
			if(!digitalRead(4)){
				printf("Resto.\n");
				break;
			}
		}
		miNumero=i;
		for (i=miNumero; i>-1; i--){
			conversorBinario(i, a);
			representarLeds(a);
			sleep(1);
			if(i==0){
				i=16;
				aux=i;
			}
			if(!digitalRead(4)){
				printf("Sumo.\n");
				break;
			}
		}
	}
}


int main(int argc, char**argv) {
   
    gpioInic();
	int opcion=0;
	do{
		printf("\n   1. El contador cuenta de 0 a 15, luego de 15 a 0 y termina.");
        printf("\n   2. El contador inicia la cuenta cuando se pulsa el boton. Cada vez que se vuelve a pulsar el boton el contador cambia de direccion. ");
        printf("\n   3. Salir.");
        printf("\n\n   Introduzca opcion (1-3): ");

        scanf( "%d", &opcion );

		switch (opcion){
            case 1: contadorSimple();
                    break;

            case 2: contadorPulsador();
                    break;
		}
    }while ( opcion != 3 );
		
    return 0;
}