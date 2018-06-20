module StockCutting where

import Data.List as List
import Data.Map as Map
import Data.Set as Set
import Data.Ord as Ord

lengths :: [(Int, Int)] -> [Int]
lengths ((length,price):xs) = length : lengths xs
lengths _ = []

partitions :: Int -> [Int] -> [[Int]]
partitions 0 xs = [[]]
partitions _ [] = []
partitions length (xs@(h:t))    | length < 0 = []
                                | otherwise = (List.map (h:) (partitions (length-h) xs)) ++ partitions length t

setPrice :: [Int] -> [(Int, Int)] -> (Int, [Int])
setPrice xs prices =
    let
        priceMap = Map.fromList prices
        price = sum [priceMap Map.! x | x <-xs]
        cuts = xs
    in
        (price, cuts)

maxPrice :: Int -> [(Int, Int)] -> (Int, [[Int]])
maxPrice rodLength prices
    | rodLength == 0 = (0, [])
    | otherwise =
        let
            allCuts = List.filter ((== rodLength) . sum) $ partitions rodLength (lengths prices)
            allCutsWithPrice = [setPrice x prices | x <- allCuts]
            maxPrice = fst $ maximumBy (comparing fst) allCutsWithPrice
            maxPriceCuts = List.filter ((==maxPrice) . fst) allCutsWithPrice
            cuts = [y | (x, y) <- maxPriceCuts]
        in
            (maxPrice, cuts)