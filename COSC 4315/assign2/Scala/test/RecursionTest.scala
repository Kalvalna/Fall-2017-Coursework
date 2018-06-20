import org.junit.Test
import org.junit.Assert._
import org.scalatest._

class RecursionTest extends FlatSpec{

    val factorial = new RecursiveFactorial()

    val fiftyFactorial = BigInt("30414093201713378043612608166064768844377641568960512000000000000")

    @Test
    def canaryTest(){
        assertTrue(true)
    }

    @Test
    def outOfRangeTest(){
        assertEquals(BigInt(-1), factorial.recursiveFactorial(BigInt(-5)))
    }

    @Test
    def zeroFactorialTest(){
        assertEquals(BigInt(1), factorial.recursiveFactorial(BigInt(0)))
    }

    @Test
    def factorialTest01(){
        assertEquals(BigInt(1), factorial.recursiveFactorial(BigInt(1)))
    }

    @Test
    def factorialTest05(){
        assertEquals(BigInt(120), factorial.recursiveFactorial(BigInt(5)))
    }

    @Test
    def factorialTest10(){
        assertEquals(BigInt(3628800), factorial.recursiveFactorial(BigInt(10)))
    }

    @Test
    def factorialTest50(){
         assertEquals(fiftyFactorial, factorial.recursiveFactorial(BigInt(50)))
    }

    @Test
    def testRegularRecursionBlowsUpTheStack(){
        assertThrows[StackOverflowError]{
            factorial.recursiveFactorial(BigInt(10000))
        }
    }
}
