//
// Created by ginko on 12/2/24.

//Animation system
//in editor drag and drop of animations
//allows to define rectangles
//it saves them in resources with the png + a toml file that describes starts and ends of bounding boxes, timings etc.
//Attach them to a state of an entity


#ifndef APTOMODO_ANIMATIONEDITOR_H
#define APTOMODO_ANIMATIONEDITOR_H

#include "managers/game.h"

struct Animation {
    Texture2D texture;
    Rectangle dest;
    std::vector<Rectangle> sources;
    int frameCount;
    std::string name{"default"};
    std::filesystem::path directoryPath;
    std::filesystem::path directorySource;

    void saveToDirectory() {
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(ATTACK_PATH) / name / "metadata.toml";
        SPDLOG_INFO("Creating directory");
        SPDLOG_INFO(directoryPath);
        std::filesystem::create_directory(directoryPath);
        std::filesystem::copy(directorySource, directoryPath, std::filesystem::copy_options::recursive);
        toml::table tbl;

        // Save spline points as a toml::array
        toml::array recDest{dest.x, dest.y, dest.width, dest.height};
        tbl.emplace("dest", std::move(recDest));

        toml::array recSource;
        for (const auto &rec: sources) {
            toml::array source{rec.x, rec.y, rec.width, rec.height};
            recSource.push_back(std::move(source));
        }
        tbl.emplace("sources", std::move(recSource));

        tbl.emplace("directoryPath", directoryPath.string());
        tbl.emplace("directorySource", directorySource.string());
        tbl.emplace("frameCount", frameCount);
        tbl.emplace("name", name);

        // Write to file
        std::ofstream outFile(path);
        if (!outFile) {
            throw std::runtime_error("Could not open file for writing: " + path.string());
        }
        outFile << tbl;

    };

    void loadMetadata(std::string_view name_) {
        SPDLOG_INFO("Loading metadata");
        SPDLOG_INFO(name);
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(ATTACK_PATH) / name_ / "metadata.toml";
        std::ifstream inFile(path);
        if (!inFile) {
            throw std::runtime_error("Could not open file: " + path.string());
        }
        toml::table tbl = toml::parse(inFile);

        frameCount = tbl["frameCount"].value_or(0);
        name = tbl["name"].value_or("undefined");
        directoryPath = tbl["directoryPath"].value_or("undefined");
        directorySource = tbl["directorySource"].value_or("undefined");


        auto dest_ = tbl["dest"].as_array();
        dest = Rectangle{
                dest_[0].value_or(0.0f),
                dest_[1].value_or(0.0f),
                dest_[2].value_or(0.0f),
                dest_[3].value_or(0.0f),
        };

        auto sources_ = tbl["sources"].as_array();
        for (size_t i {0}; i < frameCount; i+=4) {
            sources.emplace_back(Rectangle{
                    sources_[i+0].value_or(0.0f),
                    sources_[i+1].value_or(0.0f),
                    sources_[i+2].value_or(0.0f),
                    sources_[i+3].value_or(0.0f),
            });
        }
    };

    void loadTexture() {
        SPDLOG_INFO("Loading texture");
        SPDLOG_INFO(name);
        std::vector<Texture2D> textures; // TODO these are being leaked

        for (const auto &path: std::filesystem::directory_iterator(
                std::filesystem::path(ROOT_PATH) / std::filesystem::path(ANIMATION_PATH) / name)) {
            if (path.path().extension() != ".png") continue;
            auto t = LoadTexture(path.path().c_str());
            textures.emplace_back(t);
        }
        RenderTexture2D target = LoadRenderTexture(dest.width, dest.height);
        BeginTextureMode(target);
        for (size_t i{0}; i < textures.size(); i++) {
            DrawTexturePro(textures[i], sources[i], dest, {0, 0}, 0, WHITE);
        }
        EndTextureMode();
        texture = target.texture;
    }
};

LevelOutcome AnimationEditorLevel(const Camera2D &camera);

LevelOutcome PlayAnimationEditorLevelOnce();

LevelOutcome PlayAnimationEditorLevel();


#endif //APTOMODO_ANIMATIONEDITOR_H
