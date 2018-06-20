import java.math.BigInteger

class RecursiveFactorial {

    fun recursiveFactorial(num: BigInteger): BigInteger{

        if(num.compareTo(BigInteger.valueOf(0)) == -1) return BigInteger.valueOf(-1)

        if (num.compareTo(BigInteger.valueOf(0)) == 0) return BigInteger.valueOf(1)

        return num.multiply(recursiveFactorial(num - BigInteger.valueOf(1)))
    }
}
