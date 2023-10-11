#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#define MAX_MAP_SIZE 20

typedef struct TileMap {
  i32 map[MAX_MAP_SIZE][MAX_MAP_SIZE];
  u32 width;
  u32 height;
  Texture *texture;
  u32 tileWidth;
  u32 tileHeight;
} TileMap;

void TileMapLoadCSV(TileMap *m, const char *path, Texture *texture,
                    u32 tileWidth, u32 tileHeight);
void Read2DMap(const char *buffer, u32 bufferLen, i32 *map, u32 mapRowCap,
               u32 mapColumnCap, u32 *rowLen, u32 *colLen);

#endif