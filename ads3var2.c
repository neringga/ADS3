#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Eile.h"
#include "Eile_Data.h"
#include "pq.h"
#include "PQ_dataType.h"

void createLoanPaymentShedule(int, int, float *);
float loanPaymentWithPriorityQueue (float[] , int[] , float , int , float ,float ,int );
float loanPaymentWithQueue (float[] , int[] , float , int , float , float , int);
	

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
	int from, to;
	fscanf(iname, "%d", &from);
	if (from <= 0) return 0;
	fscanf(iname, "%s", &additionalInfo);
	fscanf(iname, "%d", &to);
	if (to <= 0) return 0;
	printf("%d %d", from, to);
	///////////////////////////////////////////////////////////////////////////////
	srand(time(0));
	int clientsMonthlyMoney[loanReturningTime];
	for (int i = 0; i < loanReturningTime; i++) {
		clientsMonthlyMoney[i] = (rand() % (to + 1 - from)) + from;
	}
	
	int detailed;
	printf("Do you need detailed information? 1 for yes, 0 for no");
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
		float debtToPay = 0, deletedDebt, clientsMonthMoney, loan, FINE = 0, mainSum = 0;
		
		Priority_Queue priorityQueue = Create_queue(&PQerrorCode);
		float fineToPayWithPQ =0;
		int i;
		for ( i = 0; i < loanReturningTime; i++) {
			
			if (detailed ==1) {
			printf("\tMonth number : %d\n", i+1);
			printf("Client has %d money\n", clientsMonthlyMoney[i]);
			printf("Payed debts: ");
			}
			
			clientsMonthMoney = clientsMonthlyMoney[i];
			PQerrorCode = Is_empty(priorityQueue);
			
			if (PQerrorCode != 1) {                   //there are unpaid debts       
				while ((PQerrorCode != 1) && (clientsMonthMoney > 0) && (debtToPay <= clientsMonthMoney)) {
					PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
					if (priority == 3) mainSum -= debtToPay;
					if (debtToPay <= clientsMonthMoney) {
						clientsMonthMoney -= debtToPay;
						PQerrorCode = Delete_element(&priorityQueue, &deletedDebt);
						if (detailed == 1) printf("%.2f ", debtToPay);
					}						
					PQerrorCode = Is_empty(priorityQueue);
				}
				if (mainSum != 0) {
					FINE = mainSum * fine;
					printf("New fine for unpaid debts: %f", FINE);
					PQerrorCode = Insert_element(&priorityQueue, FINE, 1);
					fineToPayWithPQ += FINE;
				}
			}
			else if (detailed == 1) printf("No debts to pay");
			
			if (detailed == 1) {
			printf("\nLoan to pay without interest rate: %.2f\n", loanPaymentShedule[i]);
			printf("Left clients month money: %.2f\n", clientsMonthMoney);
			printf("New debts: ");
			}
			if (clientsMonthMoney >= loanPaymentShedule[i]) {
				
				clientsMonthMoney -= loanPaymentShedule[i];
				
				if ( clientsMonthMoney >= monthPaymentForInterestRate) {
					clientsMonthMoney -= monthPaymentForInterestRate; 	
					if (detailed == 1) printf("No new debts. Everything was paid");
				}
				else  { 
					if (clientsMonthMoney > 0) {
					clientsMonthMoney = 0;
					PQerrorCode = Insert_element(&priorityQueue, monthPaymentForInterestRate - clientsMonthMoney , 2);
					if (detailed == 1) printf("%.2f ", monthPaymentForInterestRate - clientsMonthMoney);
					}
				}
			}
			else {  //turi maziau nei reikia sumai be palukanu
				PQerrorCode = Insert_element(&priorityQueue, loanPaymentShedule[i] - clientsMonthMoney, 3);
				mainSum += (loanPaymentShedule[i] - clientsMonthMoney);
				if (detailed == 1) printf("%.2f ", loanPaymentShedule[i] - clientsMonthMoney);
				PQerrorCode = Insert_element(&priorityQueue, monthPaymentForInterestRate, 2);
				if (detailed == 1) printf("%.2f ", monthPaymentForInterestRate);
			}
			
			if (detailed == 1) printf("\n");
			
		}
		int j = i;
		PQerrorCode = Is_empty(priorityQueue);
		i = 0;
		if (PQerrorCode == 0) {
		if (detailed == 1) printf("Left debts after the end of loan payment time:");
		while (PQerrorCode == 0) {
			printf("\nClient has %d money\n", clientsMonthlyMoney[i]);
			if (detailed == 1) printf("\n%d month: ", j+1);
			clientsMonthMoney = clientsMonthlyMoney[i];
			PQerrorCode = Get_first_element(priorityQueue, &debtToPay, &priority);
			while ((clientsMonthMoney > 0) && (PQerrorCode == 0) && (debtToPay <= clientsMonthMoney)) {
				if (clientsMonthMoney > debtToPay) {
					printf("%.2f ", debtToPay);
					clientsMonthMoney -= debtToPay;
					PQerrorCode = Delete_element(&priorityQueue, &debtToPay);
				}
				PQerrorCode = Is_empty(priorityQueue);
			}
			PQerrorCode = Is_empty(priorityQueue);
			i++;
			j++;
		}
		}
		
		
		PQerrorCode = Delete_queue(&priorityQueue);
	return fineToPayWithPQ + leftLoan + (monthPaymentForInterestRate*loanReturningTime);	
}

