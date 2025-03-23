package com.example.demo.controller;

import com.example.demo.model.Genre;
import com.example.demo.repository.GenreRepository;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/genres")
@CrossOrigin(origins = "http://localhost:3000") 
public class GenreController {

    private final GenreRepository genreRepository;

    public GenreController(GenreRepository genreRepository) {
        this.genreRepository = genreRepository;
    }

    // GET - Pobieranie wszystkich gatunków
    @GetMapping
    public List<Genre> getAllGenres() {
        return genreRepository.findAll();
    }

    // GET - Pobieranie gatunku po ID
    @GetMapping("/{id}")
    public ResponseEntity<Genre> getGenreById(@PathVariable Long id) {
        return genreRepository.findById(id)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    // POST - Dodawanie nowego gatunku
    @PostMapping
    public Genre createGenre(@RequestBody Genre genre) {
        return genreRepository.save(genre);
    }

    // PUT - Aktualizacja gatunku
    @PutMapping("/{id}")
    public ResponseEntity<Genre> updateGenre(@PathVariable Long id, @RequestBody Genre genreDetails) {
        return genreRepository.findById(id)
                .map(genre -> {
                    genre.setName(genreDetails.getName());
                    return ResponseEntity.ok(genreRepository.save(genre));
                })
                .orElse(ResponseEntity.notFound().build());
    }

    // DELETE - Usuwanie gatunku
    @DeleteMapping("/{id}")
    public ResponseEntity<Object> deleteGenre(@PathVariable Long id) {
        return genreRepository.findById(id)
                .map(genre -> {
                    genreRepository.delete(genre);
                    return ResponseEntity.noContent().build();
                })
                .orElse(ResponseEntity.notFound().build());
    }
}