ExUnit.start()
Code.require_file("src/TailRecursiveFactorial.exs")

defmodule TailRecursiveTest do
  use ExUnit.Case

  test "Out Of Range Test" do
    assert TailRecursiveFactorial.tailFactorial(-5) == -1
  end

  test "Zero Factorial Test" do
    assert TailRecursiveFactorial.tailFactorial(0) == 1
  end

  test "Factorial Test 1" do
    assert TailRecursiveFactorial.tailFactorial(1) == 1
  end

  test "Factorial Test 5" do
    assert TailRecursiveFactorial.tailFactorial(5) == 120
  end

  test "Factorial Test 10" do
    assert TailRecursiveFactorial.tailFactorial(10) == 3628800
  end

  test "Factorial Test 50" do
    assert TailRecursiveFactorial.tailFactorial(50) == 30414093201713378043612608166064768844377641568960512000000000000
  end

  test "Tail Call Optimization Test" do
    pid = spawn(fn ->  TailRecursiveFactorial.tailFactorial(10000); end)
    :timer.sleep(1)
    assert Process.info(pid, :stack_size) == {:stack_size, 0}
  end

end
