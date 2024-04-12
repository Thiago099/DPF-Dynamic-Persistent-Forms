#include <fstream>
#include <iostream>
#include <string>
#include <iostream>
#include <regex>
#include <windows.h>
#include "src/form_record.h"  
#include "src/log.cpp"  
#include "src/model.cpp"  
#include "src/persistence.cpp"
#include "src/papyrus.cpp"
#include <spdlog/sinks/basic_file_sink.h>

//void SetupLog() {
//    auto logsFolder = SKSE::log::log_directory();
//    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
//    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
//    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
//    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
//    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
//    spdlog::set_default_logger(std::move(loggerPtr));
//    spdlog::set_level(spdlog::level::trace);
//    spdlog::flush_on(spdlog::level::trace);
//}



SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    //EnableLog("DynamicPersistentFormsLog.txt", "DPF 2.2");

    SKSE::GetPapyrusInterface()->Register(PapyrusFunctions);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            LoadCache();
            print("loaded");
        } else if (message->type == SKSE::MessagingInterface::kNewGame) {
            std::filesystem::remove("DynamicPersistentFormsCache.bin");
            while (formData.size() > 0) {
                if (formData.back() && formData.back()->reference == false) {
                    if (formData.back()->actualForm)
                    {
                        formData.back()->actualForm->SetDelete(true);
                    }
                }
                delete formData.back();
                formData.pop_back();
            }
            print("new game");
        }
    });

    auto serialization = SKSE::GetSerializationInterface();
    serialization->SetUniqueID('DPF1');
    serialization->SetSaveCallback(SaveCallback);
    serialization->SetLoadCallback(LoadCallback);

    return true;
}

