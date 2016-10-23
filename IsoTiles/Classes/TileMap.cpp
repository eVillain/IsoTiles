#include "TileMap.h"
#include "IsoTile.h"
#include "TileMapUtil.h"

constexpr int TILEMAP_INITIAL_TILE_Z = 10;
constexpr int TILEMAP_SPRITES_Z = 1;
constexpr int TILEMAP_GRID_Z = 0;
constexpr float SPRITE_SCALING = 1.0f;

TileMap::TileMap(const int width, const int height, const int tileSize)
: m_width(width)
, m_height(height)
, m_tileSize(tileSize)
, m_grid(width, std::vector<int>(height, -1))
{
    createGrid();
    m_grid.resize(m_width*m_height);
    
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("res/Tiles2.plist");
    const cocos2d::Vec2 screen = cocos2d::Director::getInstance()->getVisibleSize();

    m_spriteBatch = cocos2d::SpriteBatchNode::create("res/Tiles2.png");
    m_spriteBatch->getTexture()->setAliasTexParameters();
    m_spriteBatch->setScale(SPRITE_SCALING);
    m_spriteBatch->setPosition(cocos2d::Vec2(0, screen.y));
    addChild(m_spriteBatch, TILEMAP_SPRITES_Z);

    bool generateRandom = true;
    if (generateRandom)
    {
        for (int x=0; x < m_width; x++)
        {
            for (int y=0; y < m_height; y++)
            {
                bool flatTile = cocos2d::random(0, 100) < 90;
                IsoTile::TileType tileType = flatTile ? cocos2d::random(IsoTile::dirt, IsoTile::windmill) : cocos2d::random(IsoTile::factory, IsoTile::house_d);
                //            IsoTile::TileType tileType = cocos2d::random(IsoTile::dirt, IsoTile::house_d);
                if (addTile(x, y, tileType))
                {
                    y += IsoTile::TileSizesY[tileType]-1;
                }
                else
                {
                    addTile(x, y, IsoTile::grassland);
                }
            }
        }
    }
    else
    {
        addTile(0, 0, IsoTile::grassland);
        addTile(1, 0, IsoTile::grassland);
        addTile(2, 0, IsoTile::grassland);
        addTile(3, 0, IsoTile::factory);
        addTile(0, 1, IsoTile::windmill);
        addTile(1, 1, IsoTile::windmill);
        addTile(2, 1, IsoTile::windmill);
        addTile(0, 2, IsoTile::windmill);
        addTile(1, 2, IsoTile::factory_3);
        addTile(4, 2, IsoTile::windmill);
        addTile(0, 3, IsoTile::windmill);
        addTile(4, 3, IsoTile::firestation);
        addTile(0, 4, IsoTile::grassland);
        addTile(1, 4, IsoTile::windmill);
        addTile(2, 4, IsoTile::windmill);
        addTile(3, 4, IsoTile::windmill);
        addTile(0, 5, IsoTile::grassland);
        addTile(1, 5, IsoTile::grassland);
        addTile(2, 5, IsoTile::house_b);
        addTile(4, 5, IsoTile::windmill);
        addTile(0, 6, IsoTile::windmill);
        addTile(1, 6, IsoTile::windmill);
        addTile(4, 6, IsoTile::windmill);
        addTile(5, 6, IsoTile::windmill);
    }
    scheduleUpdate();
}

void TileMap::update(float delta)
{
//    time_t startTime = time(0); // now
//    struct std::tm startTimeTM = *localtime(&startTime);

    refreshVisibleTileDepths();
    
//    time_t endTime = time(0); // now
//    struct std::tm endTimeTM = *localtime(&endTime);
//    double elapsedTime = difftime(mktime(&endTimeTM), mktime(&startTimeTM));
//    cocos2d::log("tile positioning for %i tiles, runtime: %f, tiles per second: %f", m_width*m_height, elapsedTime, (m_width*m_height)/elapsedTime);
}

void TileMap::setPosition(const cocos2d::Vec2& position)
{
    cocos2d::Layer::setPosition(position);
    
    refreshVisibleTiles();
}

bool TileMap::addTile(const int tileX, const int tileY, const IsoTile::TileType type)
{
    // Check for occupied tiles under the tile we are trying to add
    int tileSizeX = IsoTile::TileSizesX[type];
    int tileSizeY = IsoTile::TileSizesY[type];
    int xmin = tileX;
    int xmax = tileX+tileSizeX-1;
    int ymin = tileY;
    int ymax = tileY+tileSizeY-1;
    for (int x = xmin; x <= xmax; x++)
    {
        for (int y = ymin; y <= ymax; y++)
        {
            if (x < m_width &&
                y < m_height &&
                m_grid[x][y] > -1)
            {
                return false;   // One of the tiles was occupied, unable to add
            }
        }
    }

    auto tile = std::make_shared<IsoTile>(tileX, tileY, 0, type);
    m_spriteBatch->addChild(tile->getSprite(), TILEMAP_INITIAL_TILE_Z);
    cocos2d::Vec2 screenPos = isoToScreen(cocos2d::Vec3(tile->getX()+1, tile->getY()+1, tile->getZ()));
    screenPos.y *= -1;
    tile->getSprite()->setPosition(screenPos);

    m_tiles.push_back(tile);
    for (int x = xmin; x <= xmax; x++)
    {
        for (int y = ymin; y <= ymax; y++)
        {
            if (x < m_width &&
                y < m_height)
            {
                m_grid[x][y] = m_tiles.size()-1;
            }
        }
    }
    return true;
}

