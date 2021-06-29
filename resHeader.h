#ifndef HW3
#define HW3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mana
{
	char *ProductName;
	float Price;
	int Quantity;
	char Premium;
	struct mana *next, *prev;
}mana;

typedef struct kitchen
{
	mana *head;
}kitchen, *pkitchen;

typedef struct order
{
	char ProductName[100];
	int size;
	float price;
	char Premium;
	struct order *next;
}order, *Porder;

typedef struct tabel
{
	Porder head;
	float bill;
}table, *ptable;

void InstructionsManu(void);
void CreateProduct(FILE *in_instruction, FILE *in_mana, mana **p);
int CheckIfExsist(char* str, mana *p);
void AddItems(char* name, int num, mana **p);
void OrderItem(FILE *in_instruction, mana *k, table *t, int num_tabels, int TableNumber, char* ProductName, int Quantity);
void RemoveItem(mana *k, table *t, int TableNumber, char* ProductName, int Quantity);
void RemoveTable(table *t, int TableNumber);
void Report(mana *k, table *t, int TableNumber, char ch);
void Error_Msg(FILE *in_instruction, char* msg);
void DeleteList(mana *k);
void DeleteTabels(table *array, int num_tabels);

#endif