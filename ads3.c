#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Eile.h"
#include "Eile_Data.h"
#include "pq.h"
#include "PQ_dataType.h"

struct el {
	float data;
	struct el *next;
};
typedef struct el LIST;

void createLoanPaymentShedule(int, int, float *);
float loanPaymentWithPriorityQueue (float[] , int[] , float , int , float ,float ,int );
float loanPaymentWithQueue (float[] , int[] , float , int , float , float , int);
void insert_element(LIST **, float);
void delete_element(LIST **, float);
void delete_list (LIST **);
float listSum (LIST *);

int main () {
	float loan, monthMoney, monthlyReturnAmount, annualInterestRate, fine; 
	int loanReturningTime;
	///////////////////////////////////////////////////////////////////////////////
	FILE *iname;
	char additionalInfo[20];
	iname = fopen("input.txt", "r"); 
	if (iname == NULL) {
		printf("File does not exist");
		return 0;
	}
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%f", &loan);
	if (loan <= 0) return 0;
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%d", &loanReturningTime);
	if (loanReturningTime <= 0) return 0;
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%f", &annualInterestRate);
	if (annualInterestRate <= 0) return 0;
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%f", &fine);
	if (fine <= 0) return 0;
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%s", &additionalInfo);
	int min, max;
	fscanf(iname, "%d", &min);
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%d", &max);
	///////////////////////////////////////////////////////////////////////////////
	srand(time(0));
	int clientsMonthlyMoney[loanReturningTime];
	int i;
	for (i = 0; i < loanReturningTime ; i++) {
		clientsMonthlyMoney[i] = (rand() % (max + 1 - min)) + min;
	}
	
	int detailed;
	printf("Do you need detailed information? 1 for yes, 0 for no ");
	scanf("%d", &detailed);
	
	float loanPaymentShedule[loanReturningTime];
	createLoanPaymentShedule(loan, loanReturningTime, loanPaymentShedule);
	fine = (fine / 12)* 1.0 / 100 ;
	float monthPaymentForInterestRate = loan * (annualInterestRate * 1.0 / 12) / 100;
	
	if (detailed == 1) printf("\t-----------Queue:-------------\n");
	float queue = loanPaymentWithQueue(loanPaymentShedule, clientsMonthlyMoney, loan, loanReturningTime, monthPaymentForInterestRate, fine, detailed);
    if (detailed == 1) printf("\n");
	if (detailed == 1)  printf("\t--------Priority queue:----\n");
	float priorityQueue = loanPaymentWithPriorityQueue(loanPaymentShedule, clientsMonthlyMoney, loan, loanReturningTime, monthPaymentForInterestRate, fine, detailed);
	printf("\n\nLoan to pay with queue : %.2f", queue);
	printf("\nLoan to pay with priority queue : %.2f", priorityQueue);
	
	return 0;
}

