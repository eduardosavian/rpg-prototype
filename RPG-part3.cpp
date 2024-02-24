#include <cstddef>
#include <iostream>
#include <locale.h>
#include <stdexcept>
#include <string>
#include <time.h>

#ifdef _WIN32
#define PAUSE(msg) system("pause")
#define CLEAR() system("cls")
#else
#define PAUSE(msg) system("echo " msg " ; read _")
#define CLEAR() system("clear")
#endif

using namespace std;

struct Arma {
	int danoMinimo;
	int danoMaximo;
};

struct Jogador {
	string nome;
	int nivel = 42;
	int vida = 0;
	Arma arma{ 10, 20 };
	int posicaoY = 0;
	int posicaoX = 0;
};

struct NPC {
	int posicaoX = 0;
	int posicaoY = 0;
	int inimigosEncontrados = 0;

	NPC(int inimigosEncontrados) {
		this->inimigosEncontrados = inimigosEncontrados;
	}

	NPC(){}
};

struct Inimigo {
	string nome;
	int vida = 20;
	Arma arma = { 10,15 };
};

struct Bloco {
	bool isBloqueado;
	bool isInimigo;
	bool isSaida = false;
	bool isRota = false;
	Inimigo* inimigoBloco;
	bool blocoSemSaida = false;
	int x, y;
};

struct Mapa {
	int altura;
	int largura;
	Bloco** matriz;
};

struct Fase {
	string nome;
	Mapa mapa = {};
	int numeroInimigos = {};
	Inimigo* inimigo = {};
};


enum Direcao { Cima, Baixo, Direita, Esquerda };

void imprimirMapa(Fase fase, Jogador jogador) {
	for (int i = 0; i < fase.mapa.altura; i++) {
		for (int j = 0; j < fase.mapa.largura; j++) {
			if (i == jogador.posicaoY && j == jogador.posicaoX) {
				cout << "@";
			}
			else if (fase.mapa.matriz[i][j].isInimigo) {
				// cout << fase.mapa.matriz[i][j].inimigoBloco->nome;
				cout << "X";
			}
			else if (fase.mapa.matriz[i][j].isSaida) {
				cout << "F";
			}
			else if (fase.mapa.matriz[i][j].isBloqueado) {
				cout << "#";
			}
			else {
				cout << "_";
			}
			cout << " ";
		}
		cout << endl;
	}
	cout << "[" << jogador.nome << "]-[" << jogador.vida << "HP]\n";
	cout << "@ = Jogador X = Inimigo # = Pedra - Caminho\n";
}

void imprimirMapa(Fase fase, NPC npc) {
	for (int i = 0; i < fase.mapa.altura; i++) {
		for (int j = 0; j < fase.mapa.largura; j++) {
			if (i == npc.posicaoY && j == npc.posicaoX) {
				cout << "@";
			}
			else if (fase.mapa.matriz[i][j].isInimigo) {
				// cout << fase.mapa.matriz[i][j].inimigoBloco->nome;
				cout << "X";
			}
			else if (fase.mapa.matriz[i][j].isSaida) {
				cout << "F";
			}
			else if (fase.mapa.matriz[i][j].isBloqueado) {
				cout << "#";
			}
			else {
				cout << "_";
			}
			cout << " ";
		}
		cout << endl;
	}
	cout << "\n @ = Jogador X = Inimigo # = Pedra - Caminho\n";
}

bool verLimite(NPC * npc, Fase fase, Direcao direcao) {

	if (direcao == Direita) {
		if (npc->posicaoY + 1 >= fase.mapa.largura) {
			return false;
		}
		return true;
	}

	if (direcao == Esquerda) {
		if (npc->posicaoY - 1 < 0) {
			return false;
		}
		return true;
	}

	if (direcao == Baixo) {
		if (npc->posicaoX + 1 >= fase.mapa.altura) {
			return false;
		}
		return true;
	}

	if (direcao == Cima) {
		if (npc->posicaoX - 1 < 0) {
			return false;
		}
		return true;
	}
	return false;
}

