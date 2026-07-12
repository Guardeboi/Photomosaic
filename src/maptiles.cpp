/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas* ans = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    std::map<Point<3>, TileImage> t_avg;
    std::vector<Point<3>> avg;
    for (TileImage t : theTiles) { 
        t_avg.insert({convertToXYZ(t.getAverageColor()), t}); 
        avg.push_back(convertToXYZ(t.getAverageColor()));
    }
    KDTree tiles = KDTree(avg);
    for (int row = 0; row < theSource.getRows(); row++) {
        for (int col = 0; col < theSource.getColumns(); col++) {
            Point<3> clr = convertToXYZ(theSource.getRegionColor(row, col));
            TileImage* tile = new TileImage(t_avg[tiles.findNearestNeighbor(clr)]);
            ans->setTile(row, col, tile);
        }
    }
    return ans;
}

