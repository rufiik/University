% mergesort
split([], [], []).
split([X], [X], []).
split([X,Y|Rest], [X|Left], [Y|Right]) :-
    split(Rest, Left, Right).

merge([], L, L).
merge(L, [], L).
merge([X|Xs], [Y|Ys], [X|Result]) :-
    X =< Y,
    merge(Xs, [Y|Ys], Result).
merge([X|Xs], [Y|Ys], [Y|Result]) :-
    X > Y,
    merge([X|Xs], Ys, Result).

mergesort([], []).
mergesort([X], [X]).
mergesort(List, Sorted) :-
    split(List, Left, Right),
    mergesort(Left, SortedLeft),
    mergesort(Right, SortedRight),
    merge(SortedLeft, SortedRight, Sorted).
% 2. Równanie diofantyczne 

extended_gcd(A, 0, 1, 0, A) :-
    A > 0.
extended_gcd(A, 0, 1, 0, A) :-
    A < 0,
    A1 is -A,
    extended_gcd(A1, 0, 1, 0, A1).

extended_gcd(A, B, X, Y, D) :-
    B \= 0,
    A1 is abs(A),
    B1 is abs(B),
    R is A1 mod B1,
    Q is A1 // B1,
    extended_gcd(B1, R, X1, Y1, D),
    X = Y1,
    Y is X1 - Q * Y1.

de(A, B, X, Y, Z) :-
    extended_gcd(A, B, X, Y, Z).
%  Rozkład na czynniki pierwsze
is_prime(2) :- !.
is_prime(3) :- !.
is_prime(N) :-
    N > 3,
    N mod 2 =\= 0,
    N mod 3 =\= 0,
    check_prime(N, 5).

check_prime(N, I) :-
    I * I > N,
    !.
check_prime(N, I) :-
    N mod I =:= 0,
    !,
    fail.
check_prime(N, I) :-
    I2 is I + 2,
    check_prime(N, I2).

prime_factors(1, []) :- !.
prime_factors(N, [N]) :-
    is_prime(N),
    !.
prime_factors(N, [P|Rest]) :-
    prime_factor(N, P, N1),
    prime_factors(N1, Rest).

prime_factor(N, P, N1) :-
    P is 2,
    N mod P =:= 0,
    N1 is N // P,
    !.
prime_factor(N, P, N1) :-
    P is 3,
    N mod P =:= 0,
    N1 is N // P,
    !.
prime_factor(N, P, N1) :-
    find_factor(N, 5, P, N1).

find_factor(N, I, P, N1) :-
    N mod I =:= 0,
    !,
    P = I,
    N1 is N // I.
find_factor(N, I, P, N1) :-
    I2 is I + 2,
    find_factor(N, I2, P, N1).

% totient
gcd(A, 0, A) :- !.
gcd(A, B, G) :-
    B \= 0,
    R is A mod B,
    gcd(B, R, G).

relatively_prime(A, B) :-
    gcd(A, B, G),
    G =:= 1.

count_rel_prime(0, _, Acc, Acc) :- !.
count_rel_prime(I, N, Acc, Result) :-
    I > 0,
    (   relatively_prime(I, N)
    ->  NewAcc is Acc + 1
    ;   NewAcc is Acc
    ),
    I1 is I - 1,
    count_rel_prime(I1, N, NewAcc, Result).

totient(N, T) :-
    N > 0,
    count_rel_prime(N, N, 0, T).
totient(1, 1) :- !.

% Sito Eratostenesa 

generate_range(N, List) :-
    generate_range(2, N, List).

generate_range(High, High, [High]) :- !.
generate_range(Current, High, [Current|Rest]) :-
    Current < High,
    Next is Current + 1,
    generate_range(Next, High, Rest).

sieve([], []).
sieve([P|Rest], [P|Primes]) :-
    remove_multiples(P, Rest, Filtered),
    sieve(Filtered, Primes).

remove_multiples(_, [], []).
remove_multiples(P, [X|Xs], Result) :-
    X mod P =:= 0,
    !,
    remove_multiples(P, Xs, Result).
remove_multiples(P, [X|Xs], [X|Result]) :-
    remove_multiples(P, Xs, Result).

primes(N, Primes) :-
    N >= 2,
    generate_range(N, Range),
    sieve(Range, Primes).
primes(N, []) :-
    N < 2.

test_all :-
    write('TESTY'), nl,
    
    % Test mergesort
    mergesort([3,1,4,1,5,9,2], Sorted),
    write('mergesort([3,1,4,1,5,9,2], '), write(Sorted), write(')'), nl,
    
    % Test de
    de(12, 18, X, Y, Z),
    write('de(12, 18, '), write(X), write(', '), write(Y), write(', '), write(Z), write(')'), nl,
    
    % Test prime_factors
    prime_factors(84, Factors),
    write('prime_factors(84, '), write(Factors), write(')'), nl,
    
    % Test totient
    totient(12, T),
    write('totient(12, '), write(T), write(')'), nl,
    
    % Test primes
    primes(30, PrimesList),
    write('primes(30, '), write(PrimesList), write(')'), nl.

:- test_all.