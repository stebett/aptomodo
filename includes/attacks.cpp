//
// Created by ginko on 11/29/24.
//

#include "attacks.h"
#include "managers/game.h"

#include <timer.h>
#include <systems/physics.h>

auto createShapeHelper = [](b2BodyId bodyId, b2Polygon box) {
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;
    shapeDef.filter.maskBits = Physics::Enemy;
    return b2CreatePolygonShape(bodyId, &shapeDef, &box);
};

namespace Attacks {
    void Update() {
        const auto now = GetTime();
        Game::registry.view<Attacks::Attack>().each([now](auto entity, const auto attack) {
            auto timer{Game::registry.get<PassiveTimer>(entity)};
            const auto t = (now - timer.start) / timer.duration;
            auto bodyCouple{Game::registry.get<BodyCouple>(entity)};
            const auto transformer = Game::registry.get<Transform>(entity);
            auto oldTransform{transformer.get(t)};
            auto ownerTransform{b2Body_GetTransform(bodyCouple.owner)};
            auto newTransform = b2MulTransforms(ownerTransform, oldTransform);
            b2Body_SetTransform(bodyCouple.weapon, newTransform.p, newTransform.q);

            b2ShapeId shape;
            b2Body_GetShapes(bodyCouple.weapon, &shape, 1);
            b2DestroyShape(shape, false);
            createShapeHelper(bodyCouple.weapon, b2MakeBox(transformer.getDim1(t), transformer.getDim2(t)));

            if (now - timer.start > timer.duration) {
                Physics::DestroyBody(bodyCouple.weapon);;
                Game::registry.destroy(entity);
            }
        });
    }

    Attack::Attack(const float damage) : damage(damage) {
    }

    b2Rot Transform::startAngle() const { return b2Rot(cos(startRadians), sin(startRadians)); }

    b2Rot Transform::endAngle() const { return b2Rot(cos(endRadians), sin(endRadians)); }

    Transform::Transform(Spline trajectory) : trajectory(trajectory) {
    }

    b2Transform Transform::get(const float t) const {
        const auto eased_t = easingSpeed.valueAt(t);
        const Math::Vec2 p = trajectory.bezier.valueAt(eased_t);
        const Math::Vec2 norm = trajectory.bezier.normalAt(eased_t);
        if (isnan(norm.x) || isnan(norm.y)) {
            return {p, startAngle()};
        }
        const auto radians = atan2(norm.y, norm.x);
        const auto eased_angle_t = easingAngle.valueAt(t);

        const auto q = b2MulRot(b2Rot(cos(radians), sin(radians)),
                                b2NLerp(startAngle(), endAngle(), eased_angle_t));
        return {p, q};
    }

    float Transform::getDim1(const float t) const {
        return Lerp(startDim1, endDim1, easingDim1.valueAt(t));
    }

    float Transform::getDim2(const float t) const {
        return Lerp(startDim2, endDim2, easingDim2.valueAt(t));
    }

    // Save the Transform to a TOML file
    void Transform::saveToTOML() const {
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(ATTACK_PATH) / name;
        path.replace_extension(".toml");

        toml::table tbl;

        // Save spline points as a toml::array
        toml::array splinePoints;
        for (const auto &point: trajectory.get()) {
            toml::array pointArray;
            pointArray.push_back(point.x);
            pointArray.push_back(point.y);
            splinePoints.push_back(std::move(pointArray));
        }
        tbl.emplace("spline", std::move(splinePoints));

        // Save easing splines as toml::arrays
        auto saveEasingSpline = [&tbl](const std::string &key, const EasingSpline &easing) {
            toml::array easingArray;
            for (const auto &value: easing.get()) {
                easingArray.push_back(value);
            }
            tbl.emplace(key, std::move(easingArray));
        };

        saveEasingSpline("easingSpeed", easingSpeed);
        saveEasingSpline("easingAngle", easingAngle);
        saveEasingSpline("easingDim1", easingDim1);
        saveEasingSpline("easingDim2", easingDim2);

        // Save other parameters
        tbl.emplace("startRadians", startRadians);
        tbl.emplace("endRadians", endRadians);
        tbl.emplace("startDim1", startDim1);
        tbl.emplace("startDim2", startDim2);
        tbl.emplace("endDim1", endDim1);
        tbl.emplace("endDim2", endDim2);

        // Write to file
        std::ofstream outFile(path);
        if (!outFile) {
            throw std::runtime_error("Could not open file for writing: " + path.string());
        }
        outFile << tbl;
    }

    // Load the Transform from a TOML file
    void Transform::loadFromTOML(const std::string &name) {
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(ATTACK_PATH) / name;
        path.replace_extension(".toml");
        std::ifstream inFile(path);
        if (!inFile) {
            throw std::runtime_error("Could not open file: " + path.string());
        }

        toml::table tbl = toml::parse(inFile);

        // Load spline points
        auto splinePoints = tbl["spline"].as_array();
//        auto localSpline = LocalSpline(splinePoints);
        size_t i = 0;
        for (const auto &point: *splinePoints) {
            if (i >= 4) break;
            if (auto pointArray = point.as_array(); pointArray && pointArray->size() == 2) {
                trajectory.bezier[i].x = (*pointArray)[0].value_or(0.0f);
                trajectory.bezier[i].y = (*pointArray)[1].value_or(0.0f);
            }
            ++i;
        }


        // Load easing splines
        auto loadEasingSpline = [&tbl](const std::string &key, EasingSpline &easing) {
            if (auto easingArray = tbl[key].as_array(); easingArray && easingArray->size() == 4) {
                auto points{easing.get()};
                for (size_t i = 0; i < 4; ++i) {
                    points[i] = (*easingArray)[i].value_or(0.0f);
                }
                easing.update(points);
            }
        };

        loadEasingSpline("easingSpeed", easingSpeed);
        loadEasingSpline("easingAngle", easingAngle);
        loadEasingSpline("easingDim1", easingDim1);
        loadEasingSpline("easingDim2", easingDim2);

        // Load other parameters
        startRadians = tbl["startRadians"].value_or(0.0f);
        endRadians = tbl["endRadians"].value_or(0.0f);
        startDim1 = tbl["startDim1"].value_or(5.0f);
        startDim2 = tbl["startDim2"].value_or(5.0f);
        endDim1 = tbl["endDim1"].value_or(5.0f);
        endDim2 = tbl["endDim2"].value_or(5.0f);
    }
}
