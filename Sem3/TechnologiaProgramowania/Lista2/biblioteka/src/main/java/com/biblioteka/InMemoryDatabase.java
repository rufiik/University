package com.biblioteka;

import java.util.ArrayList;
import java.util.List;
/**
 * Klasa InMemoryDatabase reprezentuje bazę danych w pamięci.
 * Baza danych przechowuje listę książek oraz listę czytelników.
 * Zastosowane wzorce GRASP:
 * - Information Expert: Klasa InMemoryDatabase jest odpowiedzialna za zarządzanie
 * listą książek oraz czytelników, ponieważ posiada odpowiednie informacje.
 * - Low Coupling: Klasa InMemoryDatabase ma niskie sprzężenie z innymi klasami,
 * ponieważ używa interfejsów publicznych klas Book i Reader.
 */
public class InMemoryDatabase implements Database {
    /** books - lista książek. */
    private List<Book> books = new ArrayList<>();
   /** readers - lista czytelników. */
    private List<Reader> readers = new ArrayList<>();
/**
 * Metoda saveBook zapisuje książkę.
 * @param book - książka do zapisania
 */
    @Override
    public void saveBook(Book book) {
        books.removeIf(b -> b.getTitle().equals(book.getTitle()));
        books.add(book);
    }
/**
 * Metoda saveReader zapisuje czytelnika.
 * @param reader - czytelnik do zapisania
 */
    @Override
    public void saveReader(final Reader reader) {
        readers.add(reader);
    }
/**
 * Metoda loadBooks wczytuje listę książek.
 * return lista książek
 */
    @Override
    public List<Book> loadBooks() {
        return books;
    }
/**
 * Metoda loadReaders wczytuje listę czytelników.
 * @return lista czytelników
 */
    @Override
    public List<Reader> loadReaders() {
        return readers;
    }
}
