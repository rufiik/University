-- Utworzenie bazy danych, jeśli nie istnieje
CREATE DATABASE IF NOT EXISTS biblioteka;

-- Użycie bazy danych
USE biblioteka;
-- DELIMITER //
-- CREATE TRIGGER IF NOT EXISTS before_reservation_insert
-- BEFORE INSERT ON reservations
-- FOR EACH ROW
-- BEGIN
--     UPDATE books
--     SET available = false
--     WHERE id = NEW.book_id;
-- END //
-- CREATE TRIGGER IF NOT EXISTS before_reservation_delete
-- BEFORE DELETE ON reservations
-- FOR EACH ROW
-- BEGIN
--     UPDATE books
--     SET available = true
--     WHERE id = OLD.book_id;
-- END //
-- CREATE EVENT IF NOT EXISTS add_penalties_for_overdue_books
-- ON SCHEDULE EVERY 1 DAY
-- DO
-- BEGIN
--     INSERT INTO penalties (user_id, book_id, penalty_amount, reason)
--     SELECT user_id, book_id, 10.00, 'Nieoddanie książki w terminie'
--     FROM reservations
--     WHERE due_date < CURDATE() AND status = 'Zarezerwowana';
-- END //
-- CREATE FUNCTION all_penalty(user_id INT) 
-- RETURNS DECIMAL(10, 2)
-- DETERMINISTIC
-- BEGIN
--     DECLARE total_penalty DECIMAL(10, 2);
--     SELECT SUM(penalty_amount) INTO total_penalty
--     FROM penalties
--     WHERE penalties.user_id = user_id;
--     RETURN IFNULL(total_penalty, 0);
-- END //
-- CREATE PROCEDURE update_overdue_reservations()
-- BEGIN
--     UPDATE Reservations
--     SET status = 'nieoddana'
--     WHERE due_date < CURDATE() AND status = 'zarezerwowana';
-- END //
-- DELIMITER ;

.
-- Tabela Authors
-- Utworzenie bazy danych, jeśli nie istnieje
CREATE DATABASE IF NOT EXISTS biblioteka;

-- Użycie bazy danych
USE biblioteka;

-- Tabela Authors
INSERT IGNORE INTO Authors (id, first_name, last_name, date_of_birth, nationality) VALUES
(1, 'J.K.', 'Rowling', '1965-07-31', 'British'),
(2, 'George', 'Orwell', '1903-06-25', 'British'),
(3, 'J.R.R.', 'Tolkien', '1892-01-03', 'British'),
(4, 'Markus', 'Zusak', '1975-06-23', 'Australian'),
(5, 'Harper', 'Lee', '1926-04-28', 'American');

-- Tabela Genres
INSERT IGNORE INTO Genres (id, name) VALUES
(1, 'Fantasy'),
(2, 'Science Fiction'),
(3, 'Historical Fiction'),
(4, 'Dystopian'),
(5, 'Classic');

-- Tabela Users
INSERT IGNORE INTO Users (id, username, password_hash, role, email, registration_date) VALUES
(1, 'janek123', 'hashpassword1', 'USER', 'janek123@example.com', '2025-01-01'),
(2, 'annakow', 'hashpassword2', 'USER', 'annakow@example.com', '2025-01-02'),
(3, 'marekw', 'hashpassword3', 'USER', 'marekw@example.com', '2025-01-03'),
(4, 'ewak', 'hashpassword4', 'ADMIN', 'ewak@example.com', '2025-01-04'),
(5, 'zosiap', 'hashpassword5', 'USER', 'zosiap@example.com', '2025-01-05');

-- Tabela Books
INSERT IGNORE INTO Books (id, title, isbn, author_id, genre_id, available) VALUES
(1, 'Harry Potter and the Philosopher\'s Stone', '9780747532699', 1, 1, TRUE),
(2, '1984', '9780451524935', 2, 4, TRUE),
(3, 'The Lord of the Rings', '9780261103573', 3, 1, TRUE),
(4, 'The Book Thief', '9780375842207', 4, 3, TRUE),
(5, 'To Kill a Mockingbird', '9780061120084', 5, 5, TRUE),
(6, 'The Hobbit', '9780547928227', 3, 1, TRUE),
(7, 'Brave New World', '9780060850523', 2, 4, TRUE),
(8, 'The Catcher in the Rye', '9780316769488', 5, 5, TRUE),
(9, 'Fahrenheit 451', '9781451673319', 2, 4, TRUE),
(10, 'The Great Gatsby', '9780743273565', 5, 5, TRUE),
(11, 'Moby Dick', '9781503280786', 5, 5, TRUE),
(12, 'War and Peace', '9780199232765', 5, 5, TRUE),
(13, 'Pride and Prejudice', '9781503290563', 5, 5, TRUE),
(14, 'The Adventures of Sherlock Holmes', '9781508475316', 5, 5, TRUE),
(15, 'Crime and Punishment', '9780486415871', 5, 5, TRUE);

-- Tabela Reservations
INSERT IGNORE INTO Reservations (id, user_id, book_id, reservation_date, due_date, status) VALUES
(1, 1, 1, '2025-01-01', '2025-01-15', 'zarezerwowana'),
(2, 2, 2, '2025-01-02', '2025-01-16', 'zarezerwowana'),
(3, 3, 3, '2025-01-03', '2025-01-17', 'zarezerwowana'),
(4, 4, 4, '2025-01-04', '2025-01-18', 'zarezerwowana'),
(5, 5, 5, '2025-01-05', '2025-01-19', 'zarezerwowana'),
(6, 1, 6, '2025-01-06', '2025-01-20', 'zarezerwowana'),
(7, 2, 7, '2025-01-07', '2025-01-21', 'zarezerwowana'),
(8, 3, 8, '2025-01-08', '2025-01-22', 'zarezerwowana'),
(9, 4, 9, '2025-01-09', '2025-01-23', 'zarezerwowana'),
(10, 5, 10, '2025-01-10', '2025-01-24', 'zarezerwowana');

-- Tabela Penalties
INSERT IGNORE INTO Penalties (id, user_id, book_id, penalty_amount, reason) VALUES
(1, 1, 2, 10.50, 'Late return'),
(2, 3, 7, 5.00, 'Damaged book'),
(3, 5, 10, 7.00, 'Lost book'),
(4, 2, 3, 2.00, 'Late return'),
(5, 4, 1, 15.00, 'Lost book');

