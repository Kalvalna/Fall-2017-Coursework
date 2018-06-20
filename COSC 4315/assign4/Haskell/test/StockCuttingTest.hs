import Test.HUnit
import StockCutting

canaryTest = TestCase (assertEqual "Canary Test" True True)

test01 =
    let
        priceList = []
        rodLength = 0
    in
        TestCase ( assertEqual "Length 0" (0, []) (maxPrice rodLength priceList))

test02 =
    let
        priceList = [(1,1),(2,2),(3,3)]
        rodLength = 1
    in
        TestCase ( assertEqual "Length 1" (1, [[1]]) (maxPrice rodLength priceList))

test03 =
    let
        priceList = [(1,1),(2,2),(3,3)]
        rodLength = 2
    in
        TestCase ( assertEqual "Length 2" (2, [[1,1], [2]]) (maxPrice rodLength priceList))

test04 =
    let
        priceList = [(1,1),(2,2),(3,3)]
        rodLength = 3
    in
        TestCase ( assertEqual "Length 3" (3, [[1,1,1], [1,2],[3]]) (maxPrice rodLength priceList))

test05 =
    let
        priceList = [(1,1),(2,2),(3,3),(4,2)]
        rodLength = 4
    in
        TestCase ( assertEqual "Length 4" (4, [[1,1,1,1], [1,1,2],[1,3],[2,2]]) (maxPrice rodLength priceList))

test06 =
    let
        priceList = [(1,1),(2,2),(3,3),(4,2)]
        rodLength = 17
        max = fst (maxPrice rodLength priceList)
        size = length (snd (maxPrice rodLength priceList))
    in
        TestCase ( assertEqual "Length 17" (17, 33) (max, size))

tests = TestList
    [TestLabel "Canary Test" canaryTest,
    TestLabel "Length 0" test01,
    TestLabel "Length 1" test02,
    TestLabel "Length 2" test03,
    TestLabel "Length 3" test04,
    TestLabel "Length 4" test05,
    TestLabel "Length 17" test06]

main = runTestTT tests
