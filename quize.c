#include <stdio.h>

int main()
{
  char* str[2];
  str[0] = "hello!";
  str[1] = "jungler";
  int* a = str;

  printf("1. %s\n", str[0]);
  printf("%c\n", *(a));
  printf("1. %s\n", (str+1)[0]);
  // int numArr[5] = {11,22,33,44,55};
  // int *numPtrA;
  // void *ptr;

  // numPtrA = &numArr[2];
  // ptr = numArr;

  // printf("%d\n", *(numPtrA+2));
  // printf("%d\n", *((int*)ptr+1));
}