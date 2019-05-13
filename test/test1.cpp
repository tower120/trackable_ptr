#include <stdexcept>
#include <iostream>

int main(){
    std::cout << "test1 strart msg" << std::endl;
      int *array = new int[100];
      delete [] array;
      return array[1];  // BOOM
    return 0;
}