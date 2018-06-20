import org.junit.Test
import org.junit.Assert.*
import org.junit.Before


class PizzaBuilderTest{

    lateinit var pizzaBuilder: PizzaBuilder

    @Before fun setup(){
        pizzaBuilder = PizzaBuilder()
    }


    @Test
    fun canaryTest(){
        assertTrue(true)
    }

    @Test
    fun createPizzaTest(){
        val instance = pizzaBuilder create ::Pizza

        assertEquals("Pizza", instance.javaClass.getName())
    }

    @Test
    fun spreadSauce(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                )

        assertEquals("Sauce", instance.layers.get(0))
    }

    @Test
    fun spreadCheese(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                )

        assertEquals("Cheese", instance.layers.get(1))
    }

    @Test
    fun addMushrooms(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                        add "Mushrooms"
                )

        assertEquals("Mushrooms", instance.layers.get(2))
    }

    @Test
    fun addPepperoni(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                        add "Mushrooms"
                        add "Pepperoni"
                )
        assertEquals("Pepperoni", instance.layers.get(3))
    }

    @Test
    fun addJalapenos(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                        add "Mushrooms"
                        add "Pepperoni"
                        add "Jalapenos"
                )

        assertEquals("Jalapenos", instance.layers.get(4))
    }

    @Test
    fun bakePizza(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                        add "Mushrooms"
                        add "Pepperoni"
                        add "Jalapenos"
                        bake "Pizza"
                )

        assertEquals(true, instance.baked)
    }

    @Test
    fun pizzaString(){
        val instance = (
                pizzaBuilder create ::Pizza
                        spread "Sauce"
                        spread "Cheese"
                        add "Mushrooms"
                        add "Pepperoni"
                        add "Jalapenos"
                        bake "Pizza"
                )

        val results = instance.toString()

        assertEquals("The layers in the pizza are Sauce, Cheese, Mushrooms, Pepperoni, Jalapenos", results)
    }}
