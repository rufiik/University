(* Dwumian Newtona  *)
let rec binomial n k =
  if k = 0 || k = n then 1
  else if k < 0 || k > n then 0
  else binomial (n-1) k + binomial (n-1) (k-1)

(*  Dwumian Newtona - trójkąt Pascala *)
let rec pair_sums = function
  | [] | [_] -> []
  | x::y::xs -> (x + y) :: pair_sums (y::xs)

let next_pascal_row row =
  1 :: (pair_sums row) @ [1]

let rec pascal_triangle_row = function
  | 0 -> [1]
  | n -> next_pascal_row (pascal_triangle_row (n-1))

let binomial2 n k =
  List.nth (pascal_triangle_row n) k

(* Sortowanie przez scalanie *)
let rec merge_lists left right =
  match left, right with
  | [], _ -> right
  | _, [] -> left
  | x::xs, y::ys ->
      if x <= y then x :: merge_lists xs right
      else y :: merge_lists left ys

let rec split_at n lst =
  if n = 0 then ([], lst)
  else match lst with
    | [] -> ([], [])
    | x::xs -> 
        let (left, right) = split_at (n-1) xs in
        (x :: left, right)

let rec mergesort = function
  | [] -> []
  | [x] -> [x]
  | lst ->
      let len = List.length lst in
      let mid = len / 2 in
      let (left, right) = split_at mid lst in
      merge_lists (mergesort left) (mergesort right)

(*  Równanie diofantyczne  *)
let rec extended_gcd a b =
  if b = 0 then (1, 0, a)
  else
    let (x, y, d) = extended_gcd b (a mod b) in
    (y, x - (a / b) * y, d)

let de a b = extended_gcd (abs a) (abs b)

(*  Rozkład na czynniki pierwsze *)
let rec factor_aux num divisor =
  if num = 1 then []
  else if divisor = num then [num]
  else if num mod divisor = 0 then divisor :: factor_aux (num / divisor) divisor
  else factor_aux num (divisor + 1)

let prime_factors n = factor_aux n 2

(*  Funkcja Eulera *)
let rec gcd a b =
  if b = 0 then a else gcd b (a mod b)

let rec count_relative_primes n i acc =
  if i > n then acc
  else if gcd i n = 1 then count_relative_primes n (i+1) (acc+1)
  else count_relative_primes n (i+1) acc

let totient n =
  if n = 1 then 1
  else count_relative_primes n 1 0

(* Funkcja Eulera - na podstawie rozkładu na czynniki pierwsze *)
let rec remove_duplicates_sorted = function
  | [] -> []
  | [x] -> [x]
  | x::y::xs -> if x = y then remove_duplicates_sorted (x::xs)
                else x :: remove_duplicates_sorted (y::xs)

let unique_prime_factors n = remove_duplicates_sorted (prime_factors n)

let count_occurrences x lst =
  List.fold_left (fun acc y -> if y = x then acc+1 else acc) 0 lst

let rec integer_power base exp =
  if exp = 0 then 1
  else base * integer_power base (exp-1)

let totient2 n =
  if n = 1 then 1
  else
    let pf = prime_factors n in
    let factors = unique_prime_factors n in
    List.fold_left (fun acc p -> acc * (p-1) * integer_power p (count_occurrences p pf - 1)) 1 factors

(*Sito Eratostenesa *)
let primes n =
  let rec sieve = function
    | [] -> []
    | x::xs -> x :: sieve (List.filter (fun y -> y mod x <> 0) xs)
  in
  if n < 2 then []
  else
    let rec range start stop =
      if start > stop then []
      else start :: range (start+1) stop
    in
    sieve (range 2 n)

let uruchom_testy () =
  Printf.printf "TESTY \n";
  Printf.printf "binomial 5 2: %d\n" (binomial 5 2);
  Printf.printf "binomial2 5 2: %d\n" (binomial2 5 2);
  Printf.printf "mergesort: ";
  List.iter (Printf.printf "%d ") (mergesort [3;1;4;1;5;9;2]);
  Printf.printf "\n";
  let (x,y,z) = de 12 18 in
  Printf.printf "de 12 18: (%d,%d,%d)\n" x y z;
  Printf.printf "prime_factors 84: ";
  List.iter (Printf.printf "%d ") (prime_factors 84);
  Printf.printf "\n";
  Printf.printf "totient 12: %d\n" (totient 12);
  Printf.printf "totient2 12: %d\n" (totient2 12);
  Printf.printf "primes 30: ";
  List.iter (Printf.printf "%d ") (primes 30);
  Printf.printf "\n";;
let () = uruchom_testy ()