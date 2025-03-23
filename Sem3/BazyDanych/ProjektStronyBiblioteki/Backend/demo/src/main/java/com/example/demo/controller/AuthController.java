package com.example.demo.controller;

import com.example.demo.model.User;
import com.example.demo.repository.UserRepository;
import org.mindrot.jbcrypt.BCrypt;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

@RestController
@RequestMapping("/api/auth")
@CrossOrigin(origins = "http://localhost:3000")
public class AuthController {

    @Autowired
    private UserRepository userRepository;

    @PostMapping("/register")
    public Map<String, String> register(@RequestBody User user) {
        if (user.getPassword() == null || user.getPassword().isEmpty()) {
            throw new RuntimeException("Password cannot be null or empty");
        }
    
        // Szyfrowanie hasła
        String encodedPassword = BCrypt.hashpw(user.getPassword(), BCrypt.gensalt());
        user.setPasswordHash(encodedPassword); // Tylko to trafia do bazy
        user.setPassword(null); // Wyczyszczenie pola password po użyciu
    
        user.setRole("USER");
        user.setRegistrationDate(LocalDate.now());
        userRepository.save(user);
    
        Map<String, String> response = new HashMap<>();
        response.put("message", "User registered successfully");
        return response;
    }
    
    @PostMapping("/login")
    public Map<String, String> login(@RequestBody Map<String, String> loginRequest) {
        String username = loginRequest.get("username");
        String password = loginRequest.get("password");
    
        Optional<User> userOptional = userRepository.findByUsername(username);
        if (userOptional.isPresent()) {
            User user = userOptional.get();
            if (BCrypt.checkpw(password, user.getPasswordHash())) {
                Map<String, String> response = new HashMap<>();
                response.put("message", "Login successful");
                response.put("role", user.getRole());
                return response;
            }
        }
        throw new RuntimeException("Invalid username or password");
    }
    
}