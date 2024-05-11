let n = 1

document.addEventListener("DOMContentLoaded", function createSquares()
{
	const	gameBoard = document.getElementById("board")

	for (let i = 0; i < 12; i++)
	{
		let squares = document.createElement("div")
		squares.classList.add("square")
		squares.setAttribute("id", i + 1)
		gameBoard.appendChild(squares)
	}
})

document.addEventListener("keyup", function(event)
{
	if (n < 7 && ((event.key >= '0' && event.key <= '9') ||
	(event.key == '/') || (event.key == '*') || (event.key == '+')
	|| (event.key == '-')))
	{
		if (document.getElementById(n).textContent == "")
			document.getElementById(n).textContent += event.key
		n++
	}
	else if (event.key == "Backspace")
	{
		if (n > 1)
		{
			document.getElementById(n - 1).textContent = ""
			n--
		}
	}
	else if (event.key == "Enter")
		enter_input()
})
