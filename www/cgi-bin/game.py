#!/usr/bin/python3 

def get_eval_formula(guess):
    guess = guess.strip()
    formula = guess
    n = 0

    for i in guess:
        if ((i in "+-/*") and n != len(guess) - 1 and guess[n + 1] == '0'):
            temp = n + 1
            while (guess[temp] == '0' and temp < len(guess) - 1):
                temp += 1
            formula = formula[:n+1] + formula[(temp):]
        n += 1
    formula = formula.lstrip('0')
    return (formula)


def verify_char_position(guess, correct_equation):
    str_return = ""
    n = 0

    for i in guess:
        if (i == correct_equation[n]):
            str_return += "C"
        elif i in correct_equation:
            str_return += "T"
        else:
            str_return += "X"
        n += 1
    return str_return


def play(correct_equation, guess):
    correct_equation = correct_equation.strip()

    if (len(guess) != 6):
        return "TAMANHO ERRADO"
    if guess.strip("0123456789+-/*"):
        return "EQUAÇÃO INVÁLIDA"
    formula = get_eval_formula(guess)
    try:
        if (eval(formula) != 42):
            return "EQUAÇÃO INVÁLIDA"
    except SyntaxError:
        return "EQUAÇÃO INVÁLIDA"
    except ZeroDivisionError:
        return "EQUAÇÃO INVÁLIDA"
    return verify_char_position(guess, correct_equation)


if __name__ == "__main__":
    import os
    import json

    body = os.getenv("REQUEST_BODY")
    method = os.getenv("REQUEST_METHOD")
    correct_equation = "8*9-30"

    if (method == "GET"):
        print(json.dumps({"equation": correct_equation}))
    elif (method == "POST"):
        equation = json.loads(body)["equation"]
        result = json.dumps({"result": play(correct_equation, equation)})
        print(result)
