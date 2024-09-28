// Global variable to store the correct answer number
let correctAnswerNumberGlobal = 0; // Initialize to 0
let correctAnswerGlobal = null; // Global variable to store the correct answer

function getRandomNumber(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function generateQuestion() {
  // Generate two random numbers and an operator
  let num1 = getRandomNumber(1, 10);
  let num2 = getRandomNumber(1, 10);
  const operators = ["+", "-", "x", "/"];
  const operator = operators[getRandomNumber(0, operators.length - 1)];

  // Calculate the correct answer
  let correctAnswer;
  switch (operator) {
    case "+":
      correctAnswer = num1 + num2;
      break;
    case "-":
      correctAnswer = num1 - num2;
      break;
    case "x":
      correctAnswer = num1 * num2;
      break;
    case "/":
      // Ensure no division by zero and result is an integer
      if (num2 === 0) num2 = 1;
      correctAnswer = Math.floor(num1 / num2);
      // Adjust num1 to ensure it is divisible by num2
      num1 = correctAnswer * num2;
      break;
  }

  // Save the correct answer globally
  correctAnswerGlobal = correctAnswer;

  // Generate incorrect answers
  const answers = new Set();
  answers.add(correctAnswer);
  while (answers.size < 4) {
    let wrongAnswer;
    do {
      wrongAnswer = getRandomNumber(correctAnswer - 10, correctAnswer + 10);
    } while (
      wrongAnswer === correctAnswer ||
      wrongAnswer <= 0 ||
      answers.has(wrongAnswer)
    );
    answers.add(wrongAnswer);
  }

  // Shuffle answers
  const answerArray = Array.from(answers).sort(() => Math.random() - 0.5);

  // Update the HTML with the new question and answers
  const num1Element = document.getElementById("num1");
  const num2Element = document.getElementById("num2");
  const operandElement = document.getElementById("operand");
  const ans1Element = document.getElementById("ans1");
  const ans2Element = document.getElementById("ans2");
  const ans3Element = document.getElementById("ans3");
  const ans4Element = document.getElementById("ans4");

  if (
    !num1Element ||
    !num2Element ||
    !operandElement ||
    !ans1Element ||
    !ans2Element ||
    !ans3Element ||
    !ans4Element
  ) {
    console.error("One or more elements are missing in the DOM.");
    return;
  }

  num1Element.textContent = num1;
  num2Element.textContent = num2;
  operandElement.textContent = operator;
  ans1Element.textContent = answerArray[0];
  ans2Element.textContent = answerArray[1];
  ans3Element.textContent = answerArray[2];
  ans4Element.textContent = answerArray[3];

  // Update the global variable with the correct answer number
  correctAnswerNumberGlobal = answerArray.indexOf(correctAnswer) + 1;

  // Send reset request to server
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/reset", true);
  xhttp.send();

  // Fetch the user's answer from the server
  fetchAnswer(correctAnswerGlobal);
}

function fetchAnswer(correctAnswer) {
  fetch("/answer")
    .then((response) => response.text())
    .then((data) => {
      const fetchedAnswerNumber = parseInt(data);
      if (fetchedAnswerNumber === 0) {
        // Retry after 0.1 seconds if the response is 0
        setTimeout(() => fetchAnswer(correctAnswer), 100);
      } else {
        const correctAnswerText = document.getElementById(
          `ans${fetchedAnswerNumber}`
        ).textContent;
        if (parseInt(correctAnswerText) === correctAnswer) {
          showFeedback("#correct-wrap");
          document.getElementById("play-correct-sound").click();
        } else {
          showFeedback("#wrong-wrap");
          document.getElementById("play-wrong-sound").click();
        }
        
        // Generate a new question after displaying feedback
        setTimeout(generateQuestion, 3000); // 3 seconds delay before generating a new question
      }
    })
    .catch((error) => console.error("Error fetching answer:", error));
}

function showFeedback(elementId) {
  const feedbackElement = document.querySelector(elementId);
  feedbackElement.style.display = "flex";
  
  if (elementId === "#correct-wrap") {
    // Get the current score
    const scoreElement = document.getElementById("c-score");
    let currentScore = parseInt(scoreElement.textContent);
    
    // Increment the score
    scoreElement.textContent = currentScore + 1;
  }
  
  setTimeout(() => {
    feedbackElement.style.display = "none";
  }, 2000); // Show feedback for 2 seconds
}


function resetAnswer() {
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/reset", true);
  xhttp.send();

  // Reset global variable to 0
  correctAnswerNumberGlobal = 0;
  
  // Retry fetching the answer to ensure it's reset
  fetchAnswer(correctAnswerGlobal);
}

// Initialize the game
document.addEventListener("DOMContentLoaded", generateQuestion);
