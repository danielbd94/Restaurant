#include "resHeader.h"

void InstructionsManu(void)
{
	printf("\n\n1-Scans dishes data from input file and creates kitchen\n");
	printf("2-Adds inventory to exsiting dish\n");
	printf("3-Makes an order for a table\n");
	printf("4-Cancel part of an order\n");
	printf("5-Closing a table:prints bill\n");
	printf("6-Prints a report: A- prints the correct inventory for dishes in the kitchen, B- prints the status of a table\n\n");
	printf("Enter '0' to scan the next command from instructions file\n");
}

/*creats linked list from Manot file*/
void CreateProduct(FILE *in_instruction, FILE *in_mana, mana **p)
{
	int flag;
	char str[100];
	mana *temp, *cur;
	cur = NULL;

	temp = (mana*)malloc(sizeof(struct mana));
	if (temp == NULL)
	{
		fclose(in_mana);
		Error_Msg(in_instruction, "no memmory");
	}
	temp->next = temp->prev = NULL;
	while (fscanf(in_mana, "%s %f %d %c", str, &(temp->Price), &(temp->Quantity), &(temp->Premium)) != EOF)
	{
		flag = 0;
		if (temp->Price > 0)
		{
			if (temp->Quantity > 0)
			{
				temp->ProductName = (char*)malloc(sizeof(char)*(strlen(str) + 1));
				if (temp->ProductName == NULL)
				{
					DeleteList(*p);
					fclose(in_mana);
					Error_Msg(in_instruction, "no memmory");
				}
				strcpy(temp->ProductName, str);
				if ((CheckIfExsist(str, *p) == 0))
				{
					if (*p == NULL)
						(*p) = cur = temp;

					else
					{
						cur->next = temp;
						temp->prev = cur;
						cur = temp;
					}
					temp = (mana*)malloc(sizeof(struct mana));
					if (temp == NULL)
					{
						DeleteList(*p);
						fclose(in_mana);
						Error_Msg(in_instruction, "no memmory");
					}
					temp->next = temp->prev = NULL;
				}
				else flag = 1;
			}
			else printf("Input error,the quantity is negative\n");
		}
		else printf("Input error,the price is negative\n");
	}
	if (flag == 1)
	{
		free(temp->ProductName);
		free(temp);
	}

}


/*check if a dish is already exsist in the manu*/
int CheckIfExsist(char* str, mana *p)
{
	mana* check = p;
	while (check)
	{
		if (strcmp(str, check->ProductName) == 0)
		{
			printf("The dish is already exsist");
			return 1;
		}
		check = check->next;
	}
	return 0;
}

/*adds stock to an exsist dish in the manu*/
void AddItems(char* name, int num, mana **p)
{
	mana *temp;
	temp = *p;
	while (temp != NULL)
	{
		if (strcmp(temp->ProductName, name) == 0)
		{
			if (num <= 0)
				printf("Error,the quantity is negative\n");
			else
			{
				temp->Quantity += num;
				return;
			}
		}
		else
			temp = temp->next;
	}
	if (temp == NULL)
		printf("The dish %s is not exist\n", name);
}


/*gets the number of a table and makes an order for the table*/
void OrderItem(FILE *in_instruction, mana *k, table *t, int num_tabels, int TableNumber, char* ProductName, int Quantity)
{
	order *temp, *cur;
	mana *serch;
	serch = k;
	cur = t[TableNumber - 1].head;
	if ((TableNumber > num_tabels) || (TableNumber <= 0))
	{
		printf("There is no sach tabel\n");
		return;
	}
	while (serch != NULL)
	{
		if (strcmp(serch->ProductName, ProductName) == 0)
		{
			if ((serch->Quantity < Quantity) || (Quantity <= 0))
			{
				printf("The quantity for this dish is to low\n");
				return;
			}
			serch->Quantity -= Quantity;
			break;
		}
		serch = serch->next;
	}
	if (serch == NULL)
	{
		printf("The dish is not exist\n");
		return;
	}
	temp = (order*)malloc(sizeof(struct order));
	if (temp == NULL)
	{
		DeleteTabels(t, num_tabels);
		DeleteList(k);
		Error_Msg(in_instruction, "no memmory");
	}
	strcpy(temp->ProductName, ProductName);
	temp->size = Quantity;
	temp->Premium = serch->Premium;
	temp->next = NULL;

	if (t[TableNumber - 1].head == NULL)
		t[TableNumber - 1].head = temp;
	else
	{
		while (cur != NULL)
		{
			if (cur->next)
				cur = cur->next;
			else
			{
				cur->next = temp;
				break;
			}
		}
	}

	t[TableNumber - 1].bill += Quantity*(serch->Price);
}

