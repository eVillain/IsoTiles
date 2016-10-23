#ifndef IsoTile_h
#define IsoTile_h

#include "cocos2d.h"

class IsoTile
{
public:
    enum TileType
    {
        dirt,
        dirt_oil,
        grassland,
        street_curve_a,
        street_curve_b,
        street_curve_c,
        street_curve_d,
        street_x,
        street_y,
        windmill,
        factory,
        firestation,
        policestation,
        factory_3,
        house_1,
        house_b,
        house_d,
    };
    static std::vector<int> TileSizesX;
    static std::vector<int> TileSizesY;
    static std::vector<int> TileSizesZ;

    IsoTile(const int x, const int y, const int z, const TileType type);
    
    const TileType getType() const { return m_type; }
    
    // Isometric coordinates
    const int getX() const { return m_x; }
    const int getY() const { return m_y; }
    const int getZ() const { return m_z; }
    // Size in isometric tiles
    const int getSizeX() const { return m_sizeX; }
    const int getSizeY() const { return m_sizeY; }
    const int getSizeZ() const { return m_sizeZ; }
    
    cocos2d::RefPtr<cocos2d::Sprite> getSprite() { return m_sprite; }

    const std::string& getName() const;
private:
    void setType(const TileType);
    
    TileType m_type;
    
    int m_x;
    int m_y;
    int m_z;
    
    int m_sizeX;
    int m_sizeY;
    int m_sizeZ;
    
    cocos2d::RefPtr<cocos2d::Sprite> m_sprite;
    
    static std::vector<std::string> SPRITE_FILENAMES;
};

#endif /* IsoTile_h */
