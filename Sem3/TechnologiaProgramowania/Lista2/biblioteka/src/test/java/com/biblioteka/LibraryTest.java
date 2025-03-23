package com.biblioteka;
import org.junit.Before;
import org.junit.Test;
import java.util.List;
import java.util.Scanner;
import static org.junit.Assert.*;

public class LibraryTest {
    private Library library;

    @Before
    public void setUp() {
        Database database = new InMemoryDatabase();
        library = new Library(database);
    }

    @Test
    public void testAddBook() {
        library.addBook("Test Book", 3);
        List<Book> books = library.getBooks();
        assertEquals(1, books.size());
        assertEquals("Test Book", books.get(0).getTitle());
        assertEquals(3, books.get(0).getCopies().size());
    }

    @Test
    public void testAddCopy() {
        library.addBook("Test Book", 1);
        library.addCopy("Test Book", 2, new Scanner(System.in));
        List<Book> books = library.getBooks();
        assertEquals(1, books.size());
        assertEquals("Test Book", books.get(0).getTitle());
        assertEquals(3, books.get(0).getCopies().size());
    }

    @Test
    public void testAddReader() {
        library.addReader("Test Reader");
        List<Reader> readers = library.getReaders();
        assertEquals(1, readers.size());
        assertEquals("Test Reader", readers.get(0).getName());
    }

    @Test
    public void testBorrowBook() {
        library.addBook("Test Book", 1);
        library.addReader("Test Reader");
        library.borrowBook("Test Book", "Test Reader");
        List<Book> books = library.getBooks();
        assertTrue(books.get(0).getCopies().get(0).isBorrowed());
    }

    @Test
    public void testDisplayBooks() {
        library.addBook("Test Book", 1);
        library.displayBooks();
        // This test is for manual verification as displayBooks prints to console
    }

    @Test
    public void testDisplayReaders() {
        library.addReader("Test Reader");
        library.displayReaders();
        // This test is for manual verification as displayReaders prints to console
    }
}