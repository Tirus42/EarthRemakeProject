#ifndef PLAYERRELATIONTABLE_H_INCLUDED
#define PLAYERRELATIONTABLE_H_INCLUDED

#include <stdint.h>

/**
* Beschreibt die Beziehung zwischen Spielern.
* Todo: Tabelle so speichern, dass kein Eintrag von Spieler zu sich selbst angelegt wird.
*/
class PlayerRelationTable {
    private:
        /// Anzahl der Spieler
        uint32_t countPlayer;

		/// Tabelle welche die Relationen speichert
        uint8_t* table;

		/// Erzeugt den Speicher der Tabelle
        uint8_t* createTable(uint32_t countPlayer) const;

		/// Gibt den Speicher der Tabelle wieder frei
        void removeTable(uint8_t* table) const;

		/// Berechnet den Offset im Speicher der Tabelle für eine Beziehung
        uint32_t getTableOffset(uint32_t fromPlayer, uint32_t toPlayer) const;

        PlayerRelationTable(const PlayerRelationTable&);
        PlayerRelationTable operator=(const PlayerRelationTable&);

    public:
    	/// Enumeration der Möglichen Beziehungen
        enum {
            RELATION_ENEMY = 0,
            RELATION_NEUTRAL,
            RELEATION_ALLIED,
            RELATION_COUNT
        };

        PlayerRelationTable(uint32_t countPlayer);
        ~PlayerRelationTable();

        /// Setzt unidirektional die Beziehung von einem Spieler zu einem anderen
        void setRelation(uint32_t fromPlayer, uint32_t toPlayer, uint8_t relation);

        /// Setzt bidirektional die Beziehung zwicshen zwei Spielern
        void setRelationBidirectional(uint32_t player1, uint32_t player2, uint8_t relation);

        /// Gibt die unidirektionale Beziehung von einem Spieler zu einem anderen zurück
        uint8_t getRelation(uint32_t fromPlayer, uint32_t toPlayer) const;

        /// Skaliert die Tabelle um. Alle Einträge bleiben bei vergrößerung bestehen, bei verkleinerung werden alle behalten
        /// welche noch reinpassen.
        void resize(uint32_t newCountPlayer);

		/// Gibt die Anzahl an Spieler zurück deren Beziehung gespeichert werden kann
        uint32_t getPlayerCount() const {
			return countPlayer;
        }
};

#endif // PLAYERRELATIONTABLE_H_INCLUDED
