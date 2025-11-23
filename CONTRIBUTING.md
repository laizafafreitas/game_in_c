# Contribuindo no Projeto 

#### Se ainda não clonou o projeto:

```
git clone https://github.com/MGT-21/game_in_c.git
cd game_in_c
```

#### Se já tem o repo, entre na pasta e atualize a main:

```
git checkout main
git pull origin main
```

⚠️ Nunca desenvolva direto na main. Sempre crie uma branch nova.

## 1 - Criando uma branch para sua feature

⚠️ Crie uma branch a partir da main atualizada. Use um nome que descreva bem a tarefa:.

#### Trocando para a main e atualizando repo local:
```
git checkout main
git pull origin main
```

#### Criando a branch:


```
#exemplo:

git checkout -b feat/novo-golpe
```
```
#ou:

git checkout -b fix/ajuste-colisao
```

⚠️ A partir de agora, toda mudança dessa feature acontece nessa branch.

## 2 - Fazendo as alterações da feature

⚠️ Edite os arquivos em src/ e include/ conforme a necessidade da sua feature.

#### Testando as alterações:

```
make
make run
```
#### Conferindo e salvando mudanças

```
git status

```

#### Adicione os arquivos modificados (ou apenas os que fazem sentido para o commit):

```
#todos os arquivos rastreados/modificados:

git add .
```
```
#arquivos específicos:

git add src/fighters.c include/fighters.h
```

#### Faça o commit com uma mensagem clara:

```
git commit -m "feat: adiciona novo golpe especial ao player"
# ou
git commit -m "fix: corrige detecção de colisão perto da borda"
```
Caso precise entender o padrão: <a href="https://github.com/iuricode/padroes-de-commits"> Clique aqui </a>

## Enviando a branch para o GitHub

#### Envie sua branch para o repositório remoto:

```
git push -u origin feat/nomeDaSuaBranch

```

## Abra uma Pull Request (PR)

Vá até o repositório no GitHub:
https://github.com/MGT-21/game_in_c

O GitHub deve sugerir a branch nova para abrir um Pull Request.

#### Crie o PR com:

- Título explicando a feature.

- Descrição breve do que foi feito (ex.: “Adiciona golpe especial, ajusta dano e atualiza renderização do ataque”).

- Solicite revisão de alguém do grupo.

- Após aprovado, faça o merge na main.

#### Depois do merge

Depois que o PR for aceito e integrado na main:

Atualize seu repositório local:

```
git checkout main
git pull origin main
```

(Opcional) Apague a branch local que já foi usada:

```
git branch -d feat/novo-golpe

```

Se quiser, também apague a branch remota pelo GitHub ou:

```
git push origin --delete feat/novo-golpe
```
