package com.biblioteka;

import java.util.List;
/**
 * Interfejs Database reprezentuje bazę danych.
 * Baza danych posiada metody do zapisywania książek oraz czytelników
 * oraz do wczytywania listy książek oraz czytelników.
 */
public interface Database {
    /**
     * Metoda saveBook zapisuje książkę.
     * @param book - książka do zapisania
     */
    void saveBook(Book book);
    /**
     * Metoda saveReader zapisuje czytelnika.
     * @param reader - czytelnik do zapisania
     */
    void saveReader(Reader reader);
    /**
     * Metoda loadBooks wczytuje listę książek.
     * @return lista książek
     */
    List<Book> loadBooks();
    /**
     * Metoda loadReaders wczytuje listę czytelników.
     * @return lista czytelników
     */
    List<Reader> loadReaders();
}
