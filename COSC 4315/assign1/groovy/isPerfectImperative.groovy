def isPerfectImperative(number){
    def sum = 0
    for (int i in 1..number) {
        if (number % i == 0) {
            sum += i
        }
    }
    sum == 2 * number
}

def numbers = [1, 5, 6, 7, 28, 496]

println("Imperative style:")

for (int i in numbers) {
    def result = isPerfectImperative(i) ? "Yes" : "No"
    println("${i}: ${result}")
}
