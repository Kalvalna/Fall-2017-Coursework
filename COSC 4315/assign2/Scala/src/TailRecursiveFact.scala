import scala.annotation.tailrec

class TailRecursiveFact {

  @tailrec final def tailRecursiveFact(num: BigInt, result: BigInt = 1): BigInt = {

    if(num.compareTo(BigInt(0)) == -1) return BigInt(-1)

    if (num.compareTo(BigInt(0)) == 0) return result

    return tailRecursiveFact(num - BigInt(1), result * num)
  }
}