float loanPaymentWithPriorityQueue (float loanPaymentShedule[], int clientsMonthlyMoney[], float leftLoan, int loanReturningTime, 
									float monthPaymentForInterestRate, float fine, int detailed) {
		
		int PQerrorCode, priority ;
		float debtToPay = 0, deletedDebt, clientsMonthMoney, loan, fineList = 0, mainSum = 0, fineNextMonth = 0, oldDebt = 0, lastUnpaidDebt;
		
		Priority_Queue priorityQueue = Create_queue(&PQerrorCode);
		float fineToPayWithPQ =0;
		int i;
		LIST *fines = NULL;
		for ( i = 0; i < loanReturningTime; i++) {
			
			clientsMonthMoney = clientsMonthlyMoney[i];
			if (detailed ==1) {
			printf("\tMonth number : %d\n", i+1);
			printf("Client has %.0f money\n", clientsMonthMoney);
			}
			
			mainSum = listSum(fines);
			if (mainSum != 0) {
					fineList = mainSum * fine;
					if (detailed == 1) printf("\nNew fine for unpaid debts: %.2f", fineList);
					PQerrorCode = Insert_element(&priorityQueue, fineList, 1);
					fineToPayWithPQ += fineList;	
			}
			
			PQerrorCode = Is_empty(priorityQueue);
			if (PQerrorCode != 1) {                   //there are unpaid debts     
				if (detailed == 1) printf("\nPayed debts: ");
				PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
				while ((PQerrorCode != 1) && (clientsMonthMoney > 0) && (debtToPay <= clientsMonthMoney) ) {
					
						clientsMonthMoney -= debtToPay;
						delete_element(&fines, debtToPay);
						PQerrorCode = Delete_element(&priorityQueue, &deletedDebt);
						if (detailed == 1) printf("%.2f ", debtToPay);
					
					PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
					PQerrorCode = Is_empty(priorityQueue);
				}
				
			}
			else if (detailed == 1) printf("No debts to pay");
			
			if (detailed == 1) {
			printf("\nLoan to pay without interest rate: %.2f\n", loanPaymentShedule[i]);
			printf("Left clients month money: %.2f\n", clientsMonthMoney);
			printf("New debts: ");
			}
			if (clientsMonthMoney >= loanPaymentShedule[i]) {  //gali sumoket skola be palukanu
				clientsMonthMoney -= loanPaymentShedule[i];
				
				if ( clientsMonthMoney >= monthPaymentForInterestRate) {
					clientsMonthMoney -= monthPaymentForInterestRate; 	
					if (detailed == 1) printf("No new debts. Everything was paid");
				}
				else  { 		//negali sumoket visu palukanu
					if (clientsMonthMoney >= 0) {
					clientsMonthMoney = 0;
					PQerrorCode = Insert_element(&priorityQueue, monthPaymentForInterestRate - clientsMonthMoney , 2);
					if (detailed == 1) printf("%.2f ", monthPaymentForInterestRate - clientsMonthMoney);
					}
				}
			}
			else {  //turi maziau nei reikia sumai be palukanu
				PQerrorCode = Insert_element(&priorityQueue, loanPaymentShedule[i] - clientsMonthMoney, 3);
				insert_element(&fines, (loanPaymentShedule[i] - clientsMonthMoney));
				fineToPayWithPQ += (fineNextMonth * fine);
				if (detailed == 1) printf("%.2f ", loanPaymentShedule[i] - clientsMonthMoney);
				PQerrorCode = Insert_element(&priorityQueue, monthPaymentForInterestRate, 2);
				if (detailed == 1) printf("%.2f ", monthPaymentForInterestRate);
			}
			
			if (detailed == 1) printf("\n");
		}
		
		int j = i;
		i = 0;
		float fineSum;
		PQerrorCode = Is_empty(priorityQueue);
		if (PQerrorCode == 0) {
			if (detailed == 1) printf("\nLeft debts after the end of loan payment time:");
			while (PQerrorCode == 0) {
				if (detailed == 1) printf("\n%d month", j+1);
				if (detailed == 1) printf("\nClient has %d money\n", clientsMonthlyMoney[i]);
				clientsMonthMoney = clientsMonthlyMoney[i];
				
				fineSum = listSum(fines);
				if(fineSum != 0) {
					if (detailed ==1) printf("Fine for old unpaid debts: %.2f ", fineSum*fine);
					PQerrorCode = Insert_element(&priorityQueue, (fineSum * fine) , 1);
				}
				if (detailed == 1) printf("Payed debts: ");
				PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
				while ((clientsMonthMoney > 0) && (PQerrorCode == 0) && (debtToPay <= clientsMonthMoney)) {
					delete_element(&fines, debtToPay);
					if (detailed == 1) printf("%.2f ", debtToPay);
					clientsMonthMoney -= debtToPay;
					PQerrorCode = Delete_element(&priorityQueue, &debtToPay);
					
					PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
					PQerrorCode = Is_empty(priorityQueue);
				}
				
				i++;
				j++;
				if (i == loanReturningTime) i = 0;
			}
		}
		
	PQerrorCode = Delete_queue(&priorityQueue);
	delete_list(&fines);
	if (detailed == 0) printf("\nLoan with PQ was paid in %d months", j);
	return fineToPayWithPQ + leftLoan + (monthPaymentForInterestRate*loanReturningTime);	
}

