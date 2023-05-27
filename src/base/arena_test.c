#include "arena.c"


#define BEGIN_TESTING int main(int argc, char **argv) {
#define END_TESTING return 0;}
#define TEST(TEST_NAME) if (runTest(TEST_NAME, argc, argv))

int runTest(const char* testName, int argc, char **argv) {
  if (argc == 1) {
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if (!strcmp(testName, argv[i])) {
      return 0;
    }
  }

  return 0;
}

BEGIN_TESTING

TEST("Making arena init") {
  int size = 1000;
  void * buf = malloc(size);

  Arena a = {0};
  ASSERT(a.buf == NULL)
  ASSERT(a.ptr == 0)
  ASSERT(a.cap == 0)

  ArenaInit(&a, buf, size);
  ASSERT(a.buf == buf)
  ASSERT(a.ptr == 0)
  ASSERT(a.cap == size)

  U8* arr = (U8*)ArenaAllocAlign(&a, size, ARENA_DEFAULT_ALIGNMENT);

  for (int i = 0; i < size; i++) {
    if (arr[i] != 0) {
      printf("expected 0, but got %d", arr[i]);
    }
  }

  free(buf);
}

printf("PASS!");



END_TESTING
