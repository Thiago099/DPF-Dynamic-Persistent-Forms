

#pragma once

static bool DoLog = false;


class FileLogger {
    private:
    static std::string fileName;
    static FileLogger* instance; 
    std::ofstream stream; 
    FileLogger() {
        if (!fileName.empty()) {
            stream.open(fileName, std::ios::app);
            if (!stream.is_open()) {
                stream.open(fileName);
                if (!stream.is_open()) {
                    return;
                }
            }
        }
    }
    public:
    static FileLogger* GetSingleton() {
        if (instance == nullptr) {
            instance = new FileLogger(); 
        }
        return instance;
    }
    static void SetFileName(std::string name) {
        fileName = name;
    }
    void Print(std::string message) {
        if (stream.is_open()) {
            stream << message << std::endl;
        }
    }

};

FileLogger* FileLogger::instance = nullptr;
std::string FileLogger::fileName = "";

void EnableLog(std::string filename, std::string message) {
    FileLogger::SetFileName(filename);
    FileLogger::GetSingleton()->Print(message);
    DoLog = true;
}

void print(const char* message) {
    if (DoLog) {
        FileLogger::GetSingleton()->Print(message);
        RE::ConsoleLog::GetSingleton()->Print(message);
    }
}
void print(const char* message, const float number) {
    if (DoLog) {
        auto msg = std::format("{0}: {1}", message, number).c_str();
        FileLogger::GetSingleton()->Print(msg);
        RE::ConsoleLog::GetSingleton()->Print(msg);
    }
}
void printSize(const char* message, const size_t number) {
    if (DoLog) {
        auto msg = std::format("{0}: {1}", message, number).c_str();
        FileLogger::GetSingleton()->Print(msg);
        RE::ConsoleLog::GetSingleton()->Print(msg);
    }
}
void printInt(const char* message, const int32_t number) {
    if (DoLog) {
        auto msg = std::format("{0}: {1}", message, number).c_str();
        FileLogger::GetSingleton()->Print(msg);
        RE::ConsoleLog::GetSingleton()->Print(msg);
    }
}