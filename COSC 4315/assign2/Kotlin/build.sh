mkdir -p bin
export JARS="./tools/junit-4.12.jar:./tools/hamcrest-core-1.3.jar:./tools/kotlin-runtime-1.1.51.jar:./tools/kotlin-test.jar:bin"
kotlinc -classpath $JARS -d bin test/*.kt src/*.kt
java -classpath $JARS org.junit.runner.JUnitCore RecursionTest
java -classpath $JARS org.junit.runner.JUnitCore TailRecursionTest
/bin/rm -r bin
