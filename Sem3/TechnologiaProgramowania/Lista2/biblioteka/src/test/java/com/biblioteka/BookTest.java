package com.biblioteka;

import org.junit.Before;
import org.junit.Test;

import java.util.List;

import static org.junit.Assert.*;

public class BookTest {
    private Book book;

    @Before
    public void setUp() {
        book = new Book("Test Book");
    }

    @Test
    public void testGetTitle() {
        assertEquals("Test Book", book.getTitle());
    }

    @Test
    public void testAddCopy() {
        book.addCopy(new Copy());
        List<Copy> copies = book.getCopies();
        assertEquals(1, copies.size());
    }

    @Test
    public void testGetCopies() {
        book.addCopy(new Copy());
        List<Copy> copies = book.getCopies();
        assertEquals(1, copies.size());
    }
}