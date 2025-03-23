package com.biblioteka;
/**
 * Klasa Copy reprezentuje egzemplarz książki.
 * Egzemplarz posiada unikalny identyfikator oraz informację o tym, czy jest wypożyczony.
 * Zastosowane wzorce GRASP:
 * - Information Expert: Klasa Copy jest odpowiedzialna za zarządzanie stanem egzemplarza 
 * (wypożyczony/niewypożyczony) oraz jego unikalnym identyfikatorem, ponieważ posiada odpowiednie informacje.
 * - Low Coupling: Klasa Copy ma niskie sprzężenie z innymi klasami, 
 * ponieważ jest samodzielna i nie zależy bezpośrednio od innych klas.
 * - High Cohesion: Klasa Copy jest odpowiedzialna tylko za zarządzanie 
 * stanem egzemplarza i jego identyfikatorem, co zapewnia wysoką spójność.
 */
public class Copy {
    /**idCounter - licznik identyfikatorów egzemplarzy. */
    private static int idCounter = 1;
    /**id - unikalny identyfikator egzemplarza. */
    private int id;
    /**borrowed - informacja o tym, czy egzemplarz jest wypożyczony. */
    private boolean borrowed;
/**
 * Konstruktor tworzy egzemplarz książki o unikalnym identyfikatorze.
 * Egzemplarz jest początkowo dostępny.
 */
    public Copy() {
        this.id = idCounter++;
        this.borrowed = false;
    }
/**
 * Metoda getId zwraca identyfikator egzemplarza.
 * @return identyfikator egzemplarza
 */
    public int getId() {
        return id;
    }
/**
 * Metoda isBorrowed zwraca informację o tym, czy egzemplarz jest wypożyczony.
 * @return true, jeśli egzemplarz jest wypożyczony, w przeciwnym razie false
 */
    public boolean isBorrowed() {
        return borrowed;
    }
    /**
     * Metoda borrow oznacza egzemplarz jako wypożyczony.
     */
    public void borrow() {
        this.borrowed = true;
    }
/**
 * Metoda returnCopy oznacza egzemplarz jako zwrócony.
 */
    public void returnCopy() {
        this.borrowed = false;
    }
    /**
 * Metoda setBorrowed ustawia informację o tym, czy egzemplarz jest wypożyczony.
 * @param pborrowed true, jeśli egzemplarz jest wypożyczony, w przeciwnym razie false
 */
public void setBorrowed(final boolean pborrowed) {
    this.borrowed = pborrowed;
}
}


