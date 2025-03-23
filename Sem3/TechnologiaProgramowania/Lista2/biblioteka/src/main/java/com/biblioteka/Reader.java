package com.biblioteka;

import java.util.ArrayList;
import java.util.List;
/**
 * Klasa Reader reprezentuje czytelnika.
 * Zastosowane wzorce GRASP:
 * - Information Expert: Klasa Reader jest odpowiedzialna za zarządzanie
 *  listą wypożyczonych egzemplarzy, ponieważ posiada odpowiednie informacje.
 * - Low Coupling: Klasa Reader ma niskie sprzężenie z innymi klasami,
 *  ponieważ używa interfejsów publicznych klasy Copy.
 * - High Cohesion: Klasa Reader jest odpowiedzialna tylko za zarządzanie
 *  listą wypożyczonych egzemplarzy, co zapewnia wysoką spójność.
 */
public class Reader {
    /** name - nazwa czytelnika. */
    private String name;
    /** borrowedCopies - lista wypożyczonych egzemplarzy. */
    private List<Copy> borrowedCopies;
/**
 * Konstruktor tworzy czytelnika o podanej nazwie.
 * @param pname - nazwa czytelnika
 */
    public Reader(final String pname) {
        this.name = pname;
        this.borrowedCopies = new ArrayList<>();
    }
/**
 * Metoda getName zwraca nazwę czytelnika.
 * @return name - nazwa czytelnika
 */
    public String getName() {
        return name;
    }
/**
 * Metoda getBorrowedCopies zwraca listę wypożyczonych egzemplarzy.
 * @return borrowedCopies - lista wypożyczonych egzemplarzy
 */
    public List<Copy> getBorrowedCopies() {
        return borrowedCopies;
    }
/**
 * Metoda borrowCopy wypożycza egzemplarz.
 * @param copy - egzemplarz do wypożyczenia
 */
    public void borrowCopy(final Copy copy) {
        copy.setBorrowed(true);
        borrowedCopies.add(copy);
    }
/**
 * Metoda returnCopy zwraca egzemplarz.
 * @param copy - egzemplarz do zwrotu
 */
    public void returnCopy(final Copy copy) {
        copy.setBorrowed(false);
        borrowedCopies.remove(copy);
    }
}
