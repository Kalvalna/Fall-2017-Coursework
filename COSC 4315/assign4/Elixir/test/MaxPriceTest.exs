ExUnit.start()
Code.require_file("src/MaxPrice.exs")

defmodule MaxPriceTest do
  use ExUnit.Case

  test "Canary Test" do
    assert true == true
  end

  test "Length 0" do
    assert MaxPrice.maxPrice(0, []) == {0, []}
  end

  test "Length 1" do
    assert MaxPrice.maxPrice(1, [{1, 1},{2, 2},{3, 3}]) == {1, [[1]]}
  end

  test "Length 2" do
    assert MaxPrice.maxPrice(2, [{1, 1},{2, 2},{3, 3}]) == {2, [[1,1],[2]]}
  end

  test "Length 3" do
    assert MaxPrice.maxPrice(3, [{1, 1},{2, 2},{3, 3}]) == {3, [[1,1,1],[1,2],[3]]}
  end

  test "Length 4" do
    assert MaxPrice.maxPrice(4, [{1, 1},{2, 2},{3, 3},{4,2}]) == {4, [[1,1,1,1],[1,1,2],[1,3],[2,2]]}
  end

  test "Length 17" do
    tuple = MaxPrice.maxPrice(17, [{1, 1},{2, 2},{3, 3},{4,2}])
    maxPrice = elem(tuple,0)
    size = Enum.count(elem(tuple,1))
    assert {maxPrice, size} == {17, 33}
  end

end
