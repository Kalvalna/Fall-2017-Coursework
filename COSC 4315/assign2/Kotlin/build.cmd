mkdir bin
set JARS=".\tools\junit-4.12.jar;.\tools\hamcrest-core-1.3.jar;.\tools\kotlin-runtime-1.1.51.jar;.\tools\kotlin-test.jar;bin"
call kotlinc -classpath %JARS% -d bin test\*.kt src\*.kt
java -classpath %JARS% org.junit.runner.JUnitCore RecursionTest
java -classpath %JARS% org.junit.runner.JUnitCore TailRecursionTest
rmdir /s /q bin