/*gets the number of a table and makes a partial cancellation for an order*/
void RemoveItem(mana *k, table *t, int TableNumber, char* ProductName, int Quantity)
{
	order *head, *cur, *DelOrder;
	mana *serch;
	serch = k;
	head = cur = DelOrder = t[TableNumber - 1].head;
	while (serch != NULL)
	{
		if (strcmp(serch->ProductName, ProductName) == 0)
		{
			if (t[TableNumber - 1].head)
			{
				while (cur != NULL)
				{
					if ((strcmp(cur->ProductName, ProductName) == 1))
						cur = cur->next;
					else break;
				}

				if (cur == NULL)
				{
					printf("The is no sach order in table number %d\n", TableNumber);
					return;
				}
				else
				{
					if ((Quantity <= 0) || (cur->size < Quantity))
					{
						printf("Quantity is out of range\n");
						return;
					}
					if (cur->size >= Quantity)
					{
						t[TableNumber - 1].bill -= (serch->Price)*Quantity;
						cur->size -= Quantity;
						if (cur->size == 0)
						{
							if ((cur == head) && (cur->next == NULL))
							{
								free(head);
								t[TableNumber - 1].head = NULL;
							}
							if (cur == head)
							{          /* we are deleting first node  */
								head = head->next;    /* moves head to next node     */
								free(cur);
							}

							else
							{                                 /* if not the first node, then */
								while (DelOrder->next != cur)
								{                            /* move prev to the node before*/
									DelOrder = DelOrder->next;      /* the one to be deleted       */
								}
								DelOrder->next = cur->next;  /* link previous node to next */
								free(cur);
							}
						}
						return;
					}
				}
			}
		}
		else serch = serch->next;
	}
}

/*gets the number of a table prints its bill and closing the table*/
void RemoveTable(table *t, int TableNumber)
{
	order *CheckPrem, *PrintBill, *head, *temp;
	CheckPrem = PrintBill = head = t[TableNumber - 1].head;
	int flag = 0;
	if (CheckPrem == NULL)
	{
		printf("There is no orders for this table\n");
		return;
	}
	while (CheckPrem != NULL)
	{
		if (CheckPrem->Premium == 'Y')
		{
			flag = 1;
			break;
		}
		else CheckPrem = CheckPrem->next;
	}
	if (flag == 1)
	{
		printf("This table is a premium table\n\n");
		t[TableNumber - 1].bill *= 1.15;
	}
	while (PrintBill != NULL)
	{
		printf("%s * %d\n", PrintBill->ProductName, PrintBill->size);
		PrintBill = PrintBill->next;
	}
	printf("Your finel price is: %.2f\n", t[TableNumber - 1].bill);
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
	t[TableNumber - 1].head = NULL;
}

/*prints report according to an input*/
void Report(mana *k, table *t, int TableNumber, char ch)
{
	mana *manu;
	order *tableOrd;

	manu = k;
	tableOrd = t[TableNumber - 1].head;
	switch (ch)
	{
	case 'A':
	case'a':
		while (manu)
		{
			printf("Name:%s Stock:%d Price:%.2f Premium:%c\n", manu->ProductName, manu->Quantity, manu->Price, manu->Premium);
			manu = manu->next;
		}
		break;
	case 'B':
	case 'b':
		if (tableOrd == NULL)
		{
			printf("This table is avalible\n");
			return;
		}
		while (tableOrd)
		{
			printf("%s * %d\n", tableOrd->ProductName, tableOrd->size);
			tableOrd = tableOrd->next;
		}
		break;
	}
}

/*the function prints string and exit from the program in case of error*/
void Error_Msg(FILE *in_instruction, char* msg)
{
	printf("\n%s\n", msg);
	fclose(in_instruction);
	exit(1);
}

/*delete the linked list of the manu in the kitchen*/
void DeleteList(mana *k)
{
	mana *temp;
	while (k != NULL)
	{
		temp = k;
		k = k->next;
		free(temp->ProductName);
		free(temp);
	}
}

/*delete the orderd for each table and then the array of tables*/
void DeleteTabels(table *array, int num_tabels) //
{
	order *temp, *head;
	int i;
	for (i = 0; i < num_tabels; i++)
	{
		head = array[i].head;
		while (head != NULL)
		{
			temp = head;
			head = head->next;
			free(temp);
		}
	}
	free(array);
}