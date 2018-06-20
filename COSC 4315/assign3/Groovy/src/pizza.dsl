def pizzaBuilder = new PizzaBuilder()

def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms, pepperoni, jalapenos
          bake
        }
        ''')

instance.toString()