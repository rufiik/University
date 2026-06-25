;;  Dwumian Newtona 
(defun binomial (n k)
  (cond ((or (= k 0) (= k n)) 1)
        ((or (< k 0) (> k n)) 0)
        (t (+ (binomial (1- n) k)
              (binomial (1- n) (1- k))))))

;;Dwumian Newtona  trójkąt Pascala
(defun next-pascal-row (row)
  (cons 1 (append (mapcar #'+ row (cdr row)) '(1))))

(defun pascal-triangle-row (n)
  (if (= n 0)
      '(1)
      (next-pascal-row (pascal-triangle-row (1- n)))))

(defun binomial2 (n k)
  (nth k (pascal-triangle-row n)))

;;Sortowanie przez scalanie
(defun merge-lists (left right)
  (cond ((null left) right)
        ((null right) left)
        ((<= (car left) (car right))
         (cons (car left) (merge-lists (cdr left) right)))
        (t (cons (car right) (merge-lists left (cdr right))))))

(defun mergesort (lst)
  (if (<= (length lst) 1)
      lst
      (let ((mid (floor (length lst) 2)))
        (merge-lists (mergesort (subseq lst 0 mid))
                     (mergesort (subseq lst mid))))))

;;  Równanie diofantyczne 
(defun extended-gcd (a b)
  (if (= b 0)
      (list 1 0 a)
      (let* ((result (extended-gcd b (mod a b)))
             (x (first result))
             (y (second result))
             (d (third result)))
        (list y (- x (* y (floor a b))) d))))

(defun de (a b)
  (extended-gcd (abs a) (abs b)))

;;  Rozkład na czynniki pierwsze
(defun prime-factors (n)
  (defun factor-aux (num divisor)
    (cond ((= num 1) nil)
          ((= divisor num) (list num))
          ((= (mod num divisor) 0)
           (cons divisor (factor-aux (/ num divisor) divisor)))
          (t (factor-aux num (1+ divisor)))))
  (factor-aux n 2))

;;  Funkcja Eulera - wersja klasyczna
(defun relative-prime-p (a b)
  (= (gcd a b) 1))

(defun totient (n)
  (if (= n 1)
      1
      (let ((cnt 0))
        (do ((i 1 (1+ i)))
            ((> i n) cnt)
          (when (relative-prime-p i n)
            (incf cnt))))))

;; Funkcja Eulera - na podstawie rozkładu na czynniki pierwsze
(defun remove-duplicates-sorted (lst)
  (if (null lst)
      nil
      (if (equal (car lst) (cadr lst))
          (remove-duplicates-sorted (cdr lst))
          (cons (car lst) (remove-duplicates-sorted (cdr lst))))))

(defun unique-prime-factors (n)
  (remove-duplicates-sorted (prime-factors n)))

(defun totient2 (n)
  (if (= n 1)
      1
      (let* ((pf (prime-factors n))
             (factors (unique-prime-factors n)))
        (reduce #'* (mapcar (lambda (p)
                              (* (1- p) (expt p (1- (count p pf)))))
                            factors)
                :initial-value 1))))

;; Lista liczb pierwszych do n (sito Eratostenesa)
(defun primes (n)
  (if (< n 2)
      nil
      (let ((sieve (make-list (1+ n) :initial-element t)))
        (setf (elt sieve 0) nil)
        (setf (elt sieve 1) nil)
        (do ((i 2 (1+ i)))
            ((> i (sqrt n)))
          (when (elt sieve i)
            (do ((j (* i i) (+ j i)))
                ((> j n))
              (setf (elt sieve j) nil))))
        (do ((i 2 (1+ i))
             (res '()))
            ((> i n) (nreverse res))
          (when (elt sieve i)
            (push i res))))))

(defun uruchom-testy ()
  (format t "TESTY~%")
  (format t "binomial 5 2: ~A~%" (binomial 5 2))
  (format t "binomial2 5 2: ~A~%" (binomial2 5 2))
  (format t "mergesort: ~A~%" (mergesort '(3 1 4 1 5 9 2)))
  (format t "de 12 18: ~A~%" (de 12 18))
  (format t "prime_factors 84: ~A~%" (prime-factors 84))
  (format t "totient 12: ~A~%" (totient 12))
  (format t "totient2 12: ~A~%" (totient2 12))
  (format t "primes 30: ~A~%" (primes 30))
)
(uruchom-testy)