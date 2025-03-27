def procurar_mnemonico(string):
    tabela = {
        "zeroL": "0",
        "umL": "1",
        "copiaA": "2",
        "copiaB": "3",
        "nA": "4",
        "nB": "5",
        "AenB": "6",
        "nAeB": "7",
        "AxB": "8",
        "nAxnB": "9",
        "nAxnBn": "A",
        "AeB": "B",
        "AeBn": "C",
        "AoBn": "D",
        "AoB": "E",
        "nAonBn": "F"
    }
    return tabela.get(string, "Erro")


def main():
    try:
        with open("TESTEULA.ULA", "r") as arq, open("testeula.hex", "w") as saida:
            X = ""
            Y = ""
            for linha in arq:
                linha = linha.strip()

                if linha == "inicio:":
                    continue

                elif linha.startswith("A"):
                    X = linha.split("=")[1].replace(";", "")

                elif linha.startswith("B"):
                    Y = linha.split("=")[1].replace(";", "")

                elif linha.startswith("W"):
                    if X == "" or Y == "":
                        print("Erro, X ou Y não foram definidos antes da primeira operação.")
                    else:
                        W = linha.split("=")[1].replace(";", "")
                        try:
                            x = int(X)
                            y = int(Y)
                            saida.write(f"{x:X}{y:X}{procurar_mnemonico(W)}\n")
                        except Exception as e:
                            print("Erro ao converter X ou Y para inteiro:", e)

                elif linha == "fim.":
                    break

    except Exception as e:
        print("Erro ao processar os arquivos:", e)


if __name__ == "__main__":
    main()
