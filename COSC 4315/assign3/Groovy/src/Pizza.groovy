class Pizza {
  def layers = []
  def baked

  def spread(item) {
    layers << item
  }

  def add(String[] topping){
    topping.each{ layers << it }
  }

  def getBake(){
    baked = true
  }

  String toString(){
    "The layers in the pizza are ${layers.join(", ")}"
  }

  def propertyMissing(String name) { name }
}
