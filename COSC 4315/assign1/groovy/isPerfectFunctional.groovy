def isPerfectFunctional(number){
    (number > 1) && (number == (2..Math.sqrt(number))
        .findAll() { number % it == 0 } 
        .inject(1) { sum, i -> sum + i + number/i })
}

def numbers = [1, 5, 6, 7, 28, 496]

println("Functional style:")

for (int i in numbers) {
    def result = isPerfectFunctional(i) ? "Yes" : "No"
    println("${i}: ${result}")
}