float loanPaymentWithQueue (float loanPaymentShedule[], int clientsMonthlyMoney[], float leftLoan, int loanReturningTime, float monthPaymentForInterestRate,
					float fine, int detailed) {
	float monthDebt, monthPaymentWithoutInterestRate, firstDebtFromQueue, oldestDebt, leftUnpaidPartOfOldestDebt, loan, FINE = 0, mainDebtSum = 0, fineNextMonth = 0;;
	float interstRateForPQ, mainDebtForFine, oldestFine, sumOfFines;
	float clientsMonthMoney;
	Eile queue, mainDebts;
	LIST *fines = NULL;
	
	int queueErrorCode = SukurtiEile(&queue);
	if (queueErrorCode != 0) return;
	queueErrorCode = SukurtiEile(&mainDebts);
	if (queueErrorCode != 0) return;
	
	float fineToPayWithQueue = 0;
	int i;
	for (i = 0; i < loanReturningTime; i++) {
		clientsMonthMoney = clientsMonthlyMoney[i];
		if (detailed ==1) {
			printf("\tMonth number : %d\n", i+1);
			printf("Client has %.0f money\n", clientsMonthMoney);
		}
		sumOfFines = listSum(fines);
		if (sumOfFines != 0) {
			if (detailed == 1) printf("\nNew fine for old unpaid debts: %.2f\n", sumOfFines * fine);
			queueErrorCode = IterptiElementa (&queue, sumOfFines * fine);
			fineToPayWithQueue += (sumOfFines * fine);
		}
		queueErrorCode = ArTusciaEile(queue);
		
		if (detailed ==1) {
				printf("Payed debts: ");
				if (queueErrorCode != 0) printf("No debts to pay\n");
		}
		queueErrorCode = PirmasElementas(queue, &oldestDebt);
		queueErrorCode = ArTusciaEile(queue);
		while ((queueErrorCode == 0) && (clientsMonthMoney > 0) && (oldestDebt <= clientsMonthMoney)) {   //yra skolu ir kol turi klientas pinigu jis apmoka skolas
			delete_element(&fines, oldestDebt);
			clientsMonthMoney -= oldestDebt;
			if (detailed == 1) printf("%.2f ", oldestDebt);
			queueErrorCode = PasalintiElementa(&queue, &oldestDebt);        
			queueErrorCode = PirmasElementas(queue, &oldestDebt);
			queueErrorCode = ArTusciaEile(queue);
		}
		if (oldestDebt != 0) {
			sumOfFines += oldestDebt;
		}
		
		if (detailed == 1) {
				printf("\nLoan to pay without interest rate: %.2f\n", loanPaymentShedule[i]);
				printf("Left clients month money: %.2f\n", clientsMonthMoney);
				printf("New debts: ");
		}
		if ( loanPaymentShedule[i] > clientsMonthMoney ) {      //suma grazinti be palukanu DIDESNE nei klientas turi pinigu
			queueErrorCode = IterptiElementa (&queue, (loanPaymentShedule[i] - clientsMonthMoney));
			insert_element(&fines, (loanPaymentShedule[i] - clientsMonthMoney));
			fineToPayWithQueue += ((loanPaymentShedule[i] - clientsMonthMoney) * fine);
			if (detailed == 1) printf("%.2f ", loanPaymentShedule[i] - clientsMonthMoney);
			queueErrorCode = IterptiElementa (&queue, monthPaymentForInterestRate);
			if (detailed == 1) printf("%.2f ", monthPaymentForInterestRate);
		}
		else {
			if ( monthPaymentForInterestRate > (clientsMonthMoney - loanPaymentShedule[i]) ) {     //ar gali sumoket palukanas - ne
				monthDebt = monthPaymentForInterestRate - (clientsMonthMoney - loanPaymentShedule[i]);
				if (detailed == 1) printf("%.2f ", monthDebt);
				queueErrorCode = IterptiElementa (&queue, monthDebt);
			}
			else if (detailed == 1) printf("Loan was paid without new debts");
		}
		if (detailed == 1) printf("\n\n");
		
	}
	
	queueErrorCode = ArTusciaEile(queue);
	int j = i;
	if (queueErrorCode == 0) {
		i = 0;
		while (queueErrorCode == 0) {
			if (detailed == 1) printf("\n%d month\n", j+1);
			if (detailed == 1) printf("Client has %d money\n", clientsMonthlyMoney[i]);
			clientsMonthMoney = clientsMonthlyMoney[i] * 1.0;
			sumOfFines = listSum(fines);
			if (sumOfFines != 0) {
				if (detailed == 1) printf("Fine for unpaid debts: %.2f\n", (sumOfFines * fine));
				queueErrorCode = IterptiElementa (&queue,  (sumOfFines * fine));
			}
			queueErrorCode = PirmasElementas (queue, &oldestDebt);
			if (detailed == 1) printf("Payed debts: ");
			while ((clientsMonthMoney > 0) && (queueErrorCode == 0) && (oldestDebt <= clientsMonthMoney)) {
					delete_element(&fines, oldestDebt);
					if (detailed == 1) printf("%.2f ", oldestDebt);
					clientsMonthMoney -= oldestDebt;
					queueErrorCode = PasalintiElementa(&queue, &oldestDebt);
				queueErrorCode = PirmasElementas (queue, &oldestDebt);
				queueErrorCode = ArTusciaEile(queue);
			}
			queueErrorCode = ArTusciaEile(queue);
			i++;
			j++;
			if (i == loanReturningTime) i = 0;
		}
	}
	queueErrorCode = IstrintiEile(&queue);
	delete_list(&fines);
	
	if (detailed == 0) printf("\nLoan with queue was paid in %d months", j);
	return fineToPayWithQueue + leftLoan + (monthPaymentForInterestRate*loanReturningTime);
	
}

void createLoanPaymentShedule (int leftLoan, int loanReturningTime, float shedule[]) {
	for (int i = 0; i<loanReturningTime; i++) {
		shedule[i] = leftLoan * 1.0 / loanReturningTime;
	}
}

void insert_element(LIST **head, float n) {
	LIST *temp;
	LIST *p;
	temp=(LIST *)malloc(sizeof(LIST));
	temp->data = n;
	temp->next = NULL;
		if (*head == NULL) {
			*head = temp;
		}
		else {
			p = *head;
			while (p->next != NULL) {
				p = p->next;
			}
			p->next = temp;
		}
}

void delete_element(LIST **head, float element) {
	LIST * temp = *head, *prev;

    if (temp != NULL && temp->data == element)
    {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != element)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;

    free(temp);
}
void delete_list (LIST **head)
{
	LIST *temp;
	while ((*head) != NULL) {
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}
float listSum (LIST *head) {
	float sum = 0;
	while (head != NULL){
		sum += head->data;
		head = head -> next;
	}
	return sum;
}