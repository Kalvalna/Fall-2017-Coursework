import org.junit.Test
import org.junit.Assert.*
import java.math.BigInteger

class TailRecursionTest {

    val tailFactorial = TailRecursionFact()

    val fiftyFactorial = BigInteger("30414093201713378043612608166064768844377641568960512000000000000")

    @Test
    fun canaryTest(){
        assertTrue(true)
    }

    @Test
    fun outOfRangeTest(){
        assertEquals(BigInteger.valueOf(-1), tailFactorial.tailRecursionFact(-5))
    }

    @Test
    fun zeroFactorialTest(){
        assertEquals(BigInteger.valueOf(1), tailFactorial.tailRecursionFact(0))
    }

    @Test
    fun factorialTest01(){
        assertEquals(BigInteger.valueOf(1), tailFactorial.tailRecursionFact(1))
    }

    @Test
    fun factorialTest02(){
        assertEquals(BigInteger.valueOf(120), tailFactorial.tailRecursionFact(5))
    }

    @Test
    fun factorialTest03(){
        assertEquals(BigInteger.valueOf(3628800), tailFactorial.tailRecursionFact(10))
    }

    @Test
    fun factorialTest04(){
        assertEquals(fiftyFactorial, tailFactorial.tailRecursionFact(50))
    }

    @Test
    fun isTailCallOptimized(){
        assertNotNull(tailFactorial.tailRecursionFact(10000))
    }
}