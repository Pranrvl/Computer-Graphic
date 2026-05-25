/*
 * Mini Minecraft - Enhanced Graphics Version
 * Features:
 * - Beautiful colored blocks with shading
 * - Infinite procedural terrain with trees
 * - Smooth lighting and fog effects
 * - Inventory system
 * - Block place/break mechanics
 * 
 * Controls:
 * - WASD: Move | Mouse: Look | Space: Jump
 * - Left Click: Break | Right Click: Place
 * - 1-7: Select block | E: Inventory | ESC: Exit
 */

#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <cstdio>

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 32;
const int RENDER_DISTANCE = 2;
const float PI = 3.14159265f;

enum BlockType {
    AIR = 0, GRASS, DIRT, STONE, WOOD, LEAVES, WATER, SAND,
    COAL, IRON, GOLD, DIAMOND, BEDROCK, BLOCK_COUNT
};

const char* blockNames[] = {
    "Air", "Grass", "Dirt", "Stone", "Wood", "Leaves",
    "Water", "Sand", "Coal", "Iron", "Gold", "Diamond", "Bedrock"
};

struct Vec3 { float x, y, z; };
struct Color4 { float r, g, b, a; };

struct ChunkKey {
    int x, z;
    bool operator<(const ChunkKey& o) const {
        return x != o.x ? x < o.x : z < o.z;
    }
};

struct Chunk {
    int blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    bool ready;
    GLuint displayList; // <-- For optimization
    bool dirty;         // <-- To flag for rebuild
    Chunk() : ready(false), displayList(0), dirty(true) { // Initialize new members
        for(int i=0;i<CHUNK_SIZE;i++)
            for(int j=0;j<CHUNK_HEIGHT;j++)
                for(int k=0;k<CHUNK_SIZE;k++)
                    blocks[i][j][k] = AIR;
    }
    ~Chunk() {
        if (displayList) {
            glDeleteLists(displayList, 1);
        }
    }
};

std::map<ChunkKey, Chunk> world;

// Mob Entity
struct Pig {
    float x, y, z;
    float yaw;
    float speed;
    int walkTimer;
    int health; // <-- Pig health
};
std::vector<Pig> pigs;

// Player State
float playerHealth = 10.0f;
const float maxHealth = 10.0f;
float fallDistance = 0.0f;

float camX = 0, camY = 20, camZ = 0;
float velY = 0, yaw = 0, pitch = 0;
bool grounded = false;
int selBlock = GRASS;
int inventory[BLOCK_COUNT] = {0};
bool invOpen = false;
bool keys[256] = {false};
int winW = 1280, winH = 720;
int mouseLastX = 640, mouseLastY = 360;
bool firstMouse = true;

// Forward declaration for use in spawnPigs
int getBlock(int x, int y, int z);

// Enhanced color palette - more vibrant
Color4 getTopColor(int t) {
    switch(t) {
        case GRASS:   return {0.35f, 0.85f, 0.25f, 1.0f};  // Bright green
        case DIRT:    return {0.60f, 0.40f, 0.25f, 1.0f};  // Brown
        case STONE:   return {0.55f, 0.55f, 0.58f, 1.0f};  // Gray
        case WOOD:    return {0.70f, 0.55f, 0.35f, 1.0f};  // Light wood
        case LEAVES:  return {0.20f, 0.70f, 0.20f, 1.0f};  // Green
        case WATER:   return {0.25f, 0.50f, 0.95f, 0.7f};  // Blue
        case SAND:    return {0.95f, 0.90f, 0.65f, 1.0f};  // Yellow
        case COAL:    return {0.20f, 0.20f, 0.22f, 1.0f};  // Dark
        case IRON:    return {0.85f, 0.75f, 0.70f, 1.0f};  // Tan
        case GOLD:    return {1.00f, 0.90f, 0.30f, 1.0f};  // Gold
        case DIAMOND: return {0.50f, 0.95f, 1.00f, 1.0f};  // Cyan
        case BEDROCK: return {0.15f, 0.15f, 0.18f, 1.0f};  // Very dark
        default:      return {1.0f, 0.0f, 1.0f, 1.0f};
    }
}

