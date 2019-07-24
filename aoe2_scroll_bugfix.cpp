#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdint.h>

struct Signature {
    std::vector<uint8_t> sequence;
    std::vector<unsigned> offsets;
};

std::vector<Signature> signatures{
    { { 0x33 ,0xF6 ,0x80 ,0xBD ,0x21 ,0xFF ,0xFF ,0xFF ,0x01 ,0x76 ,0x05 ,0x83 ,0xCB ,0xFF }, {8, 22} },
    { { 0x80 ,0xBD ,0x22 ,0xFF ,0xFF ,0xFF ,0x01 ,0x8B ,0xD8 ,0x76}, {6} },
    { { 0x80 ,0xBD ,0x24 ,0xFF ,0xFF ,0xFF ,0x01 ,0x76 }, {6} },
    { { 0xC7 ,0x85 ,0xF8 ,0xFE ,0xFF ,0xFF ,0x01 ,0x00 ,0x00 ,0x00 ,0x80 ,0xBD ,0x21 ,0xFF ,0xFF ,0xFF ,0x01 ,0x76 }, {16} },
    { { 0x8B ,0xB5 ,0xF8 ,0xFE ,0xFF ,0xFF ,0x80 ,0xBD ,0x23 ,0xFF ,0xFF ,0xFF ,0x01 ,0x0F ,0x86 }, {12} },
    { { 0x0F ,0xB6 ,0xC8 ,0x8B ,0x45 ,0x10 ,0x80 ,0x3C ,0x01 ,0x01 ,0x77 }, {9} }
};

constexpr uint8_t originalConstant = 0x01;
constexpr uint8_t replacementConstant = 0x7F;

/* 
 * Simple Knuth-Morris-Pratt substring search implementation.
 * See: https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
 */
long memFind(const uint8_t *haystack, size_t haystackLength, const uint8_t *needle, size_t needleLength) {
    if (haystack == nullptr || needle == nullptr || haystackLength == 0 || needleLength == 0 || haystackLength < needleLength) {
        return -1;
    }

    std::vector<int> t(needleLength);
    auto cnd = 0;
    t[0] = -1;
    for (size_t pos = 1; pos < needleLength; pos++) {
        if (needle[pos] == needle[cnd]) {
            t[pos] = t[cnd];

        } else {
            t[pos] = cnd;
            cnd = t[cnd];
            while (cnd >= 0 && needle[pos] != needle[cnd]) {
                cnd = t[cnd];
            }
        }
        cnd++;
    }

    size_t j = 0;
    size_t k = 0;
    while (j < haystackLength) {
        if (haystack[j] == needle[k]) {
            j++;
            k++;
            if (k == needleLength) {
                return j - k;
            }

        } else {
            if (t[k] < 0) {
                k = 0;
                j++;

            } else {
                k = t[k];
            }
        }
    }

    return -1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path to AoK HD.exe>" << std::endl << std::endl;
        return 0;
    }

    const std::string exeFileName = argv[1];

    std::fstream exe(exeFileName, std::ios::in | std::ios::out | std::ios::binary);
    if (!exe.is_open()) {
        std::cerr << "Unable to open file: " << exeFileName << std::endl;
        return 1;
    }

    std::cout << "Reading file " << exeFileName << "...";
    exe.seekg(0, std::ios::end);
    const auto fileSize = static_cast<size_t>(exe.tellg());
    exe.seekg(0, std::ios::beg);
    const auto exeContentsHolder = std::make_unique<uint8_t[]>(fileSize);
    auto exeContents = exeContentsHolder.get();
    exe.read(reinterpret_cast<char*>(exeContents), fileSize);
    std::cout << "done." << std::endl;

    const auto backupFileName = exeFileName + ".backup";
    std::cout << "Backing up original file as " << backupFileName << "...";
    std::fstream backup(backupFileName, std::ios::in | std::ios::binary);
    if (backup.is_open()) {
        backup.close();
        std::cout << "failed!" << std::endl;
        std::cerr << "Backup file " << backupFileName << " already exists. Exiting..." << std::endl;
        return 2;
    }

    backup.open(backupFileName, std::ios::out | std::ios::binary);
    if (!backup.is_open()) {
        std::cout << "failed!" << std::endl;
        std::cerr << "Unable to open backup file " << backupFileName << " for writing. Exiting..." << std::endl;
        return 3;
    }
    backup.write(reinterpret_cast<const char*>(exeContents), fileSize);
    backup.close();
    std::cout << "done." << std::endl;

    auto actPatchIndex = 1;
    for (const auto &s : signatures) {
        const auto startOffset = memFind(exeContents, fileSize, &s.sequence[0], s.sequence.size());

        if (startOffset != -1) {
            for (auto offset : s.offsets) {
                const auto patchOffset = startOffset + offset;
                if (exeContents[patchOffset] == originalConstant) {
                    std::cout << std::hex << std::showbase
                        << "Patching at offset " << patchOffset
                        << std::endl;

                    exe.seekp(patchOffset);
                    exe.put(replacementConstant);

                } else {
                    std::cout << std::hex << std::showbase
                              << "Mismatch at offset " << patchOffset
                              << " (expected " << static_cast<int>(originalConstant)
                              << " got " << static_cast<int>(exeContents[patchOffset])
                              << ") skipping..."
                              << std::endl;
                }
            }

        } else {
            std::cout << "Unable to find patch signature #" << actPatchIndex << " skipping..." << std::endl;
        }

        actPatchIndex++;
    }

    exe.close();

    return 0;
}
