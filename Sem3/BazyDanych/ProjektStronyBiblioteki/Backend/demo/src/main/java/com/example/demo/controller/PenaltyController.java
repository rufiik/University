package com.example.demo.controller;

import com.example.demo.model.Penalty;
import com.example.demo.repository.PenaltyRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/penalties")
@CrossOrigin(origins = "http://localhost:3000") 
public class PenaltyController {

    @Autowired
    private PenaltyRepository penaltyRepository;

    @GetMapping
    public List<Penalty> getAllPenalties() {
        return penaltyRepository.findAll();
    }

    @GetMapping("/{id}")
    public ResponseEntity<Penalty> getPenaltyById(@PathVariable Long id) {
        Optional<Penalty> penalty = penaltyRepository.findById(id);
        if (penalty.isPresent()) {
            return ResponseEntity.ok(penalty.get());
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    @PostMapping
    public ResponseEntity<Penalty> createPenalty(@RequestBody Penalty penalty) {
        Penalty savedPenalty = penaltyRepository.save(penalty);
        return ResponseEntity.ok(savedPenalty);
    }

    @PutMapping("/{id}")
    public ResponseEntity<Penalty> updatePenalty(@PathVariable Long id, @RequestBody Penalty penaltyDetails) {
        return penaltyRepository.findById(id)
                .map(penalty -> {
                    penalty.setUser(penaltyDetails.getUser());
                    penalty.setBook(penaltyDetails.getBook());
                    penalty.setPenaltyAmount(penaltyDetails.getPenaltyAmount());
                    penalty.setReason(penaltyDetails.getReason());
                    Penalty updatedPenalty = penaltyRepository.save(penalty);
                    return ResponseEntity.ok(updatedPenalty);
                })
                .orElse(ResponseEntity.notFound().build());
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Object> deletePenalty(@PathVariable Long id) {
        return penaltyRepository.findById(id)
                .map(penalty -> {
                    penaltyRepository.delete(penalty);
                    return ResponseEntity.noContent().build();
                })
                .orElse(ResponseEntity.notFound().build());
    }
}