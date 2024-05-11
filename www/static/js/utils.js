async function get_response(word)
{
	let params = {
		'equation': word
	}
	const resp = await fetch('/equation', {
		headers: {
			'Content-type': 'application/json',
			'Accept': 'application/json'
		},
		body:JSON.stringify(params),
		method: 'POST'
	})
	const promise_ret = await resp.json()
	return promise_ret
}

function show_squares(result)
{
	let	api_response_index

	api_response_index = 0
	for (let i = 7; i <= 12; i++)
		document.getElementById(i).textContent = result[api_response_index++]
}

function append_history(word, result)
{
	let	history

	history = document.createElement("div")
	history.classList.add("history_txt")
	history.textContent = word + ":" + result
	document.getElementById("history").appendChild(history)
}