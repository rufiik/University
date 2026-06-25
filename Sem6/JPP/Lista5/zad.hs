-- Dwumian Newtona - wersja rekurencyjna
binomial :: Int -> Int -> Int
binomial n k
  | k == 0 || k == n = 1
  | k < 0 || k > n = 0
  | otherwise = binomial (n-1) k + binomial (n-1) (k-1)

-- Dwumian Newtona - trójkąt Pascala
nextPascalRow :: [Int] -> [Int]
nextPascalRow row = 1 : zipWith (+) row (tail row) ++ [1]

pascalTriangleRow :: Int -> [Int]
pascalTriangleRow 0 = [1]
pascalTriangleRow n = nextPascalRow (pascalTriangleRow (n-1))

binomial2 :: Int -> Int -> Int
binomial2 n k = pascalTriangleRow n !! k

-- Sortowanie przez scalanie
mergeLists :: Ord a => [a] -> [a] -> [a]
mergeLists [] right = right
mergeLists left [] = left
mergeLists (x:xs) (y:ys)
  | x <= y = x : mergeLists xs (y:ys)
  | otherwise = y : mergeLists (x:xs) ys

mergesort :: Ord a => [a] -> [a]
mergesort [] = []
mergesort [x] = [x]
mergesort lst = 
  let len = length lst
      mid = len `div` 2
      (left, right) = splitAt mid lst
  in mergeLists (mergesort left) (mergesort right)

-- Równanie diofantyczne - rozszerzony algorytm Euklidesa
extendedGcd :: Int -> Int -> (Int, Int, Int)
extendedGcd a 0 = (1, 0, a)
extendedGcd a b =
  let (x, y, d) = extendedGcd b (a `mod` b)
  in (y, x - (a `div` b) * y, d)

de :: Int -> Int -> (Int, Int, Int)
de a b = extendedGcd (abs a) (abs b)

-- Rozkład na czynniki pierwsze
factorAux :: Int -> Int -> [Int]
factorAux 1 _ = []
factorAux num divisor
  | divisor == num = [num]
  | num `mod` divisor == 0 = divisor : factorAux (num `div` divisor) divisor
  | otherwise = factorAux num (divisor + 1)

primeFactors :: Int -> [Int]
primeFactors n = factorAux n 2

-- Funkcja Eulera - wersja klasyczna
gcd' :: Int -> Int -> Int
gcd' a 0 = a
gcd' a b = gcd' b (a `mod` b)

totient :: Int -> Int
totient 1 = 1
totient n = length [i | i <- [1..n], gcd' i n == 1]

-- Funkcja Eulera - na podstawie rozkładu na czynniki pierwsze
removeDuplicatesSorted :: Eq a => [a] -> [a]
removeDuplicatesSorted [] = []
removeDuplicatesSorted [x] = [x]
removeDuplicatesSorted (x:y:xs)
  | x == y = removeDuplicatesSorted (x:xs)
  | otherwise = x : removeDuplicatesSorted (y:xs)

uniquePrimeFactors :: Int -> [Int]
uniquePrimeFactors = removeDuplicatesSorted . primeFactors

countOccurrences :: Eq a => a -> [a] -> Int
countOccurrences x = length . filter (== x)

totient2 :: Int -> Int
totient2 1 = 1
totient2 n =
  let pf = primeFactors n
      factors = uniquePrimeFactors n
  in product [ (p-1) * p ^ (countOccurrences p pf - 1) | p <- factors ]

-- Sito Eratostenesa
primes :: Int -> [Int]
primes n
  | n < 2 = []
  | otherwise = sieve [2..n]
  where
    sieve [] = []
    sieve (x:xs) = x : sieve (filter (\y -> y `mod` x /= 0) xs)

-- Testy
uruchomTesty :: IO ()
uruchomTesty = do
  putStrLn "TESTY"
  putStrLn $ "binomial 5 2: " ++ show (binomial 5 2)
  putStrLn $ "binomial2 5 2: " ++ show (binomial2 5 2)
  putStrLn $ "mergesort: " ++ show (mergesort [3,1,4,1,5,9,2])
  let (x,y,z) = de 12 18
  putStrLn $ "de 12 18: (" ++ show x ++ "," ++ show y ++ "," ++ show z ++ ")"
  putStrLn $ "prime_factors 84: " ++ show (primeFactors 84)
  putStrLn $ "totient 12: " ++ show (totient 12)
  putStrLn $ "totient2 12: " ++ show (totient2 12)
  putStrLn $ "primes 30: " ++ show (primes 30)
main :: IO ()
main = uruchomTesty