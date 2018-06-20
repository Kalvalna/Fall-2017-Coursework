defmodule RecursiveFactorial do
  def recursiveFact(num) when num < 0 do
    -1
  end

  def recursiveFact(0), do: 1

  def recursiveFact(num) when num > 0 do
    num * recursiveFact(num - 1)
  end
end