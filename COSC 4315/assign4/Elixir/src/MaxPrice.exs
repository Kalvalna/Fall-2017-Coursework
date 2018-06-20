defmodule MaxPrice do

  defp getFirst(prices) do
    for n <- prices, do: elem(n, 0)
  end

  defp partition([], _), do: [[]]
  defp partition(_,  0), do: [[]]
  defp partition([h|t]=xs, length) do
    (for l <- partition(xs,length-1), do: [h|l]) ++ partition(t, length)
  end

  defp setPrice(cuts, prices) do
    priceMap = Enum.into(prices, %{})
    price = Enum.sum(for n <- cuts, do: Map.get(priceMap, n))
    {price, cuts}
  end

  def maxPrice(length, prices) do
      if length == 0 do {0, []}
      else
        allCuts = Enum.uniq(Enum.concat(Enum.sort(for m <- 1..length, do: Enum.filter(partition(getFirst(prices), m), fn(x) -> Enum.sum(x) == length end))))
        allCutsWithPrice = for n <- allCuts, do: setPrice(n, prices)
        max = Enum.max(getFirst(allCutsWithPrice))
        cuts = for n <- allCutsWithPrice, (elem(n,0) == max), do: elem(n,1)
        {max, cuts}
      end
  end
end