float loanPaymentWithQueue (float loanPaymentShedule[], int clientsMonthlyMoney[], float leftLoan, int loanReturningTime, float monthPaymentForInterestRate,
					float fine, int detailed) {
	float monthDebt, monthPaymentWithoutInterestRate, firstDebtFromQueue, oldestDebt, leftUnpaidPartOfOldestDebt, loan, FINE = 0, mainDebtSum = 0;
	float interstRateForPQ, mainDebtForFine;
	float clientsMonthMoney;
	Eile queue, mainDebts;
	
	int queueErrorCode = SukurtiEile(&queue);
	if (queueErrorCode != 0) return;
	queueErrorCode = SukurtiEile(&mainDebts);
	if (queueErrorCode != 0) return;
	
	float fineToPayWithQueue = 0;
	int i;
	for (i = 0; i < loanReturningTime; i++) {
		if (detailed ==1) {
			printf("\tMonth number : %d\n", i+1);
			printf("Client has %d money\n", clientsMonthlyMoney[i]);
		}
	
		clientsMonthMoney = clientsMonthlyMoney[i];
		queueErrorCode = ArTusciaEile(queue);
		if (detailed ==1) {
				printf("Payed debts: ");
				if (queueErrorCode != 0) printf("No debts to pay\n");
		}
		while ((queueErrorCode == 0) && (clientsMonthMoney > 0) && (oldestDebt <= clientsMonthMoney)) {   //yra skolu ir kol turi klientas pinigu jis apmoka skolas
			queueErrorCode = PirmasElementas(queue, &oldestDebt);
			if (oldestDebt <= clientsMonthMoney) {
				queueErrorCode = PirmasElementas(mainDebts, &mainDebtForFine);
				if ( oldestDebt == mainDebtForFine ) {
					mainDebtSum -= mainDebtForFine;
					queueErrorCode = PasalintiElementa(&mainDebts, &mainDebtForFine); 
				}
				clientsMonthMoney -= oldestDebt;
				if (detailed == 1) printf("%.2f ", oldestDebt);
				queueErrorCode = PasalintiElementa(&queue, &oldestDebt);        
			}
			queueErrorCode = ArTusciaEile(queue);
		}
		if (mainDebtSum != 0) {
			printf("New fine for unpaid debts: %.2f", mainDebtSum * fine);
			queueErrorCode = IterptiElementa (&queue, mainDebtSum * fine);
			fineToPayWithQueue += (mainDebtSum * fine);
		}
		if (detailed == 1) {
				printf("\nLoan to pay without interest rate: %.2f\n", loanPaymentShedule[i]);
				printf("Left clients month money: %.2f\n", clientsMonthMoney);
				printf("New debts: ");
		}
		if ( loanPaymentShedule[i] > clientsMonthMoney ) {      //suma grazinti be palukanu DIDESNE nei klientas turi pinigu
			queueErrorCode = IterptiElementa (&queue, (loanPaymentShedule[i] - clientsMonthMoney));
			queueErrorCode = IterptiElementa (&mainDebts, (loanPaymentShedule[i] - clientsMonthMoney));
			mainDebtSum += (loanPaymentShedule[i] - clientsMonthMoney);
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
	
	if (queueErrorCode == 0) {
		if (detailed == 1) printf("Payed debts: ");
		i = 0;
		while (queueErrorCode == 0) {
			if (detailed == 1) printf("\n%d month: ", i);
			clientsMonthMoney = clientsMonthlyMoney[i];
			queueErrorCode = PirmasElementas (queue, &oldestDebt);
			while ((clientsMonthMoney > 0) && (queueErrorCode == 0) && (oldestDebt <= clientsMonthMoney)) {
				if (clientsMonthMoney > oldestDebt) {
					if (detailed == 1) printf("%.2f ", oldestDebt);
					clientsMonthMoney -= oldestDebt;
					queueErrorCode = PasalintiElementa(&queue, &oldestDebt);
				}
				queueErrorCode = ArTusciaEile(queue);
			}
			queueErrorCode = ArTusciaEile(queue);
			i++;
		}
		
	}
	queueErrorCode = IstrintiEile(&queue);
	
	return fineToPayWithQueue + leftLoan + (monthPaymentForInterestRate*loanReturningTime);
	
}

void createLoanPaymentShedule (int leftLoan, int loanReturningTime, float shedule[]) {
	for (int i = 0; i<loanReturningTime; i++) {
		shedule[i] = leftLoan * 1.0 / loanReturningTime;
	}
}