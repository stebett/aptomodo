//
// Created by ginko on 11/26/24.
//

#ifndef SPACE_H
#define SPACE_H


namespace Space {
    void Update(const Camera2D &camera);

    std::vector<Vector2> tilesOccupied(Vector2 center, float radius);

}


#endif //SPACE_H