bool verPedra(NPC * npc, Fase fase, Direcao direcao) {

	if (direcao == Direita) {

		if (fase.mapa.matriz[npc->posicaoY + 1][npc->posicaoX].isBloqueado) {
			return true;
		}
		return false;
	}

	if (direcao == Esquerda) {

		if (fase.mapa.matriz[npc->posicaoY - 1][npc->posicaoX].isBloqueado) {
			return true;
		}
		return false;
	}

	if (direcao == Baixo) {

		if (fase.mapa.matriz[npc->posicaoY][npc->posicaoX + 1].isBloqueado) {
			return true;
		}
		return false;
	}
	if (direcao == Cima) {
		if (fase.mapa.matriz[npc->posicaoY][npc->posicaoX - 1].isBloqueado) {
			return true;
		}
		return false;

	}
	return false;
}

bool verCaminho(NPC * npc, Fase fase, Direcao direcao) {

	if (direcao == Direita) {

		return fase.mapa.matriz[npc->posicaoY + 1][npc->posicaoX].isRota;
	}

	if (direcao == Esquerda) {

		return fase.mapa.matriz[npc->posicaoY - 1][npc->posicaoX].isRota;
	}

	if (direcao == Baixo) {

		return fase.mapa.matriz[npc->posicaoY][npc->posicaoX + 1].isRota;
	}

	if (direcao == Cima) {

		return fase.mapa.matriz[npc->posicaoY][npc->posicaoX - 1].isRota;
	}
	return false;
}

bool naoEhJogavel(NPC * npc, const Fase &fase) {

	if (fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isInimigo) {
		fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isRota = true;
		npc->inimigosEncontrados++;
	}

	if (fase.numeroInimigos == npc->inimigosEncontrados && fase.mapa.matriz[fase.mapa.largura - 1][fase.mapa.altura - 1].isRota) {
		return true;
	}

	imprimirMapa(fase, *npc);

	auto direcao = Direita;
	auto posicaoMovimento = verLimite(npc, fase, direcao);

	if (posicaoMovimento) {
		auto ehPedra = verPedra(npc, fase, direcao);
		if (!ehPedra) {
			auto ehRota = verCaminho(npc, fase, direcao);
			if (!ehRota) {
				npc->posicaoY++;
				fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isRota = true;
				return naoEhJogavel(npc, fase);
				npc->posicaoY--;
			}
		}
	}

	direcao = Esquerda;
	posicaoMovimento = verLimite(npc, fase, direcao);

	if (posicaoMovimento) {
		auto ehPedra = verPedra(npc, fase, direcao);
		if (!ehPedra) {
			auto ehRota = verCaminho(npc, fase, direcao);
			if (!ehRota) {
				npc->posicaoY--;
				fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isRota = true;
				return naoEhJogavel(npc, fase);
				npc->posicaoY++;
			}
		}
	}

	direcao = Baixo;
	posicaoMovimento = verLimite(npc, fase, direcao);

	if (posicaoMovimento) {
		auto ehPedra = verPedra(npc, fase, direcao);
		if (!ehPedra) {
			auto ehRota = verCaminho(npc, fase, direcao);
			if (!ehRota) {
				npc->posicaoX++;
				fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isRota = true;
				return naoEhJogavel(npc, fase);
				npc->posicaoX--;
			}
		}
	}

	direcao = Cima;
	posicaoMovimento = verLimite(npc, fase, direcao);

	if (posicaoMovimento) {
		auto ehPedra = verPedra(npc, fase, direcao);
		if (!ehPedra) {
			auto ehRota = verCaminho(npc, fase, direcao);
			if (!ehRota) {
				npc->posicaoX--;
				fase.mapa.matriz[npc->posicaoY][npc->posicaoX].isRota = true;
				return naoEhJogavel(npc, fase);
				npc->posicaoX++;
			}
		}
	}



	return false;
}

Mapa criarMapa(int altura, int largura) {
	Mapa mapa{};

	mapa.altura = altura;
	mapa.largura = largura;

	mapa.matriz = new Bloco *[mapa.altura]; // Cria a matriz dinamica que é o mapa

	for (int i = 0; i < mapa.altura; i++) {
		mapa.matriz[i] = new Bloco[mapa.largura]; // Cria as camadas do array bidimenional
	}

	for (int i = 0; i < mapa.altura; i++) { // Loop para colocar se é ou terra ou pedra
		for (int j = 0; j < mapa.largura; j++) {
			int numeroSorteiro = 0; // Declaração do número para sorteiro se é inimigo

			mapa.matriz[i][j].isBloqueado = false; // Atribui como padrão q é caminho até que se coloque o ontrario

			numeroSorteiro = (rand() % 2); // Calculo de 50% (0) para ser pedra ou 50% (1) para ser terra

			if (numeroSorteiro == 0) {
				mapa.matriz[i][j].isBloqueado = true; // Atribui que é pedra
			}
		}
	}
	mapa.matriz[0][0].isBloqueado = true; // Posição inicial do jogador não pode ser pedra
	mapa.matriz[mapa.altura - 1][mapa.largura - 1].isBloqueado = true; // Posição final do jogo não pode ser pedra

	return mapa; // Retorna o mapa
}

