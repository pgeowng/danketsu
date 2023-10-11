#include "map_loader.h"

void TileMapLoadCSV(TileMap *m, const char *path, Texture *texture,
                    u32 tileWidth, u32 tileHeight) {
  u32 bytesLen = 0;
  void *bytes = SDL_LoadFile(path, (size_t *)&bytesLen);
  if (bytes == nullptr) {
    LogError("TileMapLoadCSV: failed to load file: %s\n", path);
    return;
  }

  Read2DMap((char *)bytes, bytesLen, &m->map[0][0], MAX_MAP_SIZE, MAX_MAP_SIZE,
            &m->height, &m->width);

  m->texture = texture;
  m->tileWidth = tileWidth;
  m->tileHeight = tileHeight;
}

void Read2DMap(const char *buffer, u32 bufferLen, i32 *map, u32 mapRowCap,
               u32 mapColCap, u32 *mapRowLen, u32 *mapColLen) {
  u32 bufferOffset = 0;

  u32 rowLen = 0;
  u32 colLen = 0;
  b8 colLenFound = 0;
  b8 rowBreak = 0;

  for (u32 row = 0; row < mapRowCap; row++) {
    for (u32 column = 0; column < mapColCap; column++) {
      if (bufferOffset >= bufferLen) {
        break;
      }

      if (!colLenFound) {
        colLen++;
      }

      if (!rowBreak) {
        rowLen++;
        rowBreak = 1;
      }

      i32 n = sscanf_s(&buffer[bufferOffset], "%d,",
                       &map[row * mapColCap + column]);
      if (n != 1) {
        LogError("test failed: when parsing row=%d column=%d expected n=1, "
                 "actual n=0\n",
                 row, column);
        return;
      }

      while (bufferOffset < bufferLen && buffer[bufferOffset] != ',' &&
             buffer[bufferOffset] != '\n' && buffer[bufferOffset] != '\0') {
        bufferOffset++;
      }

      if (bufferOffset < bufferLen && buffer[bufferOffset] == '\n') {
        colLenFound = 1;
        rowBreak = 0;

        bufferOffset++;
        break;
      }

      bufferOffset++;
    }
  }

  if (mapRowLen != nullptr) {
    *mapRowLen = rowLen;
  }

  if (mapColLen != nullptr) {
    *mapColLen = colLen;
  }
}