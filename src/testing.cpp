
#include <cassert>

int sum(int min, int max){
  return min + max;
}

int main()
{
  assert(sum(0, 2)  == 3);
  assert(sum(-2, 2) == 0);
  assert(sum(3, 7) == 25);

  return 0;

}
