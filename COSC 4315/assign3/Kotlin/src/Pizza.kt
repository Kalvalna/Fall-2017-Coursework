class Pizza(){
    var layers: MutableList<String> = mutableListOf()
    var baked: Boolean = false

    infix fun spread(item: String): Pizza{
        layers.add(item)
        return this
    }

    infix fun add(item: String): Pizza{
        layers.add(item)
        return this
    }

    infix fun bake(pizza: String): Pizza{
        if(pizza == "Pizza"){
            baked = true
        }
        return this
    }

    override fun toString(): String{
        return "The layers in the pizza are ${layers.joinToString(", ")}"
    }
}
