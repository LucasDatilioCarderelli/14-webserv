function empty_input()
{
	for(n; n > 1; n--)
		document.getElementById(n - 1).textContent = ""
}

function get_input()
{
	let	input

	input = ""
	for (let i = 1; i <= 6; i++)
		input += document.getElementById(i).textContent
	return (input)
}

async function enter_input()
{
	let	word
	let	result

	word = get_input()
	result = await get_response(word)
	result = result['result']
	if (result == "EQUAÇÃO INVÁLIDA" || result == "TAMANHO ERRADO")
		alert(result)
	else
	{
		show_squares(result)
		append_history(word, result)
	}
	if (result != "CCCCCC")
		empty_input()
	else
		setTimeout(function() { alert("Parabéns!!") }, 500)
}