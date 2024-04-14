#pragma once

#include <cstdint>

class StreamWrapper {
    private:
    std::stringstream stream;
    public:
        void Clear() {
            stream.str("");
            stream.clear();
            stream.seekg(0);
        }
        void SeekBeginning() {
            stream.seekg(0);
        }
        template <class T>
        void Write(T value) {
            stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
        }
        template <class T>
        T Read() {
            T result;
            if (stream.read(reinterpret_cast<char*>(&result), sizeof(T))) {
                return result;
            }
            return T();
        }
        uint32_t Size() {
            std::streampos currentPosition = stream.tellg();
            stream.seekg(0, std::ios::end);
            size_t size = stream.tellg();
            stream.seekg(currentPosition);
            return static_cast<uint32_t>(size);
        }

        void WriteDown(std::function<void(uint32_t)> const& start, std::function<void(char)> const& step) {
            auto size = Size();
            start(size);
            SeekBeginning();
            for (size_t i = 0; i < size; i++) {
                step(static_cast<char>(stream.get()));
            }
            Clear();
        }
        void ReadOut(std::function<uint32_t()> start, std::function<char()> const& step) {
            Clear();
            uint32_t arrayLength = start();
            for (size_t i = 0; i < arrayLength; i++) {
                stream.put(step());
            }
            SeekBeginning();
        }
};

template <typename Derived>
class Serializer {
private:
    std::stack<StreamWrapper*> ctx;

    template <class T>
    void WriteTarget(T value) {
        static_cast<Derived*>(this)->template WriteImplementation<T>(value);
    }
    template <class T>
    T ReadSource() {
        return static_cast<Derived*>(this)->template ReadImplementation<T>();
    }

protected:
    bool error = false;
public:

    ~Serializer() {
        while (!ctx.empty()) {
            delete ctx.top();
            ctx.pop();      
        }
    }

    template <class T>
    void Write(T value) {
        if (!ctx.empty()) {
            ctx.top()->Write<T>(value);
        } else {
            WriteTarget<T>(value);
        }
    }
    template <class T>
    T Read() {
        if (!ctx.empty()) {
            return ctx.top()->Read<T>();
        } else {
            return ReadSource<T>();
        }
    }

    void StartWritingSection() {
        ctx.push(new StreamWrapper());
    }

    void finishWritingSection() {
        if (ctx.size() == 1) {
            auto body = ctx.top();
            ctx.pop();
            body->WriteDown(
                [&](uint32_t size) { WriteTarget<uint32_t>(size); }, 
                [&](char item) { WriteTarget<char>(item); }
            );
            delete body;
        } else if (ctx.size() > 1) {
            auto body = ctx.top();
            ctx.pop();
            body->WriteDown(
                [&](uint32_t size) { ctx.top()->Write<uint32_t>(size); }, 
                [&](char item) { ctx.top()->Write<char>(item); }
            );
            delete body;
        }
    }

    void startReadingSection() {

        if (ctx.size() == 0) {
            ctx.push(new StreamWrapper());
            ctx.top()->ReadOut(
                [&]() { return ReadSource<uint32_t>(); }, 
                [&]() { return ReadSource<char>(); }
            );
        } else {
            auto parent = ctx.top();
            ctx.push(new StreamWrapper());
            ctx.top()->ReadOut(
                [&]() { return parent->Read<uint32_t>(); }, 
                [&]() { return parent->Read<char>(); }
            );
        }
    }

    void finishReadingSection() {
        if (ctx.size() > 0) {
            auto top = ctx.top();
            ctx.pop();
            delete top;
        }
    }
    template<class T>
    T* ReadFormRef() {
        auto item = Read<uint32_t>();
        if (item == 0) {
            return nullptr;
        }
        auto form = RE::TESForm::LookupByID(item);
        if (!form) {
            return nullptr;
        }
        return form->As<T>();
    }

    RE::TESForm* ReadFormRef() {
        auto item = Read<uint32_t>();
        if (item == 0) {
            return nullptr;
        }
        return RE::TESForm::LookupByID(item);
    }

    void WriteFormRef(RE::TESForm* item) {
        if (!item) {
            Write<uint32_t>(0);
        } else {
            Write<uint32_t>(item->GetFormID());
        }
    }

    char* ReadString() {
        size_t arrayLength = Read<uint32_t>();
        char* result = new char[arrayLength+1];
        for (size_t i = 0; i < arrayLength; i++) {
            result[i] = Read<char>();
        }
        result[arrayLength] = '\0';
        return result;
    }

    void WriteString(const char* string) {
        size_t arrayLength = strlen(string);
        Write<uint32_t>(static_cast<uint32_t>(arrayLength));
        for (size_t i = 0; i < arrayLength; i++) {
            char item = string[i];
            Write<char>(item);
        }
    }
};

class SaveDataSerializer : public Serializer<SaveDataSerializer> {
private:
    SKSE::SerializationInterface* a_intfc;
public:
    SaveDataSerializer(SKSE::SerializationInterface* _a_intfc) { a_intfc = _a_intfc; }

    template <class T>
    void WriteImplementation(T item) {
        a_intfc->WriteRecordData(item);
    }
    template <class T>
    T ReadImplementation() {
        T item;
        auto success = a_intfc->ReadRecordData(item);
        if (!success) {
            print("error reanding");
            error = true;
        }
        return item;
    }
};

class FileWriter : public Serializer<FileWriter> {
private:
    std::ofstream fileStream;

public:
    FileWriter(const std::string& filename, std::ios_base::openmode _Mode = std::ios_base::out) {
        fileStream.open(filename, _Mode);
        if (!fileStream.is_open()) {
            print("Error: Unable to open file ");
        }
    }
    ~FileWriter() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    bool IsOpen() { return fileStream.is_open(); }

    template <class T>
    T ReadImplementation() {
        return T();
    }

    template <class T>
    void WriteImplementation(T value) {
        if (fileStream.is_open()) {
            fileStream.write(reinterpret_cast<const char*>(&value), sizeof(T));
        } else {
            print("Error: File not open for writing.");
        }
    }
};

class FileReader : public Serializer<FileReader> {
private:
    std::ifstream fileStream;

public:
    FileReader(const std::string& filename, std::ios_base::openmode _Mode = std::ios_base::in) {
        fileStream.open(filename, _Mode);
        if (!fileStream.is_open()) {
            print("Error: Unable to open file");
        }
    }
    ~FileReader() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    bool IsOpen() { return fileStream.is_open(); }

    template <class T>
    void WriteImplementation(T) {}
    template <class T>
    T ReadImplementation() {
        T value;
        if (fileStream.is_open()) {
            fileStream.read(reinterpret_cast<char*>(&value), sizeof(T));
            return value;
        } else {
            print("Error: File not open for reading.");
        }
        return T();
    }
};