std::shared_ptr<IsoTile> TileMap::getTile(const int tileX, const int tileY)
{
    if (tileX < m_width && tileY < m_height &&
        tileX >= 0 && tileY >= 0)
    {
        return m_tiles[m_grid[tileX][tileY]];
    }
    return nullptr;
}

// Draw a bunch of isometric grid lines into the drawnode
void TileMap::createGrid()
{
    m_node = cocos2d::DrawNode::create();
    addChild(m_node, TILEMAP_GRID_Z);
    
    const cocos2d::Vec2 screen = cocos2d::Director::getInstance()->getVisibleSize();
    const int fullWidth = m_width*m_tileSize*2.0;
    const int fullHeight = m_width*m_tileSize;
    const float lineThickness = 0.25f;

    for (int x = 0; x < m_width+1; x++)
    {
        cocos2d::Vec2 top = cocos2d::Vec2(x*m_tileSize-(fullWidth*0.5), screen.y-(fullHeight+(x*m_tileSize))*0.5);
        cocos2d::Vec2 bottom = cocos2d::Vec2(x*m_tileSize, screen.y-(x*m_tileSize*0.5));
        m_node->drawSegment(top, bottom, lineThickness, cocos2d::Color4F(180,180,180,100));
        
        cocos2d::Vec2 left = cocos2d::Vec2(x*m_tileSize-(fullWidth*0.5), screen.y-(fullHeight-(x*m_tileSize))*0.5);
        cocos2d::Vec2 right = cocos2d::Vec2(x*m_tileSize, screen.y-(fullHeight-(x*m_tileSize*0.5)));
        m_node->drawSegment(left, right, lineThickness, cocos2d::Color4F(180,180,180,100));
    }
}

void TileMap::refreshVisibleTiles()
{
    const cocos2d::Size screen = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 screenPos = cocos2d::Vec2(-getPosition().x, getPosition().y);
    cocos2d::Vec2 isoMin = screenToIso(screenPos);
    cocos2d::Vec2 isoMax = screenToIso(screenPos+screen);

    int minX = isoMin.y-isoMin.x;
    int minY = isoMin.y+isoMin.x;
    int maxX = isoMax.y-isoMax.x;
    int maxY = isoMax.y+isoMax.x;

    m_visibleTiles.clear();
    for (int index = 0; index < m_tiles.size(); index++)
    {
        auto tile = m_tiles[index];
        int v = tile->getY()+tile->getX();  // Vertical row
        int h = tile->getY()-tile->getX();  // Horizontal row
        if (h > maxX || h < minX || v > maxY || v < minY)
        {
            continue;
        }
        m_visibleTiles.push_back(index);
    }
}

void TileMap::refreshVisibleTileDepths()
{
    // Sort - for every visible tile A check against every tile B in the sorted
    // list, adding tile A to the sorted list after the last tile B which occludes
    // tile A. We consider this the closest position to the camera where tile A
    // can be drawn relative to the other sorted tiles.
    std::vector<int> sortedIndices; // Holds indices of IsoTiles from m_tiles
    for (int index = 0; index < m_visibleTiles.size(); index++)
    {
        const int visibleTileIndex = m_visibleTiles[index];
        auto tile = m_tiles[visibleTileIndex];
        auto boundsA = TileMapUtil::IsoBounds(cocos2d::Vec3(tile->getX(), tile->getY(), tile->getZ()),
                                             cocos2d::Vec3(tile->getSizeX(), tile->getSizeY(), tile->getSizeZ()));
        int lastObjectInFront = 0;
        for (int sortedIndex = 0; sortedIndex < sortedIndices.size(); sortedIndex++)
        {
            int sortedTileIndex = sortedIndices[sortedIndex];
            auto sortedTile = m_tiles[sortedTileIndex];
            auto boundsB = TileMapUtil::IsoBounds(cocos2d::Vec3(sortedTile->getX(), sortedTile->getY(), sortedTile->getZ()),
                                                  cocos2d::Vec3(sortedTile->getSizeX(), sortedTile->getSizeY(), sortedTile->getSizeZ()));
            // Check if tile bounds overlap, then check if tile A is behind
            // sorted tile B in which case we should consider inserting tile A
            // behind tile B in the sorted list (ie. draw tile A behind tile B)
            if (TileMapUtil::doHexagonsOverlap(boundsA, boundsB) &&
                !TileMapUtil::isTileInFront(tile, sortedTile))
            {
                lastObjectInFront = sortedIndex+1;
            }
        }
        sortedIndices.emplace(sortedIndices.begin()+lastObjectInFront, visibleTileIndex);
    }
    
    // Set z-position for each sprite according to sorting
    for (int i = 0; i < sortedIndices.size(); i++)
    {
        auto tile = m_tiles[sortedIndices[i]];
        float z = float(i*2)/sortedIndices.size();
        tile->getSprite()->setPositionZ(z);
    }
}
