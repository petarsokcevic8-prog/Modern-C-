#include <iostream>
#include <cstdio>
#include <string>
#include <stdexcept>

// Globalna varijabla za unos imena datoteke.
// Koristi se kako bi se ime moglo unijeti u runtime-u, umjesto hardcodanja.
std::string odabir;

class FileWrapper
{
private:
    FILE* file; 
    // Raw C file pointer koji predstavlja otvoreni file handle.
    // Držimo ga privatnim da spriječimo direktnu manipulaciju (encapsulation).

public:
    FileWrapper(const char* filename, const char* mode)
    {
        // Otvaranje file-a u zadanom modu ("r", "w", itd.)
        file = fopen(filename, mode);

        // Ako file nije uspješno otvoren, prekidamo program kroz exception.
        // Ovo sprječava rad s nevalidnim resursom.
        if (file == nullptr)
        {
            throw std::runtime_error("Failed to open the file or doesnt exist!");
        }
    }

    ~FileWrapper()
    {
        // Destructor implementira RAII princip.
        // Automatski zatvara file kada objekt izađe iz scope-a.
        // Time se sprječavaju resource leakovi.
        if (file != nullptr)
        {
            fclose(file);
        }
    }

    std::string readAll()
    {
        // Buffer služi kao privremeno spremište za učitavanje linija iz file-a.
        char buffer[256];

        std::string content;

        // fgets čita file line-by-line dok god ima sadržaja.
        // Svaki chunk se dodaje u finalni string.
        while(fgets(buffer, sizeof(buffer), file))
        {
            content += buffer;
        }

        return content;
    }

    void write(const std::string& text)
    {
        // Pretvara C++ string u C-style string jer fputs radi samo s char*
        // i zapisuje sadržaj u otvoreni file.
        fputs(text.c_str(), file);
    }
};

int main()
{
    // User input omogućuje fleksibilnost (runtime izbor file-a)
    std::cout << "Upisite ime datoteke: ";
    std::cin >> odabir;

    try
    {
        // input file se otvara u read modu
        FileWrapper input(odabir.c_str(), "r");

        // output file se otvara u write modu (overwrite)
        FileWrapper output("output.txt", "w");

        // čitanje cijelog sadržaja input file-a
        std::string content = input.readAll();

        // kopiranje sadržaja u output file
        output.write(content);
    }

    // hvatanje svih runtime grešaka (npr. file ne postoji)
    catch (const std::exception& e)
    {
        printf("Error: %s\n", e.what());
    }

    return 0;
}