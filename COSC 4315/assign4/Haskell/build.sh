mkdir -p bin
ghc -o StockCuttingTest ./test/StockCuttingTest.hs -outputdir ./bin -isrc
./StockCuttingTest
/bin/rm -r bin
rm -f StockCuttingTest
