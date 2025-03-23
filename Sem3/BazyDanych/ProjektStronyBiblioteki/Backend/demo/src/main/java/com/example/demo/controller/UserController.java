package com.example.demo.controller;

import com.example.demo.model.User;
import com.example.demo.repository.UserRepository;
import org.mindrot.jbcrypt.BCrypt;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/users")
@CrossOrigin(origins = "http://localhost:3000")
public class UserController {

    private final UserRepository userRepository;

    public UserController(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    // GET - Pobieranie wszystkich użytkowników
    @GetMapping
    public List<User> getAllUsers() {
        return userRepository.findAll();
    }

    // GET - Pobieranie użytkownika po ID
    @GetMapping("/{id}")
    public ResponseEntity<User> getUserById(@PathVariable Long id) {
        Optional<User> user = userRepository.findById(id);
        return user.map(ResponseEntity::ok).orElseGet(() -> ResponseEntity.notFound().build());
    }

    // POST - Dodawanie nowego użytkownika
    @PostMapping
    public ResponseEntity<User> createUser(@RequestBody User user) {
        try {
            // Szyfrowanie hasła przed zapisaniem w bazie danych
            String encodedPassword = BCrypt.hashpw(user.getPassword(), BCrypt.gensalt());
            user.setPasswordHash(encodedPassword);
            user.setPassword(null); // Wyczyszczenie pola password po użyciu
            user.setRegistrationDate(LocalDate.now());
            User savedUser = userRepository.save(user);
            return ResponseEntity.ok(savedUser);
        } catch (Exception e) {
            return ResponseEntity.badRequest().body(null);
        }
    }

    // PUT - Aktualizacja istniejącego użytkownika
    @PutMapping("/{id}")
    public ResponseEntity<User> updateUser(@PathVariable Long id, @RequestBody User userDetails) {
        return userRepository.findById(id)
                .map(user -> {
                    user.setUsername(userDetails.getUsername());
                    if (userDetails.getPassword() != null && !userDetails.getPassword().isEmpty()) {
                        // Szyfrowanie hasła przed zapisaniem w bazie danych
                        String encodedPassword = BCrypt.hashpw(userDetails.getPassword(), BCrypt.gensalt());
                        user.setPasswordHash(encodedPassword);
                    }
                    user.setRole(userDetails.getRole());
                    user.setEmail(userDetails.getEmail());
                    User updatedUser = userRepository.save(user);
                    return ResponseEntity.ok(updatedUser);
                })
                .orElseGet(() -> ResponseEntity.notFound().build());
    }

    // DELETE - Usuwanie użytkownika
    @DeleteMapping("/{id}")
    public ResponseEntity<Object> deleteUser(@PathVariable Long id) {
        return userRepository.findById(id)
                .map(user -> {
                    userRepository.delete(user);
                    return ResponseEntity.ok().build();
                })
                .orElseGet(() -> ResponseEntity.notFound().build());
    }
}