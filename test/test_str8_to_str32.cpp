#

void runTest() {
  MArena a = MArenaMake(MMallocBaseMemory());
  MArena *m = &a;

  {
    u8 input[2] = {0x24, 0x00};
    str32 b = str8To32(m, str8Lit(input));
    assert(b.size == 1);
    assert(b.str[0] == 0x24);
    assert(b.hasNull);
  }
  {
    u8 input[3];
    input[0] = 0xC2;
    input[1] = 0xA2;
    input[2] = 0x00;
    str32 b = str8To32(m, str8Lit(input));
    assert(b.size == 1);
    assert(b.str[0] == 0xA2);
    assert(b.hasNull);
  }
  {
    u8 input[4];
    input[0] = 0xE2;
    input[1] = 0x82;
    input[2] = 0xAC;
    input[3] = 0x00;
    str32 b = str8To32(m, str8Lit(input));
    assert(b.size == 1);
    assert(b.str[0] == 0x20AC);
    assert(b.hasNull);
  }
  {
    u8 input[5];
    input[0] = 0xF0;
    input[1] = 0x90;
    input[2] = 0x8D;
    input[3] = 0x88;
    input[4] = 0x00;
    str32 b = str8To32(m, str8Lit(input));
    assert(b.size == 1);
    assert(b.str[0] == 0x10348);
    assert(b.hasNull);
  }

  {
    u32 input[1] = {0x3042};
    str16 b = str32To16(m, str32New(input, ArrayCount(input), 0));
    assert(b.size == 1);
    assert(b.str[0] == 0x3042);
    assert(b.hasNull);
  }

  {
    u32 input[1] = {0x1F64B};
    str16 b = str32To16(m, str32New(input, ArrayCount(input), 0));
    assert(b.size == 2);
    assert(b.str[0] == 0xD83D);
    assert(b.str[1] == 0xDE4B);
    assert(b.hasNull);
  }
}