Inimigo* criarInimigos(int numeroInimigos, int tipo) {
	Inimigo* inimigos = new Inimigo[numeroInimigos];
	if (tipo == 1) {
		for (int i = 0; i < numeroInimigos; i++) {
			if (i == 0) {
				inimigos[0] = { "Barman", 40,{ 10, 18 } };
			}
			else if (i == 1) {
				inimigos[1] = { "Amanda", 20,{ 6, 10 } };
			}
			else if (i == 2) {
				inimigos[2] = { "Pedro", 10,{ 6, 10 } };
			}
			else if (i == 3) {
				inimigos[3] = { "Aquele cara bêbado", 20,{ 6, 10 } };
			}
			else if (i == 4) {
				inimigos[4] = { "Segurança", 20,{ 10, 14 } };
			}
			else {
				inimigos[i].nome = "Bêbado " + std::to_string(i + 1);
			}
		}
	}
	else if (tipo == 2) {
		for (int i = 0; i < numeroInimigos; i++) {
			if (i == 0) {
				inimigos[0] = { "Fehlauer, o Trolldier", 40,{ 10, 18 } };
			}
			else if (i == 1) {
				inimigos[1] = { "Cinderela", 25,{ 6, 10 } };
			}
			else if (i == 2) {
				inimigos[2] = { "Pedro", 10,{ 6, 10 } };
			}
			else if (i == 3) {
				inimigos[3] = { "Engenheiro", 20,{ 6, 10 } };
			}
			else if (i == 4) {
				inimigos[4] = { "Urso", 25,{ 10, 15 } };
			}
			else {
				inimigos[i].nome = "Duende " + std::to_string(i + 1);
			}
		}
	}
	else {
		return nullptr; // return nulo, algum erro ocorreu pra chegar aqui
	}

	return inimigos; // responsabilidade de quem chamou para desalocar essa memoria
}

Fase criarFase(int numeroInimigos, Inimigo * inimigos, int numeroFase, int alturaMapa, int larguraMapa) {
	Fase fase;

	fase.numeroInimigos = numeroInimigos;  // Setar na fase o número de inimigos

	fase.inimigo = inimigos; // Array da fase para apontar para o array da função inicalizar()

	if (numeroFase == 1) {
		fase.nome = "Bar do Moe";
	}
	else if (numeroFase == 2) {
		fase.nome = "Floresta perdida";
	}

	fase.mapa = criarMapa(alturaMapa, larguraMapa); // Declara a variável do tipo Mapa e chama a função para criar a matriz do mapa

	for (int i = 0; i < alturaMapa; i++) { // Loop para colocar todas posições não ter inimigos
		for (int j = 0; j < alturaMapa; j++) {
			fase.mapa.matriz[i][j].isInimigo = false;
		}
	}

	for (int iterador = 0; iterador < numeroInimigos; iterador++) {

		int i = rand() % alturaMapa; // Sorteia o i para colocar o inimigo
		int j = rand() % larguraMapa; // Sorteia o j para colocar o inimigo


		if (fase.mapa.matriz[i][j].isBloqueado == false && fase.mapa.matriz[i][j].isInimigo == false) { // Coloca se não tiver pedra nem inimigo
			fase.mapa.matriz[i][j].inimigoBloco = fase.inimigo; // Faz a magica
			fase.inimigo++; // Muda a posição do ponteiro de memoria
			fase.mapa.matriz[i][j].isInimigo = true; // Seta verdadeiro
		}
		else if (fase.mapa.matriz[i][j].isBloqueado == false || fase.mapa.matriz[i][j].isInimigo == false) { // Se já tive pedra ou inimigo vai sortear novamente ou se for a posiçaõ inicial do jogador
			iterador -= 1;
		}
	}
	fase.mapa.matriz[0][0].isBloqueado = false; // Posição incial não pode ser inimigo nem pedra, então não vai inimigo se tiver uma pedra, aqui seto pra não ser pedra
	fase.mapa.matriz[fase.mapa.altura - 1][fase.mapa.largura - 1].isBloqueado = false; // Posição final não pode ser inimigo nem pedra, então não vai inimigo se tiver uma pedra, aqui seto pra não ser pedra
	fase.mapa.matriz[fase.mapa.altura - 1][fase.mapa.largura - 1].isSaida = true;
	return fase;
}

