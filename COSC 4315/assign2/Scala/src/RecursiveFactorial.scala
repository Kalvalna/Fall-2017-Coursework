class RecursiveFactorial{

    def recursiveFactorial(num: BigInt): BigInt = {

        if(num.compareTo(0) == -1) return -1

        if (num.compareTo(0) == 0) return 1

        return num * recursiveFactorial(num - 1)
    }
}