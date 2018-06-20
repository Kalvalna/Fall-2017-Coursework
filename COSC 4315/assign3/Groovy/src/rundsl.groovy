def dsl = new File(args[0]).text

println(new PizzaBuilder().evaluate(dsl))