template <typename T1, typename T2>
void Atacar(T1 & atacante, T2 & atacado) {
	atacado.vida = atacado.vida - (atacante.arma.danoMinimo + (abs(rand()) % (atacante.arma.danoMaximo - atacante.arma.danoMinimo + 1))); // Calculo do dano
	cout << "\n O " << atacante.nome << " atacou o " << atacado.nome << " ficando com " << atacado.vida << " de vida"; // Imprimir mensagem bonita
}

template <typename T>
bool taMorto(T morto) {
	if (morto.vida <= 0) {
		cout << "\n" << morto.nome << " morreu\n" << endl;
		return true;
	}
	return false;
}

Jogador combater(Fase fase, Jogador jogador) { // função jogar fase
	bool jogadorMorto = false;
	bool inimigoMorto = false;

	while (jogadorMorto == false && inimigoMorto == false) { // Acaba se um dos dois morrer
		Atacar(jogador, *fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco); // chama a função e coloca como parametro o jogador e o bloco do inimigo
		Atacar(*fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco, jogador); // chama a função e coloca como parametro o bloco do inimigo e o jogador
		inimigoMorto = taMorto(*fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco);
		jogadorMorto = taMorto(jogador);
	}
	if (jogadorMorto) {
		cout << "\n Você morreu!\n";
		return jogador;
	}
	fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isInimigo = false;
	fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco = nullptr; // remove referencia do inimigo do bloco
	cout << "\n Parabéns, você matou o inimigo!\n";
	return jogador;

}

void movimentar(Fase fase, Jogador jogador) {
	while (jogador.vida > 0 && fase.numeroInimigos > 0 || jogador.posicaoX != fase.mapa.altura - 1 || jogador.posicaoX != fase.mapa.largura - 1) {
		cout << "Entrando em " << fase.nome << ", o(a)" << jogador.nome << " terá que derrorar todo os inimigos para sair ou irá morrer! \n";
		imprimirMapa(fase, jogador);

		cout << "\n Tecle W para ir para cima\n Tecle A para ir para esquerda\n Tecle S para ir para baixo\n Tecle D para ir para direira \n Tecla: ";
		char tecla = 0; // Declaração para uma variável para pegar a tecla
		cin >> tecla; // Pegar a letra do teclado para fazer a movimentação

		if (tecla == 'W' || tecla == 'w') {
			jogador.posicaoY--; // Diminiu em 1 o numero da linha, fazendo subir

		}
		else if (tecla == 'A' || tecla == 'a') { // Aumenta em 1 o numero da linha, fazendo descer
			jogador.posicaoX--;

		}
		else if (tecla == 'S' || tecla == 's') { // Diminiu em 1 o numero da coluna, fazendo ir para esquerda
			jogador.posicaoY++;

		}
		else if (tecla == 'D' || tecla == 'd') { // Aumenta em 1 o numero da coluna, fazendo ir para direira
			jogador.posicaoX++;

		}

		if (jogador.posicaoX < 0) { // Condicional para não deixar sair do mapa
			cout << "\n Movimento inválido!";
			jogador.posicaoX++; // Aumenta a posição da coluna para voltar pro mapa
		}
		else if (jogador.posicaoY < 0) {
			cout << "\n Movimento inválido!";
			jogador.posicaoY++; // Aumenta a posição da linha para voltar para o mapa
		}
		else if (jogador.posicaoX > fase.mapa.largura - 1) {
			cout << "\n Movimento inválido!"; // Diminiu a posição da coluna para voltar para o mapa
			jogador.posicaoX--;
		}
		else if (jogador.posicaoY > fase.mapa.altura - 1) {
			cout << "\n Movimento inválido!";
			jogador.posicaoY--; // Diminui a posição da linha para voltar pro mapa
		}

		cout << "\n ";

		if (fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isBloqueado) {
			cout << "\n\n Não pode";

			if (tecla == 'W' || tecla == 'w') {
				jogador.posicaoY++; // Diminiu em 1 o numero da linha, fazendo subir

			}
			else if (tecla == 'A' || tecla == 'a') { // Aumenta em 1 o numero da linha, fazendo descer
				jogador.posicaoX++;

			}
			else if (tecla == 'S' || tecla == 's') { // Diminiu em 1 o numero da coluna, fazendo ir para esquerda
				jogador.posicaoY--;

			}
			else if (tecla == 'D' || tecla == 'd') { // Aumenta em 1 o numero da coluna, fazendo ir para direira
				jogador.posicaoX--;

			}
		}
		else if (fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isInimigo) {
			jogador = combater(fase, jogador); // a vida do jogador retornar 0, acaba
			fase.numeroInimigos--; // Se o jogo não acabou, então ele derrotou logo terá que enfretar um a menos
		}
		else {
			cout << "\n Caminhou \n";
		}

		cout << "\n\n ";

		PAUSE("Pressione entre para continuar...");
		CLEAR();
	}

	if (jogador.vida <= 0) {
		cout << "\n Você morreu!!! \n ";
	}
	else {
		cout << "\n Parabéns, você venceu \n\n ";
	}
	PAUSE("Pressione entre para continuar...");
}

