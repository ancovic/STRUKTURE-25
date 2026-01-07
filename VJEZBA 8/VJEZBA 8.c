#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* 8. Napisati program koji omogucava rad s binarnim stablom pretrazivanja. Treba
omoguciti unosenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalazenje nekog elementa.*/

typedef struct _treeNode* treePointer;
typedef struct _treeNode {

	int data;
	treePointer left;
	treePointer right;

}treeNode;

treePointer allocateMemory(int element);
treePointer insertElement(int element, treePointer root);
int inOrderPrint(treePointer root);
int postOrderPrint(treePointer root);
int preOrderPrint(treePointer root);
treePointer deleteElement(int element, treePointer root);
treePointer findMin(treePointer root);
treePointer findElement(int element, treePointer root);
int userInterface(treePointer root);

int main()
{
	treePointer root = NULL;
	root = insertElement(2, root);
	root = insertElement(4, root);
	root = insertElement(9, root);
	root = insertElement(1, root);
	root = insertElement(11, root);
	root = insertElement(7, root);
	root = deleteElement(2, root);
	
	userInterface(root);

	return 0;
}

treePointer allocateMemory(int element)
{
	treePointer newNode = (treePointer)malloc(sizeof(treeNode));

	if (!newNode)
	{
		printf("\nERROR: Greska u alokaciji memorije\n");
		return newNode;
	}
	newNode->data = element;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

treePointer insertElement(int element, treePointer root)
{
	if (!root)
		return allocateMemory(element);
	else if (element < root->data)
		root->left = insertElement(element, root->left);
	else if (element > root->data)
		root->right = insertElement(element, root->right);

	return root;
}

int inOrderPrint(treePointer root)
{
	if (root != NULL)
	{
		inOrderPrint(root->left);
		printf("%d\n", root->data);
		inOrderPrint(root->right);
	}

	return 0;
}

int postOrderPrint(treePointer root)
{
	if (root != NULL)
	{
		postOrderPrint(root->left);
		postOrderPrint(root->right);
		printf("%d\n", root->data);
	}

	return 0;
}

int preOrderPrint(treePointer root)
{
	if (root != NULL)
	{
		printf("%d\n", root->data);
		preOrderPrint(root->left);
		preOrderPrint(root->right);
	}

	return 0;
}

treePointer deleteElement(int element, treePointer root)
{
	treePointer temp = NULL;

	if (!root)
		return root;
	else if (element < root->data)
		root->left = deleteElement(element, root->left);
	else if (element > root->data)
		root->right = deleteElement(element, root->right);
	else
	{
		if (root->left && root->right)
		{
			temp = findMin(root->right);
			root->data = temp->data;
			root->right = deleteElement(temp->data, root->right);
		}
		else
		{
			temp = root;
			if (!root->left)
				root = root->right;
			else if (!root->right)
				root = root->left;
			free(temp);
		}
	}

	return root;
}

treePointer findMin(treePointer root)
{
	while (root->left)
		root = root->left;
	return root;
}

treePointer findElement(int element, treePointer root)
{
	if (!root)
		return root;
	else if (element < root->data)
		return findElement(element, root->left);
	else if (element > root->data)
		return findElement(element, root->right);
	else
		return root;
}

int userInterface(treePointer root) {

	int choice = 0, element = 0;
	treePointer temp = NULL;

	do
	{
		printf("Odaberi:\n\n1 - unos novog elementa u stablo\n2 - ispis inorder stabla\n3 - ispis postorder stabla\n4 - ispis preorder stabla\n5 - trazenje elementa stabla\n6 - brisanje elementa iz stabla\n7 - izlaz\n");
		printf("\nUnesite odabir: ");
		scanf("%d", &choice);
		system("cls");

		switch (choice) {

		case 1:
			printf("Unesite element koji zelite unijeti u stablo:\n");
			scanf("%d", &element);
			root = insertElement(element, root);
			break;

		case 2:
			inOrderPrint(root);
			break;

		case 3:
			postOrderPrint(root);
			break;

		case 4:
			preOrderPrint(root);
			break;

		case 5:
			printf("Unesite element koji zelite pronaci u stablu:\n");
			scanf("%d", &element);
			temp = findElement(element, root);
			if (temp)
				printf("Element %d se nalazi na adresi %d\n", temp->data, temp);

			break;

		case 6:
			printf("Unesite element koji zelite izbrisati:\n");
			scanf("%d", &element);
			root = deleteElement(element, root);

			break;

		case 7:
			break;

		default:
			printf("Pogresan unos!\n");
		}

	} while (choice != 7);

	return 0;
}