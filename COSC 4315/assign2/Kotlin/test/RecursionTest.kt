import org.junit.Test
import org.junit.Assert.*
import java.math.BigInteger
import kotlin.test.assertFailsWith

class RecursionTest{

    val factorial = RecursiveFactorial()

    val fiftyFactorial = BigInteger("30414093201713378043612608166064768844377641568960512000000000000")

    @Test
    fun canaryTest(){
        assertTrue(true)
    }

    @Test
    fun outOfRangeTest(){
        assertEquals(BigInteger.valueOf(-1), factorial.recursiveFactorial(BigInteger.valueOf(-5)))
    }

    @Test
    fun zeroFactorialTest(){
        assertEquals(BigInteger.valueOf(1), factorial.recursiveFactorial(BigInteger.valueOf(0)))
    }

    @Test
    fun factorialTest01(){
        assertEquals(BigInteger.valueOf(1), factorial.recursiveFactorial(BigInteger.valueOf(1)))
    }

    @Test
    fun factorialTest05(){
        assertEquals(BigInteger.valueOf(120), factorial.recursiveFactorial(BigInteger.valueOf(5)))
    }

    @Test
    fun factorialTest10(){
        assertEquals(BigInteger.valueOf(3628800), factorial.recursiveFactorial(BigInteger.valueOf(10)))
    }

    @Test
    fun factorialTest50(){
         assertEquals(fiftyFactorial, factorial.recursiveFactorial(BigInteger.valueOf(50)))
    }

    @Test
    fun notTailCallOptimized(){
        assertFailsWith(StackOverflowError::class){
            factorial.recursiveFactorial(BigInteger.valueOf(10000))
        }
    }
}