void inicializador() {
	Jogador personagemJogador; // Declaração da struct do jogador

	cout << "\n Escreva o nome do seu personagem: ";
	cin >> personagemJogador.nome; // nome do jogador

								   // personagemJogador.nome = "Kratos";
	int alturaMapa = 0; // Declaração da altura
	int larguraMapa = 0; // Declaração da largura

	cout << "\n Digite o tamanho do mapa ";

	while (alturaMapa <= 1) {
		cout << "\n Altura, tem que ser maior que 1: ";
		cin >> alturaMapa; // Inserir a altura
	}

	while (larguraMapa <= 1) {
		cout << "\n Largura, tem que ser maior que 1: ";
		cin >> larguraMapa; // Inserir a largura
	}

	int numeroInimigos = 0; // Declaração da variável do numero de inimigos

	while (numeroInimigos <= 0 || numeroInimigos > (alturaMapa * larguraMapa * 0.8)) { // 5 inimigos personalizados
		cout << "\n Digite o número de inimigos, que ocupe até 80%(por cento) do espaço do mapa: ";
		cin >> numeroInimigos; // Inserção do número de inimigos
	}

	int numeroFase = 1;
	while (true) { // Looping para selecionar uma fase válida
		cout << "\n Escolha uma fase \n 1- Bar \n 2- Floresta \n Escolha:";
		cin >> numeroFase;
		if (numeroFase <= 2) {
			break;
		}
		cout << "\n Opção de fase inválida, escolha novamente!\n ";
		PAUSE("Pressione entre para continuar...");
		CLEAR();
	}

	personagemJogador.vida = numeroInimigos * 40;

	CLEAR();
	Inimigo* inimigo = criarInimigos(numeroInimigos, numeroFase);

	bool faseNaoJogavel = true;

	Fase fase;
	NPC npc;

	do {
		npc = NPC(0);
		fase = criarFase(numeroInimigos, inimigo, numeroFase, alturaMapa, larguraMapa); // Declaração e criação da fase pela função

	} while (!naoEhJogavel(&npc, fase));


	movimentar(fase, personagemJogador);

	cout << "\n\nFim";

	/*for (int i = 0; i < fase.mapa.largura; i++) {
		delete[] fase.mapa.matriz[i];
	}
	delete[] fase.mapa.matriz;
	delete[] fase.inimigo;
	fase.mapa.matriz = nullptr;*/
}

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");

	int opcaoJogo = 0;
	bool is_running = true;
	while (is_running)
	{
		opcaoJogo = 1;
		cout << "Escolha uma opção:";
		cout << "\n 1- Jogar \n 2- Informações \n 3- Sair \n Opção: ";
		cin >> opcaoJogo;


		switch (opcaoJogo) { // Switch da escolha do que fazer
		case 1: // Jogo
			inicializador();
			break;
		case 2: // Informações sobre os alunos
			cout << "\n Informações:\n";
			cout << " Jogo feito por Eduardo Savian de Oliveira, Joao Filipe Gohr e Miguel Luiz Dalpra Pereira\n\n";
			break;
		case 3: // Sai do jogo
			cout << "\n Saiu do jogo! \n";
			is_running = false;
			break;
		default: // Opção inválida que pede uma novo digito
			cout << "\n Opção inválida, escolha outra opção!\n";
			break;
		}
	}
	return 0;
}
