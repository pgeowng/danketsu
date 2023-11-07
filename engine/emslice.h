#ifndef EMSLICE_H
#define EMSLICE_H

typedef struct emArr {
  void *data;
  u64 len;
  u64 cap;
  u64 elemSize;
} emArr;

static emArr emArrNewFromElemSize(u64 elemSize, u64 len, u64 cap) {
  emArr arr;
  arr.elemSize = elemSize;
  arr.len = len;
  arr.cap = cap;
  arr.data = PushArrayElemSize(emArena, void *, elemSize *cap);
  return arr;
}

#define emArrNew(c) emArrNewFromElemSize(sizeof(c), 0, 16)
#define emArrNewCap(c, cap) emArrNewFromElemSize(sizeof(c), 0, (cap))
#define emArrGet(a, T, idx) (T *)emArrGetVoidElem((a), (idx))

static void *emArrGetVoidElem(emArr *arr, u64 index) {
  assert(index < arr->len);
  return (void *)((u8 *)arr->data + index * arr->elemSize);
}

static void *emArrPush(emArr *arr) {
  if (arr->len == arr->cap) {
    LogError("emArrPush: resize at push is not implemented");
    return NULL;
  }
  void *result = (void *)((u8 *)arr->data + arr->elemSize * arr->len);
  arr->len++;
  return result;
}

// static void emArray

// static emArr *emArrSlice(emArr *arr, i32 i, i32 n) {}

#endif