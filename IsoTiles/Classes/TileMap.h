#ifndef TileMap_hpp
#define TileMap_hpp

#include "cocos2d.h"
#include "IsoTile.h"

class TileMap : public cocos2d::Layer
{
public:
    TileMap(const int width, const int height, const int tileSize);

    virtual void update(float delta) override;
    virtual void setPosition(const cocos2d::Vec2& position) override;

    bool addTile(const int tileX, const int tileY, const IsoTile::TileType type);
    std::shared_ptr<IsoTile> getTile(const int tileX, const int tileY);
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getTileSize() const { return m_tileSize; }
    
    inline const cocos2d::Vec2 screenToIso(cocos2d::Vec2 screen) const
    {
        float isoX = screen.y / m_tileSize - screen.x / (2.0f * m_tileSize);
        float isoY = screen.y / m_tileSize + screen.x / (2.0f * m_tileSize);
        return cocos2d::Vec2(isoX, isoY);
    }
    
    inline const cocos2d::Vec2 isoToScreen(cocos2d::Vec2 iso) const
    {
        float posX = (iso.y - iso.x) * m_tileSize;
        float posY = (iso.x + iso.y) * m_tileSize / 2.0f;
        return cocos2d::Vec2(posX, posY);
    }

    inline const cocos2d::Vec2 isoToScreen(cocos2d::Vec3 iso) const
    {
        float posX = (iso.y - iso.x) * m_tileSize;
        float posY = (((iso.x + iso.y) / 2.0f) - iso.z) * m_tileSize;
        return cocos2d::Vec2(posX, posY);
    }
private:
    void createGrid();
    void refreshVisibleTiles();
    void refreshVisibleTileDepths();
    
    int m_width;
    int m_height;
    int m_tileSize;
    
    std::vector<std::shared_ptr<IsoTile>> m_tiles;
    // Holds indices of IsoTiles from m_tiles, for z-order sorting
    std::vector<int> m_visibleTiles;
    // Holds indices of IsoTiles from m_tiles, for collision checking
    std::vector<std::vector<int>> m_grid;

    cocos2d::RefPtr<cocos2d::SpriteBatchNode> m_spriteBatch;
    cocos2d::RefPtr<cocos2d::DrawNode> m_node;
    
};

#endif /* TileMap_hpp */
