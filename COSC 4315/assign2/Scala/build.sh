mkdir -p bin
export JARS="./tools/junit-4.12.jar:./tools/hamcrest-core-1.3.jar:./tools/scalatest_2.12-3.0.4.jar:./tools/scalactic_2.12-3.0.4.jar:bin"
scalac -classpath $JARS -d bin test/*.scala src/*.scala
scala -classpath $JARS org.junit.runner.JUnitCore RecursionTest
scala -classpath $JARS org.junit.runner.JUnitCore TailRecursionTest
/bin/rm -r bin
