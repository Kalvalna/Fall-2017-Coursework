import spock.lang.Specification
import Pizza

class PizzaBuilderTest extends Specification{
  def pizzaBuilder

  void setup() {
    pizzaBuilder = new PizzaBuilder()
  }

  def "Canary Test"(){
    expect : true
  }

  def "Create a pizza"(){
    def instance = pizzaBuilder.evaluate('create Pizza')
    expect:
       instance.class.name == 'Pizza'
  }

  def "Create a pizza and put sauce"(){
    def instance = pizzaBuilder.evaluate('''
      create Pizza, {
        spread sauce
      }
    ''')

    expect:
      instance.layers[0] == 'sauce'
  }

  def "Create a pizza, put sauce, and spread cheese"(){
      def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
        }
      ''')

      expect:
        instance.layers[1] == 'cheese'
  }

  def "Create a pizza, spread sauce, spread cheese, and add mushrooms"(){
      def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms
        }
      ''')

      expect:
        instance.layers[2] == 'mushrooms'
  }

  def "Create a pizza, spread sauce, spread cheese, add mushrooms, and add pepperoni"(){
      def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms, pepperoni
        }
      ''')

      expect:
        instance.layers[2] == 'mushrooms'
        instance.layers[3] == 'pepperoni'
  }

  def "Create a pizza, spread sauce, spread cheese, add mushrooms, add pepperoni, and add jalapenos"(){
      def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms, pepperoni, jalapenos
        }
      ''')

      expect:
        instance.layers[2] == 'mushrooms'
        instance.layers[3] == 'pepperoni'
        instance.layers[4] == 'jalapenos'
  }

    def "Create pizza, spread sauce and cheese, add toppings, and bake"(){
        def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms, pepperoni, jalapenos
          bake
        }
        ''')

        expect:
        instance.baked == true
    }

    def "Create pizza, add layers, bake, and obtaining string containing pizza details"(){
        def instance = pizzaBuilder.evaluate('''
        create Pizza, {
          spread sauce
          spread cheese
          add mushrooms, pepperoni, jalapenos
          bake
        }
        ''')
        def results = instance.toString()

        expect:
        results == "The layers in the pizza are sauce, cheese, mushrooms, pepperoni, jalapenos"
    }
}
