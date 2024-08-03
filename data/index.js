// Function to generate random number within a given range
function getRandomNumber(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }
  
  // Function to generate a random math question
  function generateQuestion() {
    // Generate two random numbers and an operator
    const num1 = getRandomNumber(1, 10);
    const num2 = getRandomNumber(1, 10);
    const operators = ['+', '-', 'x', '/'];
    const operator = operators[getRandomNumber(0, operators.length - 1)];
  
    // Calculate the correct answer
    let correctAnswer;
    switch (operator) {
      case '+':
        correctAnswer = num1 + num2;
        break;
      case '-':
        correctAnswer = num1 - num2;
        break;
      case 'x':
        correctAnswer = num1 * num2;
        break;
      case '/':
        // Ensure no division by zero and result is an integer
        if (num2 === 0) num2 = 1;
        correctAnswer = Math.floor(num1 / num2);
        // Adjust num1 to ensure it is divisible by num2
        num1 = correctAnswer * num2;
        break;
    }
  
    // Generate incorrect answers
    const answers = new Set();
    answers.add(correctAnswer);
    while (answers.size < 4) {
      let wrongAnswer;
      do {
        wrongAnswer = getRandomNumber(correctAnswer - 10, correctAnswer + 10);
      } while (wrongAnswer === correctAnswer || wrongAnswer <= 0 || answers.has(wrongAnswer));
      answers.add(wrongAnswer);
    }
    
    // Shuffle answers
    const answerArray = Array.from(answers).sort(() => Math.random() - 0.5);
  
    // Update the HTML with the new question and answers
    document.getElementById('numn1').textContent = num1;
    document.getElementById('num2').textContent = num2;
    document.getElementById('operand').textContent = operator;
  
    document.getElementById('ans1').textContent = answerArray[0];
    document.getElementById('ans2').textContent = answerArray[1];
    document.getElementById('ans3').textContent = answerArray[2];
    document.getElementById('ans4').textContent = answerArray[3];
  
    // Store the correct answer's element id
    const correctAnswerId = `ans${answerArray.indexOf(correctAnswer) + 1}`;
    document.querySelectorAll('.answer').forEach((element) => {
      element.onclick = function() {
        if (element.querySelector('h1').id === correctAnswerId) {
          alert('Correct!');
        } else {
          alert('Try again!');
        }
        generateQuestion(); // Generate a new question after answering
      };
    });
  }
  
  // Initialize the game
  document.addEventListener('DOMContentLoaded', generateQuestion);
  