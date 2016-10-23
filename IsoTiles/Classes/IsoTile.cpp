#include "IsoTile.h"

std::vector<int> IsoTile::TileSizesX = {
    1,1,1,1,1,1,1,1,1,1,2,2,2,3,3,2,2
};
std::vector<int> IsoTile::TileSizesY = {
    1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,3,3
};
std::vector<int> IsoTile::TileSizesZ = {
    1,1,1,1,1,1,1,1,1,5,2,1,1,2,5,3,4
};
std::vector<cocos2d::Vec2> AnchorPoints = {
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
    cocos2d::Vec2(0.5, 0.0),
 
    cocos2d::Vec2(0.5, 0.275),
    cocos2d::Vec2(0.5, 0.34),
    cocos2d::Vec2(0.5, 0.33),
    cocos2d::Vec2(0.6, 0.35),
    cocos2d::Vec2(0.6, 0.23),
    cocos2d::Vec2(0.4, 0.32),
    cocos2d::Vec2(0.41, 0.29),
};

std::vector<std::string> IsoTile::SPRITE_FILENAMES =
{
    "1x1_dirt.png",
    "1x1_dirt_oil.png",
    "1x1_grassland.png",
    "1x1_street_curve_a.png",
    "1x1_street_curve_b.png",
    "1x1_street_curve_c.png",
    "1x1_street_curve_d.png",
    "1x1_street_x.png",
    "1x1_street_y.png",
    "1x1_windmill.png",
    "2x2_factory.png",
    "2x2_firestation.png",
    "2x2_policestation.png",
    "2x3_factory_3.png",
    "2x3_house_1.png",
    "3x2_house_b.png",
    "3x2_house_d.png",
};

IsoTile::IsoTile(const int x, const int y, const int z, const TileType type)
: m_type(type)
, m_x(x)
, m_y(y)
, m_z(z)
, m_sizeX(TileSizesX[type])
, m_sizeY(TileSizesY[type])
, m_sizeZ(TileSizesZ[type])
, m_sprite(nullptr)
{
    setType(m_type);
}

void IsoTile::setType(const IsoTile::TileType type)
{
    std::string spriteName = SPRITE_FILENAMES[type];
    m_sprite = cocos2d::Sprite::createWithSpriteFrameName(spriteName);
    m_sprite->setAnchorPoint(AnchorPoints[type]);
    m_type = type;
}

const std::string& IsoTile::getName() const
{
    return SPRITE_FILENAMES[m_type];
}
