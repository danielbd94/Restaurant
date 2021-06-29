#include "resHeader.h"

void main()
{
	kitchen manu;
	manu.head = NULL;
	FILE *in_mana, *in_instructions;
	int num_tabels, i, amount, FuncNum;
	char ch, DishName[100], temp1[100], temp2[100];
	table *restaurant;
	printf("Enter the number of tabels for the restaurant: \n");
	scanf("%d", &num_tabels);
	rewind(stdin);
	restaurant = (table*)malloc(num_tabels * sizeof(table));
	if (restaurant == NULL)
	{
		DeleteList(&manu.head);
		printf("no memmory");
		exit(1);
	}
	for (i = 0; i < num_tabels; i++)
	{
		restaurant[i].head = NULL;
		restaurant[i].bill = 0;
	}
	InstructionsManu();
	scanf("%c", &ch);
	rewind(stdin);
	while (ch != '0')
	{
		printf("Error,'0' must be enterd to get instructions from file\n");
		scanf("%c", &ch);
		rewind(stdin);
	}
	if (!(in_instructions = fopen("Instructions.txt", "r")))
	{
		printf("The input file is wrong");
		exit(1);
	}


	while (fscanf(in_instructions, "%d", &FuncNum) != EOF)
	{
		switch (FuncNum)
		{
		case 1:
		{
			if (!(in_mana = fopen("Manot.txt", "r")))
			{

				Error_Msg(in_instructions, "The input file is wrong");
			}
			CreateProduct(in_instructions, in_mana, &manu.head);
			fclose(in_mana);
			printf("*Function %d finished sucssesfully*\n",FuncNum);
			break;
		}
		case 2:
		{
			fscanf(in_instructions, "%s%d", DishName, &amount);
			AddItems(DishName, amount, &manu.head);
			printf("*Function %d finished sucssesfully*\n", FuncNum);
			break;
		}
		case 3:
		{
			fscanf(in_instructions, "%d%s%d", &i, DishName, &amount);
			OrderItem(in_instructions, manu.head, restaurant, num_tabels, i, DishName, amount);
			printf("*Function %d finished sucssesfully*\n", FuncNum);
			break;
		}
		case 4:
		{
			fscanf(in_instructions, "%d%s%d", &i, DishName, &amount);
			RemoveItem(manu.head, restaurant, i, DishName, amount);
			printf("*Function %d finished sucssesfully*\n", FuncNum);
			break;
		}
		case 5:
		{
			fscanf(in_instructions, "%d", &i);
			RemoveTable(restaurant, i);
			printf("*Function %d finished sucssesfully*\n", FuncNum);
			break;
		}
		case 6:
		{
			fscanf(in_instructions, "%s%s", temp1, temp2);
			i = atoi(temp2);
			ch = temp1[0];
			Report(manu.head, restaurant, i, ch);
			printf("*Function %d finished sucssesfully*\n", FuncNum);
			break;
		}
		}
		InstructionsManu();
		scanf("%c", &ch);
		rewind(stdin);
		while (ch != '0')
		{
			printf("Error,'0' must be enterd to get instructions from file\n");
			scanf("%c", &ch);
			rewind(stdin);
		}

	}
	DeleteTabels(restaurant, num_tabels);
	DeleteList(manu.head);
	fclose(in_instructions);
}