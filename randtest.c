#include <stdio.h>
#include <stdlib.h>

int main(){
  for(int i=0; i<5; i++){
    printf("%d: %d\n" , i, rand());
  }
}
