//
// Created by ginko on 12/6/24.
//

#include "audioAssets.h"
#include "rapidcsv.h"

namespace Audio {

    std::vector<std::string> loadCSV() {
        try {
            auto path{std::filesystem::path(ROOT_PATH) / std::filesystem::path(AUDIO_PATH) / "tracks.csv"};
            std::vector<std::string> names{};
            SPDLOG_INFO("[AUDIO] file trying to parse: {}");
            SPDLOG_INFO(path);
            rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));
            SPDLOG_INFO("[AUDIO]: Reading file");
            auto rows{doc.GetRowCount()};
            names.reserve(rows);
            for (size_t i = 0; i < rows; ++i) {
                names.emplace_back(doc.GetCell<std::string>("Name", i));
            }

            return names;
        } catch (const std::exception &e) {
            SPDLOG_ERROR("[ENEMIES]: Error loading enemy types: {}", e.what());
            SPDLOG_ERROR(e.what());
            return {};
        }
    }
}