//Estratégia de dsconto com padrōes GOF

// Enzo Bacelar Conte Gebauer

// Strategy Interface
class DescontoStrategy {
	calcularDesconto(valor) {
		throw new Error('Método "calcularDesconto" deve ser implementado');
	}
}

// Implementações concretas das estratégias de desconto
class DescontoNatal extends DescontoStrategy {
	calcularDesconto(valor) {
		return valor * 0.25; // 25% de desconto
	}
}

class DescontoAnoNovo extends DescontoStrategy {
	calcularDesconto(valor) {
		return valor * 0.3; // 30% de desconto
	}
}

// Singleton para a fábrica de estratégias de desconto
class DescontoFactory {
	constructor() {
		if (DescontoFactory.instance) {
			return DescontoFactory.instance;
		}
		DescontoFactory.instance = this;
	}

	criarDesconto(tipo) {
		switch (tipo) {
			case "Natal":
				return new DescontoNatal();
			case "AnoNovo":
				return new DescontoAnoNovo();
			default:
				throw new Error("Tipo de desconto inválido");
		}
	}
}

// Classe de Venda que utiliza uma estratégia de desconto
class Venda {
	constructor(desconto) {
		this.desconto = desconto;
	}

	aplicarDesconto(valor) {
		return this.desconto.calcularDesconto(valor);
	}
}

// Exemplo de uso
function main() {
	try {
		const factory = new DescontoFactory();

		// Criando uma venda com desconto de Natal
		const descontoNatal = factory.criarDesconto("Natal");
		const vendaNatal = new Venda(descontoNatal);
		console.log(`Desconto de Natal: ${vendaNatal.aplicarDesconto(1000)}`);

		// Criando uma venda com desconto de Ano Novo
		const descontoAnoNovo = factory.criarDesconto("AnoNovo");
		const vendaAnoNovo = new Venda(descontoAnoNovo);
		console.log(`Desconto de Ano Novo: ${vendaAnoNovo.aplicarDesconto(1000)}`);
	} catch (error) {
		console.error(`Erro: ${error.message}`);
	}
}

main();
