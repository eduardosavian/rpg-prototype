#include <iostream>
#include <locale.h>
#include <string>
#include <time.h>

using namespace std;

struct Arma {
	int danoMinimo;
	int danoMaximo;
};

struct Jogador {
	string nome;
	int nivel = 42;
	int vida = 100;
	Arma arma{20, 30};
};

struct Inimigo {
	string nome;
	int vida;
	Arma arma;
};

struct Fase {
	string nome;
	Inimigo inimigo[5];
};

template <typename T>
bool taMorto(T morto) {
	if (morto.vida <= 0) {
		cout << "\n" << morto.nome << " morreu\n" << endl;
		return true;
	}
	return false;
}

template <typename T1, typename T2>
void Atacar(T1& atacante, T2& atacado) {
	atacado.vida = atacado.vida - (atacante.arma.danoMinimo + (abs(rand()) % (atacante.arma.danoMaximo - atacante.arma.danoMinimo + 1)));

	cout << "\n O " << atacante.nome << " atacou o " << atacado.nome << " ficando com " << atacado.vida << " de vida";
}

void jogarFase(Jogador& personagemJogador, Fase& faseJogada) { // função jogar fase//
	cout << "Entrando em " << faseJogada.nome << ", o(a)" << personagemJogador.nome << " encontra inimigos, irá ter que batalhar contra todos eles!";

	for (int i = 0; i < 5; i++) {
		bool jogadorMorto = false;
		bool inimigoMorto = false;
		while (jogadorMorto == false && inimigoMorto == false) {
			Atacar(personagemJogador, faseJogada.inimigo[i]);
			Atacar(faseJogada.inimigo[i], personagemJogador);
			jogadorMorto = taMorto(personagemJogador);
			inimigoMorto = taMorto(faseJogada.inimigo[i]);
		}
		if (jogadorMorto) {
			cout << "Você morreu!\n";
		}
		cout << "Parabéns, você matou o inimigo!\n";

	}

}

void inicializador() {

	Jogador personagemJogador; // Struct do personagem do jogador//

	cout << "\n Escreve o nome do seu personagem: ";
	cin >> personagemJogador.nome;

	Fase faseJogada; // struct da fase//

	unsigned int numeroFase;// parametros para a fase//

	while (true) { // Looping para selecionar uma fase válida//
		cout << "\n Escolha uma fase \n 1- Bar \n 2- Floresta \n Escolha:";
		cin >> numeroFase;
		if (numeroFase <= 2) {
			break;
		}
		cout << "\n Opção de fase inválida, escolha novamente!\n ";
		system("read _");
		system("clear");
	}

	if (numeroFase == 1) {
		faseJogada.nome = "Bar do Moe";

		faseJogada.inimigo[0] = { "Segurança", 20, {10, 14} };
		faseJogada.inimigo[1] = { "Amanda", 20, {6, 10} };
		faseJogada.inimigo[2] = { "Pedro", 10, {6, 10} };
		faseJogada.inimigo[3] = { "Aquele cara bêbado", 20, {6, 10} };
		faseJogada.inimigo[4] = { "Barman", 40, {10, 19} };

	}
	else if (numeroFase == 2) {
		faseJogada.nome = "Floresta perdida";

		faseJogada.inimigo[0] = { "Urso", 25, {10, 15} };
		faseJogada.inimigo[1] = { "Cinderela", 25, {6, 10} };
		faseJogada.inimigo[2] = { "Pedro", 10, {6, 10} };
		faseJogada.inimigo[3] = { "Engenheiro", 20, {6, 10} };
		faseJogada.inimigo[4] = { "Fehlauer, o Trolldier", 40, {10, 18} };


	}
	jogarFase(personagemJogador, faseJogada);
}

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");


	int opcaoJogo = 0;

	while (opcaoJogo != 3)
	{
		cout << "Escolha uma opção:";		cout << "\n 1- Jogar \n 2- Informações \n 3- Sair \n Opção: ";
		cin >> opcaoJogo;

		switch (opcaoJogo) { // Switch da escolha do que fazer //
		case 1: // Jogo //
			inicializador();

			break;
		case 2: // Informações sobre os alunos //
			cout << "\nInformações:\n";
			cout << "Jogo feito por Eduardo Savian de Oliveira e Miguel Luiz Dalpra Pereira\n\n";
			break;
		case 3: // Sai do jogo//
			cout << "\n Saiu do jogo! \n";
			break;
		default: // Opção inválida que pede uma novo digito //
			cout << "\n Opção inválida, escolha outra opção!";
			break;
		}
		
	}
	return 0;
}


