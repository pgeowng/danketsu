
typedef struct testCase {
  f32 a[4];
  f32 b[4];
  b8 result;
} testCase;

void runCase(f32 *a, f32 *b, b8 expected) {
  b8 actual = rectOverlaps(a, b);
  if (expected != actual) {
    LogError("test failed: a=[%f,%f,%f,%f], b=[%f,%f,%f,%f], expected=%d\n",
             a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3], expected);
  }
}

u32 stringLen(const char *a) {
  u32 offset = 0;
  while (a[offset] != '\0') {
    offset++;
  }
  return offset;
}

void runTest() {
  {
    const char *a = "1,2,3,4,5\n6";
    u32 len = stringLen(a);

    u32 expected[5][5] = {{1, 2, 3, 4, 5},
                          {6, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0}};
    u32 expectedRowLen = 2;
    u32 expectedColLen = 5;

    u32 mm[5][5] = {0};
    u32 rowCap = 5;
    u32 columnCap = 5;
    u32 rowLen = 0;
    u32 colLen = 0;

    Read2DMap(a, len, &mm[0][0], rowCap, columnCap, &rowLen, &colLen);

    if (expectedRowLen != rowLen) {
      LogError("test failed: rowLen expected=%d actual=%d", expectedRowLen,
               rowLen);
    }

    if (expectedColLen != colLen) {
      LogError("test failed: colLen expected=%d actual=%d", expectedColLen,
               colLen);
    }

    for (u32 r = 0; r < rowCap; r++) {
      for (u32 c = 0; c < columnCap; c++) {
        if (mm[r][c] != expected[r][c]) {
          LogError("test failed: at row=%d column=%d expected=%d actual=%d\n",
                   r, c, expected[r][c], mm[r][c]);
        }
      }
    }
  }

  {
    const char *a = "-1\n6";
    u32 len = stringLen(a);

    u32 expected[2][1] = {{0}, {6}};
    u32 expectedRowLen = 1;
    u32 expectedColLen = 2;

    u32 mm[5][5] = {0};
    u32 rowCap = 5;
    u32 columnCap = 5;
    u32 rowLen = 0;
    u32 colLen = 0;

    Read2DMap(a, len, &mm[0][0], rowCap, columnCap, &rowLen, &colLen);

    if (expectedRowLen != rowLen) {
      LogError("test failed: rowLen expected=%d actual=%d", expectedRowLen,
               rowLen);
    }

    if (expectedColLen != colLen) {
      LogError("test failed: colLen expected=%d actual=%d", expectedColLen,
               colLen);
    }

    for (u32 r = 0; r < rowCap; r++) {
      for (u32 c = 0; c < columnCap; c++) {
        if (mm[r][c] != expected[r][c]) {
          LogError("test failed: at row=%d column=%d expected=%d actual=%d\n",
                   r, c, expected[r][c], mm[r][c]);
        }
      }
    }
  }
}
