#ifndef TileMapUtil_h
#define TileMapUtil_h

class TileMapUtil
{
public:
    // Convert 3D space coordinates to flattened 2D isometric coordinates.
    // x and y coordinates are oblique axes separated by 120 degrees.
    // h,v are the horizontal and vertical distances from the origin.
    struct HexagonPoint
    {
        float x;
        float y;
        float h;
        float v;
        HexagonPoint(float isoX, float isoY, float isoZ)
        {
            x = isoX + isoZ;
            y = isoY + isoZ;
            h = (x - y) * cos(M_PI/6.0);
            v = (x + y) / 2.0;
        }
    };
    
    // Holds the screen-space hexagon bounds for a given isometric bounding box
    struct IsoBounds
    {
        float xmin;
        float xmax;
        float ymin;
        float ymax;
        float hmin;
        float hmax;
        
        IsoBounds(const cocos2d::Vec3 pos, const cocos2d::Vec3 size)
        {
            HexagonPoint frontDown = HexagonPoint(pos.x, pos.y, pos.z);
            HexagonPoint backUp = HexagonPoint(pos.x+size.x, pos.y+size.y, pos.z+size.z);
            HexagonPoint rightDown = HexagonPoint(pos.x+size.x, pos.y, pos.z);
            HexagonPoint leftDown = HexagonPoint(pos.x, pos.y+size.y, pos.z);
            
            xmin = frontDown.x;
            xmax = backUp.x;
            ymin = frontDown.y;
            ymax = backUp.y;
            hmin = leftDown.h;
            hmax = rightDown.h;
        }
    };
    
    // Hexagons overlap if and only if all axis regions overlap.
    static bool doHexagonsOverlap(const IsoBounds& hex1, const IsoBounds& hex2)
    {
        return (!(hex1.xmin >= hex2.xmax || hex2.xmin >= hex1.xmax) &&
                !(hex1.ymin >= hex2.ymax || hex2.ymin >= hex1.ymax) &&
                !(hex1.hmin >= hex2.hmax || hex2.hmin >= hex1.hmax));
    }
    

    
    
    // Determine if the given ranges are disjoint (i.e. do not overlap).
    // For determining the drawing order, we will consider two
    // ranges to be disjoint even if they share an endpoint.
    // Thus, we use less-or-equal (<=) instead of strictly less (<).
    static inline bool areRangesDisjoint(float amin, float amax, float bmin, float bmax)
    {
        return (amax <= bmin || bmax <= amin);
    }
    
    // Find a 3D separation axis, and use it to determine
    // which block is in front of the other.
    static bool isTileInFront(std::shared_ptr<IsoTile> tileA, std::shared_ptr<IsoTile> tileB)
    {
        cocos2d::Vec3 amin = cocos2d::Vec3(tileA->getX(), tileA->getY(), tileA->getZ());
        cocos2d::Vec3 amax = amin+cocos2d::Vec3(tileA->getSizeX(), tileA->getSizeY(), tileA->getSizeZ());
        cocos2d::Vec3 bmin = cocos2d::Vec3(tileB->getX(), tileB->getY(), tileB->getZ());
        cocos2d::Vec3 bmax = bmin+cocos2d::Vec3(tileB->getSizeX(), tileB->getSizeY(), tileB->getSizeZ());
        
        if (areRangesDisjoint(amin.x,amax.x,bmin.x,bmax.x))
        {
            return (amin.x < bmin.x);
        }
        else if (areRangesDisjoint(amin.y,amax.y,bmin.y,bmax.y))
        {
            return (amax.y < bmax.y);
        }
        else if (areRangesDisjoint(amin.z,amax.z,bmin.z,bmax.z))
        {
            return (amin.z < bmin.z);
        }
        
        return false;
    }

};

#endif /* TileMapUtil_h */
