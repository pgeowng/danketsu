#include "file.h"

// static bool emReadFileCRT(MArena *a, const char *name, u8 **bytes) {
//   i32 fd = 0;

//   i32 status = _sopen_s(&fd, name, _O_RDONLY, _SH_DENYNO, _S_IREAD |
//   _S_IWRITE); if (fd == -1) {
//     LogError("emReadFile failed: %s", name);
//     return 1;
//   }

//   struct _stat statBuf;

//   i32 result = _fstat(fd, &statBuf);
//   if (result != 0) {
//     if (errno == EBADF)
//       LogError("emReadFile failed: fstat: Bad file descriptor");
//     else if (errno == EINVAL)
//       LogError("emReadFile failed: fstat: Invalid argument to fstat");

//     _close(fd);
//     return 1;
//   }

//   LogInfo("emReadFile: %s filesize %ld", name, statBuf.st_size);
//   i32 size = statBuf.st_size;
//   size++;
//   if (size < 512) {
//     size = 512;
//   }
//   u8 *buf = (u8 *)MArenaPush(a, size * sizeof(u8));

//   _close(fd);
//   return 0;
// }

static void LogGetLastError(str8 name) {
  DWORD dw = GetLastError();
  LPVOID lpMsgBuf;
  // https://stackoverflow.com/questions/455434/how-should-i-use-formatmessage-properly-in-c
  // https://devblogs.microsoft.com/oldnewthing/20071128-00/?p=24353
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

  LogInfo("emReadFile %s: GetLastError: dw: %d, error: %s", name.str, dw,
          lpMsgBuf);

  LocalFree(lpMsgBuf);
}

static bool emReadFile(MArena *a, str8 name, str8 *resultBuffer) {
  assert(resultBuffer != NULL);

  bool err = 0;
  HANDLE hFile;

  str16 b = str32To16(a, str8To32(a, name));
  hFile = CreateFileW((LPCWSTR)b.str, GENERIC_READ, FILE_SHARE_READ, NULL,
                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE) {
    LogError("emReadFile %s: unable to open file for read", name);
    LogGetLastError(name);
    err = 1;
    goto defer;
  }

  LARGE_INTEGER fileSize;
  {
    bool ok = GetFileSizeEx(hFile, &fileSize);
    if (!ok) {
      LogError("emReadFile %s: unable to get size of file", name);

      err = 1;
      goto defer;
    }
  }

  i32 size = fileSize.LowPart;
  LogInfo("emReadFile %s: filesize %d", name, fileSize);
  size++;
  i32 bufSize = size;
  if (bufSize < 512) {
    bufSize = 512;
  }

  u8 *buf = (u8 *)MArenaPush(a, bufSize * sizeof(u8));
  DWORD bytesRead = 0;
  {
    bool ok = ReadFile(hFile, buf, size, &bytesRead, NULL);
    if (!ok) {

      LogInfo("emReadFile %s: unable to read file", name);
      LogGetLastError(name);

      err = 1;
      goto defer;
    }
  }

  err = 0;
  buf[size - 1] = '\0';
  resultBuffer->str = buf;
  resultBuffer->size = size;

defer:
  CloseHandle(hFile);
  return err;
}
