package com.example.demo.controller;

import com.example.demo.model.Author;
import com.example.demo.model.Book;
import com.example.demo.model.Genre;
import com.example.demo.repository.AuthorRepository;
import com.example.demo.repository.BookRepository;
import com.example.demo.repository.GenreRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/books")
@CrossOrigin(origins = "http://localhost:3000")
public class BookController {

    @Autowired
    private BookRepository bookRepository;

    @Autowired
    private GenreRepository genreRepository;

    @Autowired
    private AuthorRepository authorRepository;

    @GetMapping
    public List<Book> getAllBooks() {
        return bookRepository.findAll();
    }

    @GetMapping("/{id}")
    public ResponseEntity<Book> getBookById(@PathVariable Long id) {
        return bookRepository.findById(id)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<Book> createBook(@RequestBody Book book) {
        if (book.getGenre() == null || book.getGenre().getName() == null) {
            return ResponseEntity.badRequest().body(null);
        }

        if (book.getAuthor() == null || book.getAuthor().getFirstName() == null || book.getAuthor().getLastName() == null) {
            return ResponseEntity.badRequest().body(null);
        }

        Optional<Genre> genre = genreRepository.findByName(book.getGenre().getName());
        if (!genre.isPresent()) {
            Genre newGenre = new Genre();
            newGenre.setName(book.getGenre().getName());
            genreRepository.save(newGenre);
            book.setGenre(newGenre);
        } else {
            book.setGenre(genre.get());
        }

        Optional<Author> author = authorRepository.findByFirstNameAndLastName(book.getAuthor().getFirstName(), book.getAuthor().getLastName());
        if (!author.isPresent()) {
            Author newAuthor = new Author();
            newAuthor.setFirstName(book.getAuthor().getFirstName());
            newAuthor.setLastName(book.getAuthor().getLastName());
            newAuthor.setDateOfBirth(book.getAuthor().getDateOfBirth());
            newAuthor.setNationality(book.getAuthor().getNationality());
            authorRepository.save(newAuthor);
            book.setAuthor(newAuthor);
        } else {
            book.setAuthor(author.get());
        }

        Book savedBook = bookRepository.save(book);
        return ResponseEntity.ok(savedBook);
    }

    @PutMapping("/{id}")
    public ResponseEntity<Book> updateBook(@PathVariable Long id, @RequestBody Book bookDetails) {
        return bookRepository.findById(id)
                .map(book -> {
                    if (bookDetails.getGenre() != null && bookDetails.getGenre().getName() != null) {
                        Optional<Genre> genre = genreRepository.findByName(bookDetails.getGenre().getName());
                        if (!genre.isPresent()) {
                            Genre newGenre = new Genre();
                            newGenre.setName(bookDetails.getGenre().getName());
                            genreRepository.save(newGenre);
                            book.setGenre(newGenre);
                        } else {
                            book.setGenre(genre.get());
                        }
                    }

                    if (bookDetails.getAuthor() != null && bookDetails.getAuthor().getFirstName() != null && bookDetails.getAuthor().getLastName() != null) {
                        Optional<Author> author = authorRepository.findByFirstNameAndLastName(bookDetails.getAuthor().getFirstName(), bookDetails.getAuthor().getLastName());
                        if (!author.isPresent()) {
                            Author newAuthor = new Author();
                            newAuthor.setFirstName(bookDetails.getAuthor().getFirstName());
                            newAuthor.setLastName(bookDetails.getAuthor().getLastName());
                            newAuthor.setDateOfBirth(bookDetails.getAuthor().getDateOfBirth());
                            newAuthor.setNationality(bookDetails.getAuthor().getNationality());
                            authorRepository.save(newAuthor);
                            book.setAuthor(newAuthor);
                        } else {
                            book.setAuthor(author.get());
                        }
                    }

                    book.setTitle(bookDetails.getTitle());
                    book.setIsbn(bookDetails.getIsbn());
                    book.setAvailable(bookDetails.isAvailable());
                    Book updatedBook = bookRepository.save(book);
                    return ResponseEntity.ok(updatedBook);
                })
                .orElse(ResponseEntity.notFound().build());
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Object> deleteBook(@PathVariable Long id) {
        return bookRepository.findById(id)
                .map(book -> {
                    bookRepository.delete(book);
                    return ResponseEntity.noContent().build();
                })
                .orElse(ResponseEntity.notFound().build());
    }
}