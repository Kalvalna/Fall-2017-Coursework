import org.junit.Test
import org.junit.Assert._
import org.scalatest._

class TailRecursionTest extends FlatSpec{

    val factorial = new TailRecursiveFact()

    val fiftyFactorial = BigInt("30414093201713378043612608166064768844377641568960512000000000000")

    @Test
    def canaryTest(){
        assertTrue(true)
    }

    @Test
    def outOfRangeTest(){
        assertEquals(BigInt(-1), factorial.tailRecursiveFact(BigInt(-5)))
    }

    @Test
    def zeroFactorialTest(){
        assertEquals(BigInt(1), factorial.tailRecursiveFact(BigInt(0)))
    }

    @Test
    def factorialTest01(){
        assertEquals(BigInt(1), factorial.tailRecursiveFact(BigInt(1)))
    }

    @Test
    def factorialTest05(){
        assertEquals(BigInt(120), factorial.tailRecursiveFact(BigInt(5)))
    }

    @Test
    def factorialTest10(){
        assertEquals(BigInt(3628800), factorial.tailRecursiveFact(BigInt(10)))
    }

    @Test
    def factorialTest50(){
        assertEquals(fiftyFactorial, factorial.tailRecursiveFact(BigInt(50)))
    }

    @Test
    def testTailRecursionIsTCO(){
        assertNotNull(factorial.tailRecursiveFact(BigInt(10000)))
    }
}
