function isPerfectImperative(number){
    let sum = 0;
    for (let i = 0; i <= number; i++){
        if (number % i === 0){
            sum += i;
        }
    }
    return sum === 2 * number;
}

function isPerfectFunctional(number){
    return number * 2 ===
        new Array(number)
            .fill()
            .map((index, i) => i + 1)
            .filter(i => number % i === 0)
            .reduce((sum, i) => sum + i, 0);
}

let numbers = [1, 5, 6, 7, 28, 496];

console.log("Imperative Style:");

for (const i of numbers) {
    let result = isPerfectImperative(i) ? "Yes" : "No";
    console.log(`${i} : ${result}`);
}

console.log("\nFunctional Style:");

for (const i of numbers) {
    let result = isPerfectFunctional(i) ? "Yes" : "No";
    console.log(`${i} : ${result}`);
}
