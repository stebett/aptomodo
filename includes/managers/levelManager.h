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
class LevelManager {
    static const char *root;
    static LevelManager *instance;

    static int current_level;

    static ldtk::Project *ldtkProject;
    static const ldtk::World *ldtkWorld;
    static const ldtk::Level *currentLdtkLevel;

    static Texture2D currentTilesetTexture;

    static std::vector<std::pair<std::string, Vector2> > entitiesPositions;
    static std::vector<std::pair<std::string, Vector2> > enemiesPositions;
    static std::hash<std::string> hasher; // todo remove

public:
    static IntGrid grid; // todo does this need to be here?
    static Texture2D renderedLevelTexture;
    // todo also this, maybe it can be just an unaccessible variable in the rendering manager context?


    ~LevelManager();

    static std::vector<std::pair<std::string, Vector2> > &GetEntitiesPositions();

    static std::vector<std::pair<std::string, Vector2> > &GetEnemiesPositions();

    // todo name these starting positions, and also they're probably needed only once so maybe you can add a function to delete them right after to save space?
    static void Update(const entt::registry &registry); // todo remove?

    static void SetLevel(int level);

    static LevelManager &Instance();

    // todo remove, useless since all functions are static? Or should they be not static?
    // anyway set level is called in the main loop, and also the starting positions are needed only there

    static void Instantiate();
};


#endif //APTOMODO_LEVELMANAGER_H
