//
// Created by ginko on 12/6/24.
//

#include "audioAssets.h"
#include "rapidcsv.h"

namespace Audio {
    const auto path{std::filesystem::path(ROOT_PATH) / std::filesystem::path(AUDIO_PATH) / "tracks.csv"};

    std::vector<std::string> loadCSV() {
        try {
            std::vector<std::string> names{};
            SPDLOG_INFO("[AUDIO] file trying to parse: {}");
            SPDLOG_INFO(path);
            rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));
            SPDLOG_INFO("[AUDIO]: Reading file");
            auto rows{doc.GetRowCount()};
            names.reserve(rows);
            for (size_t i = 0; i < rows; ++i) {
                auto name {doc.GetCell<std::string>("Name", i)};
                names.push_back(name);
            }

            return names;
        } catch (const std::exception &e) {
            SPDLOG_ERROR("[AUDIO]: Error loading enemy types: {}", e.what());
            SPDLOG_ERROR(e.what());
            return {};
        }
    }

    bool saveCSV(const std::vector<std::string> &filenames) {
        try {
            std::vector<std::string> names;
            names.reserve(filenames.size());
            for (const auto &name: filenames) {
                names.emplace_back(name);
            }

            rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));
            doc.SetColumn<std::string>("Name", names);

            doc.Save(path);

            SPDLOG_INFO("[AUDIO]: File saved successfully to {}", path.string());
            SPDLOG_INFO(path.string());
            return true;
        } catch (const std::exception &e) {
            SPDLOG_ERROR("[AUDIO]: Failed to save file: {}", e.what());
            SPDLOG_ERROR(e.what());
            return false;
        }
    }

}