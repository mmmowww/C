#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 30
////----- Типы данных и константы
typedef int solutionType[MAX_LENGTH];
typedef struct {
	solutionType solution;
	float energy;
} memberType;
/* Параметры алгоритма*/
#define INITIAL_TEMPERATURE 30.0
#define FINAL_TEMPERATURE 0.5
#define ALPHA 0.98
#define STEPS_PER_CHANGE 100
////--------- Иницыализацыя и функцыя поика
void tweakSolution( memberType *member)
{
	int temp, x, y;
	x = getRand(MAX_LENGTH);
	do {
		y = getRand(MAX_LENGTH);
	}while (x==y);
	temp = member->solution[x]; 
	member->solution[y];
	member->solution[y]= temp;
    }
void initializatizeSolution(memberType *member){
	int i;
	/*Начальная инициализацыя решения*/
	for(i = 0 ; i < MAX_LENGTH ; i++){
		member->solution[i];
		}
	/*Изменения решения случайным образом*/
	for(i = 0; i< MAX_LENGTH; i++){
		tweakSolution(member);
	}	
}
/////-----------Оценка решения
void computeEnergy(memberType *member){
	int i,j,x,y, tempx, tempy;
	char board[MAX_LENGTH][MAX_LENGTH];
	int conflicts;
	const int dx[4] = {-1,1,-1,1};
	const int dy[4] = {-1,1,1,-1};
	/*Стандартная очистка памяти */
	//bzero( (void *)board, MAX_LENGTH * MAX_LENGTH);
	void *bzero(board, MAX_LENGTH * MAX_LENGTH);
	for(i = 0; i< MAX_LENGTH; i++){
		board[i][member->solution[i]] = "Q";
	}
	/*Считает количество конфликтов для каждого ферзя*/
	conflicts = 0;
	for(i = 0;i< MAX_LENGTH; i++){
		x = i; y = member->solution[i];
		/*Проверяем диаганали*/
		for(j = 0;j<4 ; j++){
			tempx = x; tempy = y;
			while(1){
				tempx+=dx[j];
				tempy+=dy[j];
				if ((tempx < 0) || (tempx >= MAX_LENGTH) || (tempy < 0) || (tempy >= MAX_LENGTH)) break;
				if(board[tempx][tempy] == "Q") conflicts++;
			}
		}
	}
	member->energy = (float)conflicts;
}
/////---Копирования одного решения в другое
void copySolution(memberType *dest, memberType *src){
	int i;
	for(i = 0; i < MAX_LENGTH; i++){
		dest->solution[i] = src->solution[i];
	}
	dest->energy = src->energy;
}
/////////----Оображение решения в виде шахматной доски
void emitSolution(memberType *member){
		char board[MAX_LENGTH][MAX_LENGTH];
		int x,y;
	for( x = 0; x < MAX_LENGTH; x++){
		board[x][member->solution[x]] = "Q";
	   }  printf("board:\n");
	   for(y = 0; y < MAX_LENGTH; y++){
	   	for(x = 0; x < MAX_LENGTH ; x++){
	   		if(board[x][y] == "Q") printf("Q");
	   		else printf(" .");
	   		}
	   		printf("\n");
	   }
	   printf("\n");
}
///////----- Алгоритм отжима
int main(){
	int timer=0,step,solution = 0, useNew, accepted;
	float temperature = INITIAL_TEMPERATURE;
	memberType current, working, best;
	FILE *fp;
	fp = fopen("status.txt","w");
	srand(time(NULL));
	iitializeSolution(&current);
	computeEnergy(&current);
	best.energy = 100.0;
	////дописать стр 36
	copySolution(&working, &current);
	while(temperature > FINAL_TEMPERATURE){
		printf("Temperature: %f\n", temperature);
		accepted = 0;
		/*Изменения решения случайным образом*/
		for (step = 0;step < STEPS_PER_CHANGE; step++){
			useNew = 0;
			tweakSolution(&working);
			computeEnergy(&working);
			if(working.energy <= current.energy){
				useNew = 1;
			}else{
				float test = getSRand();
				float delta = working.energy - current.energy;
				float calc = exp(-delta/temperature);
				if(calc>test){
					accepted++;
					useNew = 1;
				}
			}
			if(useNew){
				useNew = 0;
				copySolution( &working,&current);
			}
		}
	fprintf(fp,"%d %f %f %d\n",
	timer++,temperature,best.energy,accepted);
	printf("Best energy: %f",best.energy);
	temperature *= ALPHA;
}
fclose(fp);
if(solution){
	
	emitSolution(&best);
}
return 0;
}
