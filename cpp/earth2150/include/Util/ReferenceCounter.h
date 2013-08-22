#ifndef REFERENCECOUNTER_H_INCLUDED
#define REFERENCECOUNTER_H_INCLUDED

/**
* Hilfsklasse welche die Anzahl an Referenzen auf ein Objekt zählt.
* Beim erstellen wird die Anzahl auf 1 gesetzt.
* Erreicht der Wert 0, so wird es gelöscht.
* Weitere Referenzen können mit grab() geholt, und mit drop() wieder freigegeben werden.
*/
class ReferenceCounter {
	private:
		/// Der Counter, Mutable damit er auf in Const Objekten funktioniert.
		mutable int32_t referenceCount;

	public:
		ReferenceCounter()
			: referenceCount(1) {};

		virtual ~ReferenceCounter()
			{};

		/// Zählt eine Referenz dazu
		void grab() const {
			++referenceCount;
		}

		/// Zieht eine Referenz ab.
		/// Sollte die Anzahl 0 erreichen, wird das Objekt gelöscht und true zurück gegeben.
		/// Andernfalls return false.
		bool drop() const {
			--referenceCount;

			if (referenceCount == 0) {
				delete this;
				return true;
			}

			return false;
		}

		/// Gibt die Anzahl an Referenzen zurück.
		int32_t getReferenceCount() const {
			return referenceCount;
		}
};

#endif // REFERENCECOUNTER_H_INCLUDED
