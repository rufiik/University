package com.biblioteka;

import java.util.ArrayList;
import java.util.List;

/**
 * Klasa Book reprezentuje książkę.
 * Zastosowane wzorce GRASP:
 * - Information Expert: Klasa Book jest odpowiedzialna za zarządzanie tytułem
 *   książki oraz listą egzemplarzy, ponieważ posiada odpowiednie informacje.
 * - Low Coupling: Klasa Book ma niskie sprzężenie z innymi klasami,
 *   ponieważ używa interfejsów publicznych klasy Copy.
 * - High Cohesion: Klasa Book jest odpowiedzialna tylko za zarządzanie tytułem
 *   książki oraz listą egzemplarzy, co zapewnia wysoką spójność.
 */
public final class Book {
    /** Tytuł książki. */
    private String title;

    /** Lista egzemplarzy książki. */
    private List<Copy> copies;
/** Liczba egzemplarzy */
    private int availableCopies;

    /**
     * Konstruktor tworzy książkę o podanym tytule.
     * @param ptitle tytuł książki
     */
    public Book(final String ptitle) {
        this.title = ptitle;
        this.copies = new ArrayList<>();
        this.availableCopies = 0; 
    }
    /**
     * Metoda getAvailableCopies zwraca liczbę dostępnych egzemplarzy książki.
     * @return liczba dostępnych egzemplarzy książki
     */
    public int getAvailableCopies() {
        return availableCopies;
    }
/**
     * Metoda decrementAvailableCopies zmniejsza liczbę dostępnych egzemplarzy książki.
     */
    public void incrementAvailableCopies() {
        availableCopies++;
    }
    /**
     * Metoda decrementAvailableCopies zmniejsza liczbę dostępnych egzemplarzy książki.
     * 
     */
    public void decrementAvailableCopies() {
        if (availableCopies > 0) {
            availableCopies--;
        }
    }
    /**
     * Metoda getTitle zwraca tytuł książki.
     * @return tytuł książki
     */
    public String getTitle() {
        return title;
    }

    /**
     * Metoda getCopies zwraca listę egzemplarzy książki.
     * @return lista egzemplarzy książki
     */
    public List<Copy> getCopies() {
        return copies;
    }

    /**
     * Metoda addCopy dodaje nowy egzemplarz książki.
     * @param copy egzemplarz książki
     */
    public void addCopy(final Copy copy) {
        copies.add(copy);
        incrementAvailableCopies();
    }

    /**
     * Metoda getAvailableCopy zwraca dostępny egzemplarz książki.
     * @return dostępny egzemplarz książki lub null, jeśli wszystkie egzemplarze
     *         są wypożyczone
     */
    public Copy getAvailableCopy() {
        for (Copy copy : copies) {
            if (!copy.isBorrowed()) {
                return copy;
            }
        }
        return null;
    }
}
