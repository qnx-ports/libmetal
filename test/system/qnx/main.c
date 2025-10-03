#include "metal-test.h"

int main(void)
{
  int status;

  status = metal_tests_run(NULL);

  return status;
}
