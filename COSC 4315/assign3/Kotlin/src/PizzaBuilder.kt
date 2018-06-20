class PizzaBuilder {
    infix fun <T> create(klass: () -> T) : T {
        return klass()
    }
}
