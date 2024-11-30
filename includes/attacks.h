//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H


namespace Attacks {
    struct Hit {
    };
    void Update(entt::registry &registry);

    class Attack {
    public:
        float damage = 0;

        explicit Attack(const float damage);;
    };
}


#endif //ATTACKS_H
