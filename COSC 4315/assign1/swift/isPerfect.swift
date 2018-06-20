func isPerfectImperative(number: Int) -> Bool {
    var sum = 0
    for index in 1...number {
        if (number % index == 0) {
            sum += index
        }
    }
    return sum == 2 * number
}

func isPerfectFunctional(number: Int) -> Bool {
    return number == (1..<number)
      .filter({number % $0 == 0})
      .reduce(0, {$0 + $1})
}

let numArray = [1, 5, 6, 7, 28, 496]

print("Imperative style:")

for index in numArray {
    let result = isPerfectImperative(number: index) ? "Yes" : "No"
    print("\(index): \(result)")
}

print("")

print("Functional style:")

for index in numArray {
    let result = isPerfectFunctional(number: index) ? "Yes" : "No"
    print("\(index): \(result)")
}
