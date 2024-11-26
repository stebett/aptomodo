//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_LEVELMANAGER_H
#define APTOMODO_LEVELMANAGER_H

#include "intGrid.h"

/* This simply loads
 * - Level's background
 * - Level's entities (player, npc, items) positions
 * - Level's IntGrid
 *
 * The background is ever only accessed in the rendering loop, it can probably be passed by const ref there
 * The entities starting positions are only at the beginning of the PlayLevel functions, they can be passed by const ref
 * IntGrid is good to have it global for now, so maybe it can be a self-responsible object?
 *
 * So LevelManager can be a normal object, unless there is some issue with loading order. In that case, maybe we can just make ldtk* variables static?
 */
namespace Level {
    struct Entity {
        Vector2 position;
        std::string type;
        std::string name;
    };

    std::vector<Level::Entity> LoadEntities(const ldtk::Level &level);

    Texture2D LoadLevelTexture(const ldtk::Level &level);

    IntGrid LoadIntGrid(const ldtk::Level &level);
}


#endif //APTOMODO_LEVELMANAGER_H