Color4 getSideColor(int t) {
    switch(t) {
        case GRASS:   return {0.55f, 0.40f, 0.25f, 1.0f};  // Dirt side
        case DIRT:    return {0.55f, 0.35f, 0.20f, 1.0f};
        case STONE:   return {0.50f, 0.50f, 0.52f, 1.0f};
        case WOOD:    return {0.50f, 0.35f, 0.18f, 1.0f};  // Bark
        case LEAVES:  return {0.18f, 0.60f, 0.18f, 1.0f};
        case WATER:   return {0.20f, 0.40f, 0.85f, 0.7f};
        case SAND:    return {0.90f, 0.85f, 0.60f, 1.0f};
        case COAL:    return {0.22f, 0.22f, 0.25f, 1.0f};
        case IRON:    return {0.75f, 0.65f, 0.60f, 1.0f};
        case GOLD:    return {0.95f, 0.80f, 0.25f, 1.0f};
        case DIAMOND: return {0.45f, 0.85f, 0.90f, 1.0f};
        case BEDROCK: return {0.12f, 0.12f, 0.15f, 1.0f};
        default:      return {1.0f, 0.0f, 1.0f, 1.0f};
    }
}

Color4 getBottomColor(int t) {
    Color4 c = getSideColor(t);
    c.r *= 0.7f; c.g *= 0.7f; c.b *= 0.7f;
    return c;
}

// Perlin noise for terrain
float noise2D(float x, float z) {
    int n = (int)x + (int)z * 57;
    n = (n << 13) ^ n;
    return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
}

float smoothNoise(float x, float z) {
    float corners = (noise2D(x-1,z-1) + noise2D(x+1,z-1) + noise2D(x-1,z+1) + noise2D(x+1,z+1)) / 16.0f;
    float sides = (noise2D(x-1,z) + noise2D(x+1,z) + noise2D(x,z-1) + noise2D(x,z+1)) / 8.0f;
    return corners + sides + noise2D(x,z) / 4.0f;
}

float interpolate(float a, float b, float t) {
    float f = (1.0f - cosf(t * PI)) * 0.5f;
    return a*(1-f) + b*f;
}

float interpNoise(float x, float z) {
    int ix = (int)floorf(x), iz = (int)floorf(z);
    float fx = x - ix, fz = z - iz;
    float v1 = smoothNoise((float)ix, (float)iz);
    float v2 = smoothNoise((float)(ix+1), (float)iz);
    float v3 = smoothNoise((float)ix, (float)(iz+1));
    float v4 = smoothNoise((float)(ix+1), (float)(iz+1));
    return interpolate(interpolate(v1,v2,fx), interpolate(v3,v4,fx), fz);
}

float perlin(float x, float z, int oct, float pers) {
    float total = 0, freq = 1, amp = 1, maxVal = 0;
    for(int i = 0; i < oct; i++) {
        total += interpNoise(x*freq, z*freq) * amp;
        maxVal += amp;
        amp *= pers;
        freq *= 2;
    }
    return total / maxVal;
}

int getHeight(int wx, int wz) {
    float n = perlin(wx * 0.015f, wz * 0.015f, 5, 0.5f);
    return (int)(12 + n * 10);
}

