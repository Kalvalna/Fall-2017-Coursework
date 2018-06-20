ExUnit.start()
Code.require_file("src/RecursiveFactorial.exs")

defmodule RecursiveTest do
  use ExUnit.Case

  test "Canary Test" do
    assert true == true
  end

  test "Out Of Range Test" do
    assert RecursiveFactorial.recursiveFact(-5) == -1
  end

  test "Zero Factorial Test" do
    assert RecursiveFactorial.recursiveFact(0) == 1
  end

  test "Factorial Test 1" do
    assert RecursiveFactorial.recursiveFact(1) == 1
  end

  test "Factorial Test 5" do
    assert RecursiveFactorial.recursiveFact(5) == 120
  end

  test "Factorial Test 10" do
    assert RecursiveFactorial.recursiveFact(10) == 3628800
  end

  test "Factorial Test 50" do
    assert RecursiveFactorial.recursiveFact(50) == 30414093201713378043612608166064768844377641568960512000000000000
  end

  test "Not Tail Call Optimization Test" do
    pid = spawn(fn ->  RecursiveFactorial.recursiveFact(10000); end)
    :timer.sleep(1)
    assert Process.info(pid, :stack_size) != {:stack_size, 0}
  end

end
