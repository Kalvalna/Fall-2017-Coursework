class PizzaBuilder {
    def create(klass, closure = {}) {
      def instance = klass.newInstance()
      instance.with closure

      instance
    }

    def evaluate(String dsl){
      def closure = new GroovyShell().evaluate("{ -> $dsl }")
      closure.delegate = this

      closure()
    }
}