void genChunk(ChunkKey k) {
    Chunk& c = world[k];
    if(c.ready) return;
    
    int bx = k.x * CHUNK_SIZE, bz = k.z * CHUNK_SIZE;
    
    for(int x = 0; x < CHUNK_SIZE; x++) {
        for(int z = 0; z < CHUNK_SIZE; z++) {
            int wx = bx + x, wz = bz + z;
            int h = getHeight(wx, wz);
            
            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                if(y == 0) {
                    c.blocks[x][y][z] = BEDROCK;
                } else if(y < h - 4) {
                    c.blocks[x][y][z] = STONE;
                    // Ores
                    float ore = noise2D((float)(wx + y*100), (float)(wz + y*50));
                    if(y < 8 && ore > 0.94f) c.blocks[x][y][z] = DIAMOND;
                    else if(y < 14 && ore > 0.89f) c.blocks[x][y][z] = GOLD;
                    else if(y < 24 && ore > 0.84f) c.blocks[x][y][z] = IRON;
                    else if(ore > 0.78f) c.blocks[x][y][z] = COAL;
                } else if(y < h) {
                    c.blocks[x][y][z] = DIRT;
                } else if(y == h) {
                    c.blocks[x][y][z] = (h < 8) ? SAND : GRASS;
                } else if(y < 7) {
                    c.blocks[x][y][z] = WATER;
                }
            }
            
            // Trees
            if(h >= 8 && h < CHUNK_HEIGHT - 7) {
                float tn = noise2D(wx*0.4f, wz*0.4f);
                if(tn > 0.82f && x > 2 && x < CHUNK_SIZE-3 && z > 2 && z < CHUNK_SIZE-3) {
                    int th = 5 + (int)(tn * 2);
                    for(int ty = 1; ty <= th; ty++)
                        if(h+ty < CHUNK_HEIGHT) c.blocks[x][h+ty][z] = WOOD;
                    
                    for(int lx = -2; lx <= 2; lx++) {
                        for(int lz = -2; lz <= 2; lz++) {
                            for(int ly = th-2; ly <= th+2; ly++) {
                                if(abs(lx)+abs(lz) <= 3 && h+ly < CHUNK_HEIGHT) {
                                    int nx = x+lx, nz = z+lz;
                                    if(nx >= 0 && nx < CHUNK_SIZE && nz >= 0 && nz < CHUNK_SIZE) {
                                        if(c.blocks[nx][h+ly][nz] == AIR)
                                            c.blocks[nx][h+ly][nz] = LEAVES;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    c.ready = true;
}

void spawnPigs() {
    int pcx = (int)floorf(camX / CHUNK_SIZE);
    int pcz = (int)floorf(camZ / CHUNK_SIZE);

    if (pigs.size() < 20 && (rand() % 100) < 5) {
        int spawnChunkX = pcx + (rand() % (RENDER_DISTANCE * 2 + 1)) - RENDER_DISTANCE;
        int spawnChunkZ = pcz + (rand() % (RENDER_DISTANCE * 2 + 1)) - RENDER_DISTANCE;
        
        int localX = rand() % CHUNK_SIZE;
        int localZ = rand() % CHUNK_SIZE;

        int worldX = spawnChunkX * CHUNK_SIZE + localX;
        int worldZ = spawnChunkZ * CHUNK_SIZE + localZ;

        int height = getHeight(worldX, worldZ);
        if (getBlock(worldX, height, worldZ) == GRASS) {
            pigs.push_back({(float)worldX + 0.5f, (float)height + 1.5f, (float)worldZ + 0.5f, (float)(rand() % 360), 0.02f, 0, 10});
        }
    }
}

int getBlock(int x, int y, int z) {
    if(y < 0 || y >= CHUNK_HEIGHT) return AIR;
    int cx = (int)floorf((float)x / CHUNK_SIZE);
    int cz = (int)floorf((float)z / CHUNK_SIZE);
    int lx = x - cx * CHUNK_SIZE;
    int lz = z - cz * CHUNK_SIZE;
    ChunkKey k = {cx, cz};
    auto it = world.find(k);
    if(it == world.end()) return AIR;
    return it->second.blocks[lx][y][lz];
}

void setBlock(int x, int y, int z, int t) {
    if(y < 0 || y >= CHUNK_HEIGHT) return;
    int cx = (int)floorf((float)x / CHUNK_SIZE);
    int cz = (int)floorf((float)z / CHUNK_SIZE);
    int lx = x - cx * CHUNK_SIZE;
    int lz = z - cz * CHUNK_SIZE;
    ChunkKey k = {cx, cz};
    auto it = world.find(k);
    if(it != world.end()) {
        it->second.blocks[lx][y][lz] = t;
        it->second.dirty = true; // Mark chunk as dirty
        // Also mark neighboring chunks if on a border
        if (lx == 0) {
            ChunkKey nk = {cx - 1, cz};
            if (world.count(nk)) world[nk].dirty = true;
        } else if (lx == CHUNK_SIZE - 1) {
            ChunkKey nk = {cx + 1, cz};
            if (world.count(nk)) world[nk].dirty = true;
        }
        if (lz == 0) {
            ChunkKey nk = {cx, cz - 1};
            if (world.count(nk)) world[nk].dirty = true;
        } else if (lz == CHUNK_SIZE - 1) {
            ChunkKey nk = {cx, cz + 1};
            if (world.count(nk)) world[nk].dirty = true;
        }
    }
}

void drawFace(float x, float y, float z, int face, Color4 col) {
    glColor4f(col.r, col.g, col.b, col.a);
    glBegin(GL_QUADS);
    switch(face) {
        case 0: // Top
            glNormal3f(0,1,0);
            glVertex3f(x,y+1,z); glVertex3f(x+1,y+1,z);
            glVertex3f(x+1,y+1,z+1); glVertex3f(x,y+1,z+1);
            break;
        case 1: // Bottom
            glNormal3f(0,-1,0);
            glVertex3f(x,y,z); glVertex3f(x,y,z+1);
            glVertex3f(x+1,y,z+1); glVertex3f(x+1,y,z);
            break;
        case 2: // Front (+Z)
            glNormal3f(0,0,1);
            glVertex3f(x,y,z+1); glVertex3f(x,y+1,z+1);
            glVertex3f(x+1,y+1,z+1); glVertex3f(x+1,y,z+1);
            break;
        case 3: // Back (-Z)
            glNormal3f(0,0,-1);
            glVertex3f(x,y,z); glVertex3f(x+1,y,z);
            glVertex3f(x+1,y+1,z); glVertex3f(x,y+1,z);
            break;
        case 4: // Right (+X)
            glNormal3f(1,0,0);
            glVertex3f(x+1,y,z); glVertex3f(x+1,y,z+1);
            glVertex3f(x+1,y+1,z+1); glVertex3f(x+1,y+1,z);
            break;
        case 5: // Left (-X)
            glNormal3f(-1,0,0);
            glVertex3f(x,y,z); glVertex3f(x,y+1,z);
            glVertex3f(x,y+1,z+1); glVertex3f(x,y,z+1);
            break;
    }
    glEnd();
}

bool isTransparent(int t) { return t == AIR || t == WATER; }

void drawBlock(int x, int y, int z, int t) {
    if(t == AIR) return;
    float fx = (float)x, fy = (float)y, fz = (float)z;
    
    // Always draw top face - fix for visual bug
    int topBlock = getBlock(x, y+1, z);
    if(topBlock == AIR || topBlock == WATER) 
        drawFace(fx,fy,fz, 0, getTopColor(t));
    
    if(y==0 || isTransparent(getBlock(x,y-1,z))) 
        drawFace(fx,fy,fz, 1, getBottomColor(t));
    
    if(isTransparent(getBlock(x,y,z+1))) 
        drawFace(fx,fy,fz, 2, getSideColor(t));
    
    if(isTransparent(getBlock(x,y,z-1))) 
        drawFace(fx,fy,fz, 3, getSideColor(t));
    
    if(isTransparent(getBlock(x+1,y,z))) 
        drawFace(fx,fy,fz, 4, getSideColor(t));
    
    if(isTransparent(getBlock(x-1,y,z))) 
        drawFace(fx,fy,fz, 5, getSideColor(t));
}

void drawPig(const Pig& pig) {
    glPushMatrix();
    glTranslatef(pig.x, pig.y, pig.z);
    glRotatef(pig.yaw, 0, 1, 0);

    // Body
    glColor3f(1.0f, 0.7f, 0.8f);
    glPushMatrix();
    glScalef(1.0f, 0.6f, 0.5f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Head
    glColor3f(1.0f, 0.8f, 0.9f);
    glPushMatrix();
    glTranslatef(0.6f, 0.2f, 0.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Legs
    glColor3f(0.9f, 0.6f, 0.7f);
    glPushMatrix();
    glTranslatef(0.4f, -0.4f, 0.2f);
    glScalef(0.15f, 0.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4f, -0.4f, 0.2f);
    glScalef(0.15f, 0.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4f, -0.4f, -0.2f);
    glScalef(0.15f, 0.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4f, -0.4f, -0.2f);
    glScalef(0.15f, 0.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void buildChunk(Chunk& c, ChunkKey k) {
    if (c.displayList) {
        glDeleteLists(c.displayList, 1);
    }
    c.displayList = glGenLists(1);
    glNewList(c.displayList, GL_COMPILE);

    int bx = k.x * CHUNK_SIZE, bz = k.z * CHUNK_SIZE;
    
    // Solid blocks
    for(int x=0; x<CHUNK_SIZE; x++)
        for(int y=0; y<CHUNK_HEIGHT; y++)
            for(int z=0; z<CHUNK_SIZE; z++) {
                int t = c.blocks[x][y][z];
                if(t != AIR && t != WATER) drawBlock(bx+x, y, bz+z, t);
            }
    
    // Water (transparent) - drawn separately
    glEndList();
    c.dirty = false;
}

void drawChunk(ChunkKey k) {
    Chunk& c = world[k];
    if (c.dirty) {
        buildChunk(c, k);
    }
    glCallList(c.displayList);

    // Draw transparent parts separately, not in display list
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int bx = k.x * CHUNK_SIZE, bz = k.z * CHUNK_SIZE;
    for(int x=0; x<CHUNK_SIZE; x++)
        for(int y=0; y<CHUNK_HEIGHT; y++)
            for(int z=0; z<CHUNK_SIZE; z++)
                if(c.blocks[x][y][z] == WATER) {
                    float fx = (float)(bx+x), fy = (float)y, fz = (float)(bz+z);
                    glColor4f(0.25f, 0.50f, 0.95f, 0.65f);
                    glBegin(GL_QUADS);
                    glNormal3f(0,1,0);
                    glVertex3f(fx, fy+0.85f, fz);
                    glVertex3f(fx+1, fy+0.85f, fz);
                    glVertex3f(fx+1, fy+0.85f, fz+1);
                    glVertex3f(fx, fy+0.85f, fz+1);
                    glEnd();
                }
    glDisable(GL_BLEND);
}

void updateWorld() {
    int pcx = (int)floorf(camX / CHUNK_SIZE);
    int pcz = (int)floorf(camZ / CHUNK_SIZE);
    for(int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        for(int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++) {
            ChunkKey k = {pcx+dx, pcz+dz};
            if(world.find(k) == world.end() || !world[k].ready)
                genChunk(k);
        }
}

bool isSolid(int x, int y, int z) {
    int b = getBlock(x,y,z);
    return b != AIR && b != WATER;
}

bool collides(float x, float y, float z) {
    float w = 0.3f, h = 1.8f;
    for(int dy = 0; dy < (int)ceilf(h); dy++) {
        if(isSolid((int)floorf(x-w), (int)floorf(y+dy), (int)floorf(z-w)) ||
           isSolid((int)floorf(x+w), (int)floorf(y+dy), (int)floorf(z-w)) ||
           isSolid((int)floorf(x-w), (int)floorf(y+dy), (int)floorf(z+w)) ||
           isSolid((int)floorf(x+w), (int)floorf(y+dy), (int)floorf(z+w)))
            return true;
    }
    return false;
}

bool raycast(float& hx, float& hy, float& hz, int& hf, float maxD = 5.0f) {
    float ry = yaw * PI / 180.0f;
    float rp = pitch * PI / 180.0f;
    float dx = -sinf(ry) * cosf(rp);
    float dy = sinf(rp);
    float dz = -cosf(ry) * cosf(rp);
    float eyeY = camY + 1.6f;
    
    float step = 0.05f;
    float lx = camX, ly = eyeY, lz = camZ;
    
    for(float t = 0; t < maxD; t += step) {
        float x = camX + dx*t;
        float y = eyeY + dy*t;
        float z = camZ + dz*t;
        int bx = (int)floorf(x), by = (int)floorf(y), bz = (int)floorf(z);
        
        if(isSolid(bx, by, bz)) {
            hx = (float)bx; hy = (float)by; hz = (float)bz;
            int lbx = (int)floorf(lx), lby = (int)floorf(ly), lbz = (int)floorf(lz);
            if(lbx < bx) hf = 5;
            else if(lbx > bx) hf = 4;
            else if(lby < by) hf = 1;
            else if(lby > by) hf = 0;
            else if(lbz < bz) hf = 3;
            else hf = 2;
            return true;
        }
        lx = x; ly = y; lz = z;
    }
    return false;
}

void text2D(float x, float y, const char* s, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    while(*s) glutBitmapCharacter(font, *s++);
}

void drawPixelHeart(float x, float y, float size, bool full) {
    if(full) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for full heart
    } else {
        glColor3f(0.3f, 0.3f, 0.3f); // Gray for empty heart
    }
    
    // Draw a simple pixel heart shape
    glBegin(GL_QUADS);
    // Left bump
    glVertex2f(x, y+size*0.5f);
    glVertex2f(x+size*0.3f, y+size*0.5f);
    glVertex2f(x+size*0.3f, y+size*0.8f);
    glVertex2f(x, y+size*0.8f);
    
    glVertex2f(x+size*0.1f, y+size*0.8f);
    glVertex2f(x+size*0.4f, y+size*0.8f);
    glVertex2f(x+size*0.4f, y+size);
    glVertex2f(x+size*0.1f, y+size);
    glEnd();
    
    glBegin(GL_QUADS);
    // Right bump
    glVertex2f(x+size*0.7f, y+size*0.5f);
    glVertex2f(x+size, y+size*0.5f);
    glVertex2f(x+size, y+size*0.8f);
    glVertex2f(x+size*0.7f, y+size*0.8f);
    
    glVertex2f(x+size*0.6f, y+size*0.8f);
    glVertex2f(x+size*0.9f, y+size*0.8f);
    glVertex2f(x+size*0.9f, y+size);
    glVertex2f(x+size*0.6f, y+size);
    glEnd();
    
    // Bottom triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(x+size*0.5f, y);
    glVertex2f(x, y+size*0.5f);
    glVertex2f(x+size, y+size*0.5f);
    glEnd();
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    // Crosshair
    glColor3f(1, 1, 1);
    glLineWidth(2);
    float cx = winW/2.0f, cy = winH/2.0f;
    glBegin(GL_LINES);
    glVertex2f(cx-12, cy); glVertex2f(cx+12, cy);
    glVertex2f(cx, cy-12); glVertex2f(cx, cy+12);
    glEnd();
    
    // Hotbar
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.15f, 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(cx-200, 8); glVertex2f(cx+200, 8);
    glVertex2f(cx+200, 65); glVertex2f(cx-200, 65);
    glEnd();
    
    for(int i = 1; i <= 7; i++) {
        float sx = cx - 175 + (i-1)*52;
        
        if(i == selBlock) {
            glColor3f(1,1,1);
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            glVertex2f(sx-2, 12); glVertex2f(sx+46, 12);
            glVertex2f(sx+46, 60); glVertex2f(sx-2, 60);
            glEnd();
        }
        
        Color4 col = getTopColor(i);
        glColor3f(col.r, col.g, col.b);
        glBegin(GL_QUADS);
        glVertex2f(sx+4, 18); glVertex2f(sx+40, 18);
        glVertex2f(sx+40, 54); glVertex2f(sx+4, 54);
        glEnd();
        
        // Add 3D effect
        Color4 side = getSideColor(i);
        glColor3f(side.r*0.8f, side.g*0.8f, side.b*0.8f);
        glBegin(GL_QUADS);
        glVertex2f(sx+4, 18); glVertex2f(sx+4, 54);
        glVertex2f(sx+10, 50); glVertex2f(sx+10, 22);
        glEnd();
        
        glColor3f(1,1,1);
        char num[2] = {(char)('0'+i), 0};
        text2D(sx+20, 20, num, GLUT_BITMAP_HELVETICA_12);
    }
    
    glColor3f(1,1,1);
    char info[64];
    sprintf(info, "%s", blockNames[selBlock]);
    text2D(10, winH-28, info);
    
    sprintf(info, "%.0f, %.0f, %.0f", camX, camY, camZ);
    text2D(10, winH-52, info);
    
    // Inventory
    if(invOpen) {
        glColor4f(0, 0, 0, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(cx-220, cy-170); glVertex2f(cx+220, cy-170);
        glVertex2f(cx+220, cy+170); glVertex2f(cx-220, cy+170);
        glEnd();
        
        // Health bar with pixel hearts at top of inventory
        glColor3f(1.0f, 1.0f, 1.0f);
        text2D(cx-200, cy+150, "Health:", GLUT_BITMAP_HELVETICA_12);
        for(int i = 0; i < 10; i++) {
            float hx = cx - 150 + i * 35;
            float hy = cy + 130;
            drawPixelHeart(hx, hy, 20.0f, i < (int)playerHealth);
        }
        
        glColor3f(1,1,1);
        text2D(cx-45, cy+80, "INVENTORY");
        
        for(int i = 1; i < BLOCK_COUNT; i++) {
            int row = (i-1)/4, col = (i-1)%4;
            float ix = cx - 160 + col*85;
            float iy = cy - 20 - row*65;
            
            Color4 c = getTopColor(i);
            glColor3f(c.r, c.g, c.b);
            glBegin(GL_QUADS);
            glVertex2f(ix, iy-35); glVertex2f(ix+45, iy-35);
            glVertex2f(ix+45, iy+10); glVertex2f(ix, iy+10);
            glEnd();
            
            glColor3f(1,1,1);
            char cnt[16];
            sprintf(cnt, "%d", inventory[i]);
            text2D(ix+50, iy-18, cnt, GLUT_BITMAP_HELVETICA_12);
        }
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    // Sky gradient
    glClearColor(0.45f, 0.70f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float ry = yaw * PI / 180.0f;
    float rp = pitch * PI / 180.0f;
    float eyeY = camY + 1.6f;
    float lx = camX - sinf(ry)*cosf(rp);
    float ly = eyeY + sinf(rp);
    float lz = camZ - cosf(ry)*cosf(rp);
    gluLookAt(camX, eyeY, camZ, lx, ly, lz, 0, 1, 0);
    
    // Enhanced lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    // Sun light
    GLfloat sunPos[] = {0.4f, 1.0f, 0.3f, 0.0f};
    GLfloat sunAmb[] = {0.35f, 0.35f, 0.4f, 1.0f};
    GLfloat sunDif[] = {1.0f, 0.95f, 0.85f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDif);
    
    // Fill light
    GLfloat fillPos[] = {-0.3f, -0.5f, 0.2f, 0.0f};
    GLfloat fillDif[] = {0.2f, 0.25f, 0.35f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, fillPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, fillDif);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Fog for atmosphere
    glEnable(GL_FOG);
    GLfloat fogCol[] = {0.55f, 0.75f, 1.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogCol);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 40.0f);
    glFogf(GL_FOG_END, 80.0f);
    
    // Draw chunks
    int pcx = (int)floorf(camX / CHUNK_SIZE);
    int pcz = (int)floorf(camZ / CHUNK_SIZE);
    for(int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        for(int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++) {
            ChunkKey k = {pcx+dx, pcz+dz};
            if(world.find(k) != world.end())
                drawChunk(k);
        }
    
    // Draw pigs
    for (const auto& pig : pigs) {
        drawPig(pig);
    }
    
    glDisable(GL_FOG);
    
    // Selection outline
    float hx, hy, hz; int hf;
    if(raycast(hx, hy, hz, hf)) {
        glDisable(GL_LIGHTING);
        glColor3f(0, 0, 0);
        glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(hx, hy, hz); glVertex3f(hx+1, hy, hz);
        glVertex3f(hx+1, hy+1, hz); glVertex3f(hx, hy+1, hz);
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex3f(hx, hy, hz+1); glVertex3f(hx+1, hy, hz+1);
        glVertex3f(hx+1, hy+1, hz+1); glVertex3f(hx, hy+1, hz+1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(hx, hy, hz); glVertex3f(hx, hy, hz+1);
        glVertex3f(hx+1, hy, hz); glVertex3f(hx+1, hy, hz+1);
        glVertex3f(hx, hy+1, hz); glVertex3f(hx, hy+1, hz+1);
        glVertex3f(hx+1, hy+1, hz); glVertex3f(hx+1, hy+1, hz+1);
        glEnd();
        glEnable(GL_LIGHTING);
    }
    
    drawHUD();
    glutSwapBuffers();
}

void update(int v) {
    float ry = yaw * PI / 180.0f;
    float fx = -sinf(ry), fz = -cosf(ry);
    float rx = cosf(ry), rz = -sinf(ry);
    
    float mx = 0, mz = 0;
    if(keys['w']||keys['W']) { mx += fx; mz += fz; }
    if(keys['s']||keys['S']) { mx -= fx; mz -= fz; }
    if(keys['a']||keys['A']) { mx -= rx; mz -= rz; }
    if(keys['d']||keys['D']) { mx += rx; mz += rz; }
    
    float len = sqrtf(mx*mx + mz*mz);
    float spd = 0.18f;
    if(len > 0) { mx = mx/len*spd; mz = mz/len*spd; }
    
    // Separate collision checking for X and Z axes - more stable
    float nx = camX + mx;
    if(!collides(nx, camY, camZ)) camX = nx;
    
    float nz = camZ + mz;
    if(!collides(camX, camY, nz)) camZ = nz;
    
    // Improved gravity and vertical collision system
    bool wasGrounded = grounded;
    grounded = false;
    
    velY -= 0.018f;
    float ny = camY + velY;
    
    // Check if we're falling and will hit something
    if(velY < 0) {
        float checkY = camY;
        while(checkY > ny) {
            checkY -= 0.05f;
            if(collides(camX, checkY, camZ)) {
                // Found ground - place player properly on it
                int blockY = (int)floorf(checkY);
                camY = (float)(blockY + 1) + 0.8f;
                grounded = true;
                velY = 0;
                break;
            }
        }
        if(!grounded) camY = ny;
    } else if(velY > 0) {
        // Check ceiling collision
        if(collides(camX, ny, camZ)) {
            velY = 0;
        } else {
            camY = ny;
        }
    } else {
        camY = ny;
    }
    
    if(camY < 1) {
        camY = 1;
        velY = 0;
        grounded = true;
    }
    
    updateWorld();
    spawnPigs(); // Try to spawn pigs each update

    // Update pigs
    for (auto& pig : pigs) {
        if (pig.walkTimer <= 0) {
            pig.yaw = (float)(rand() % 360);
            pig.walkTimer = 60 + (rand() % 120); // Walk for 1-3 seconds
        }
        float pigRad = pig.yaw * PI / 180.0f;
        float nx = pig.x - sinf(pigRad) * pig.speed;
        float nz = pig.z - cosf(pigRad) * pig.speed;

        // Simple AI: walk forward, avoid falling
        int groundY = (int)floorf(pig.y - 0.6f);
        int nextGroundY = (int)floorf(getHeight((int)nx, (int)nz));

        if (isSolid((int)floorf(nx), groundY, (int)floorf(nz)) && abs(groundY - nextGroundY) <= 1) {
            pig.x = nx;
            pig.z = nz;
            pig.y = (float)getHeight((int)pig.x, (int)pig.z) + 0.5f;
        } else {
            pig.walkTimer = 0; // Turn around
        }
        pig.walkTimer--;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int w, int h) {
    winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)w/h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char k, int x, int y) {
    keys[k] = true;
    if(k >= '1' && k <= '7') selBlock = k - '0';
    if(k == ' ' && grounded) { velY = 0.35f; grounded = false; }
    if(k == 'e' || k == 'E') invOpen = !invOpen;
    if(k == 27) exit(0);
}

void keyboardUp(unsigned char k, int x, int y) { keys[k] = false; }

void mouse(int btn, int state, int x, int y) {
    if(state != GLUT_DOWN) return;
    float hx, hy, hz; int hf;
    
    if(btn == GLUT_LEFT_BUTTON && raycast(hx, hy, hz, hf)) {
        int bx = (int)hx, by = (int)hy, bz = (int)hz;
        int t = getBlock(bx, by, bz);
        if(t != BEDROCK) {
            inventory[t]++;
            setBlock(bx, by, bz, AIR);
        }
    } else if(btn == GLUT_RIGHT_BUTTON && raycast(hx, hy, hz, hf)) {
        int bx = (int)hx, by = (int)hy, bz = (int)hz;
        switch(hf) {
            case 0: by++; break;
            case 1: by--; break;
            case 2: bz++; break;
            case 3: bz--; break;
            case 4: bx++; break;
            case 5: bx--; break;
        }
        if(!collides(camX, camY, camZ))
            setBlock(bx, by, bz, selBlock);
    }
}

void motion(int x, int y) {
    if(firstMouse) { mouseLastX = x; mouseLastY = y; firstMouse = false; return; }
    
    float sens = 0.2f;
    yaw += (x - mouseLastX) * sens;
    pitch -= (y - mouseLastY) * sens;
    if(pitch > 89) pitch = 89;
    if(pitch < -89) pitch = -89;
    
    glutWarpPointer(winW/2, winH/2);
    mouseLastX = winW/2;
    mouseLastY = winH/2;
}

void passiveMotion(int x, int y) { motion(x, y); }

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Mini Minecraft - Enhanced Graphics");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    updateWorld();
    camY = (float)(getHeight(0, 0) + 3);
    
    inventory[DIRT] = 64;
    inventory[STONE] = 64;
    inventory[WOOD] = 32;
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(16, update, 0);
    
    printf("\n=== Mini Minecraft - Enhanced ===\n");
    printf("WASD: Move | Mouse: Look | Space: Jump\n");
    printf("Left Click: Break | Right Click: Place\n");
    printf("1-7: Select Block | E: Inventory | ESC: Exit\n");
    printf("=================================\n\n");
    
    glutMainLoop();
    return 0;
}