#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#define MAX_MAP_SIZE 20

typedef struct TileMap {
  i32 map[MAX_MAP_SIZE][MAX_MAP_SIZE];
  u32 width;
  u32 height;
  Texture *texture;

  // tile is size of one tile.
  u32 tileWidth;
  u32 tileHeight;

  // offsetStep is used for offset tile grid.
  u32 offsetStepX;
  u32 offsetStepY;
} TileMap;

void TileMapLoadCSV(TileMap *m, const char *path, Texture *texture,
                    u32 tileWidth, u32 tileHeight);
void Read2DMap(const char *buffer, u32 bufferLen, i32 *map, u32 mapRowCap,
               u32 mapColumnCap, u32 *rowLen, u32 *colLen);

static i32 TileMapTileByPosition(TileMap *m, v2 position);

static void TileMapInit(TileMap *m, Texture *texture, u32 tileWidth,
                        u32 tileHeight, u32 offsetStepX, u32 offsetStepY) {
  m->width = 0;
  m->height = 0;
  m->texture = texture;
  m->tileWidth = tileWidth;
  m->tileHeight = tileHeight;
  m->offsetStepX = offsetStepX;
  m->offsetStepY = offsetStepY;
}

static void TileMapUVByIdx(v4 uvOut, TileMap *tm, i32 index) {
  u32 xCap = (u32)(tm->texture->width / tm->offsetStepX);

  uvOut[0] = f32(tm->offsetStepX) / tm->texture->width * (index % xCap);
  uvOut[1] = f32(tm->offsetStepY) / tm->texture->height * (index / xCap);
  uvOut[2] = f32(tm->tileWidth) / tm->texture->width;
  uvOut[3] = f32(tm->tileHeight) / tm->texture->height;
}

#endif