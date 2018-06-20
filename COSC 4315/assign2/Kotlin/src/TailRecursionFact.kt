import java.math.BigInteger

class TailRecursionFact {

    tailrec fun tailRecursionFact(num: Long,result: BigInteger = BigInteger.valueOf(1)) : BigInteger{

        if (num < 0) return BigInteger.valueOf(-1)

        if (num == 0.toLong()) return result

        return tailRecursionFact(num - 1, result * BigInteger.valueOf(num))

    }

}