defmodule TailRecursiveFactorial do
    def tailFactorial(num, sum \\ 1)

    def tailFactorial(num, sum) when num < 0 do
        sum * -1
    end

    def tailFactorial(0, sum), do: sum

    def tailFactorial(num, sum) do
        tailFactorial(num - 1, num * sum)
    end
end
