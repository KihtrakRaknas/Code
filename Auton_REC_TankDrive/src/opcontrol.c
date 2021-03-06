
#include "main.h"
FILE *fptr;

bool motorIsDrive(int m){
	if(m == 10)// || m == 2 || m == 3 || m == 4)
		return true;
	return false;
}
int motorsInUse[] = {1,10};

Encoder encodeName(int m){
	//if(m==10)
		return encodeOne;
	/*else if(m==2)
		return encodeTwo;
	else if(m==3)
		return encodeThree;
	//Same as saying else if(m==[last encoder value])
	return encodeFour;*/
}

int potPort(int Motorport){
	return Motorport;
	/*if(m==5)
		return 5;
	else if(m==6)
		return 6;
	else if(m==7)
		return 7;
	//Same as saying else if(m==[last encoder value])
	return 8;*/
}

void SetUp();
void End();
void check();

void operatorControl() {
	SetUp();
	int oldmotoronespeed = 0;
	int oldmotortenspeed = 0;
	while(!(joystickGetDigital(1, 8, JOY_UP))){
		int motoronespeed = joystickGetAnalog(1, 3);
		int motortenspeed = joystickGetAnalog(1, 2);
		if(motortenspeed!=oldmotortenspeed||motoronespeed!=oldmotoronespeed){
			check();
			motorSet(1,motoronespeed);
			motorSet(10,motortenspeed);
			oldmotoronespeed = motoronespeed;
			oldmotortenspeed = motortenspeed;
		}
	}
	End();
}

int motorValues[11];
void SetUp(){
	fptr = fopen("auton.txt", "r");
	if (!(fptr == NULL)){
		for(int i = 0; i!=10;i++){
			printf("Printing contents of the file\n\n");
			delay(1000);
		}
		char string[5];
		while(fgets(string, 5,fptr)!=NULL){
			printf("%s", string);
			delay(20);
		}
		fclose(fptr);
		printf("\nDONE!");
	}
	fptr = fopen("auton.txt", "w");
	fprintf(fptr, "{");
	for(int i = 0; i!=sizeof(motorsInUse)/sizeof(int);i++){
		if(motorIsDrive(motorsInUse[i])){
			int val = encoderGet(encodeName(motorsInUse[i]));
			fprintf(fptr, "%d,%d,", motorsInUse[i], val);
			motorValues[motorsInUse[i]] = val;
		}else{
			int val = analogRead(motorsInUse[i]);
			fprintf(fptr, "%d,%d,", motorsInUse[i], val);
			motorValues[motorsInUse[i]] = val;
		}
	}
	fprintf(fptr, "0");
}

void End(){
	fprintf(fptr, "}");
	fclose(fptr);
}
#define potTHRESH 10
bool change = true;
void check(){
	if(change)
		fprintf(fptr, ",");
	change = false;
	for(int i = 0; i!=sizeof(motorsInUse)/sizeof(int);i++){
		if(motorIsDrive(motorsInUse[i])){
			int val = encoderGet(encodeName(motorsInUse[i]));
			printf("%d VS %d\n", val,motorValues[motorsInUse[i]]);
			if(motorValues[motorsInUse[i]] != val){
				fprintf(fptr, "%d,%d,", motorsInUse[i], val);
				motorValues[motorsInUse[i]] = val;
				change = true;
			}
		}else{
			int val = analogRead(motorsInUse[i]);
			if(val < motorValues[motorsInUse[i]]-potTHRESH || motorValues[motorsInUse[i]]+potTHRESH < val){
				fprintf(fptr, "%d,%d,", motorsInUse[i], val);
				motorValues[motorsInUse[i]] = val;
				change = true;
			}
		}
	}
	if(change)
		fprintf(fptr, "0");
}
