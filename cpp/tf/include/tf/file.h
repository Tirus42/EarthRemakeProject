#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>
#include <stdint.h>

/// Gibt den Typ des Pfardes an: 0 = ungültig, 1 = Datei, 2 = Verzeichnis
uint8_t FileType(const std::string& filename);

/// Gibt die Dateigröße der Angegebenen Datei zurück
int64_t FileSize(const std::string& filename);

#endif // NETWORK_H_INCLUDED
