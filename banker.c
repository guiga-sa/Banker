#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void updateNeed(int **need, int **allocated, int **max, int qtdLinhas, int qtdArgumentos){
    for(int i = 0; i < qtdLinhas; i++){
        for(int j = 0; j < qtdArgumentos; j++){
            need[i][j] = max[i][j] - allocated[i][j];
        }
    }
}
int isSafeState(int **need, int *available, int qtdLinhas, int qtdArgumentos) {
    int *work = (int *)malloc(qtdArgumentos * sizeof(int));
    for (int i = 0; i < qtdArgumentos; i++) {
        work[i] = available[i];
    }

    int *finish = (int *)malloc(qtdLinhas * sizeof(int));
    for (int i = 0; i < qtdLinhas; i++) {
        finish[i] = 0;
    }

    for (int i = 0; i < qtdLinhas; i++) {
        if (finish[i] == 0) {
            int canAllocate = 1;
            for (int j = 0; j < qtdArgumentos; j++) {
                if (need[i][j] > work[j]) {
                    canAllocate = 0;
                    break;
                }
            }
            if (canAllocate) {
                for (int j = 0; j < qtdArgumentos; j++) {
                    work[j] += need[i][j];
                }
                finish[i] = 1;
                i = -1;
            }
        }
    }

    for (int i = 0; i < qtdLinhas; i++) {
        if (finish[i] == 0) {
            free(work);
            free(finish);
            return 0;
        }
    }

    free(work);
    free(finish);
    return 1;
}

int main(int argc, char *argv[]) {
    //crie uma variavel que contem a quantidade argumento passados na linha de comando
    int qtdArgumentos = argc -1;
    //crie um vetor de inteiros que contem os argumentos passados na linha de comando
    int Recursos[qtdArgumentos];
    for(int i = 0; i < qtdArgumentos; i++){
        Recursos[i] = atoi(argv[i+1]);
    }
    // Abrir o arquivo customers.txt para leitura
    FILE *commandsFile = fopen("commands.txt", "r");
    if (commandsFile == NULL) {
        printf("Fail to read commands.txt\n");
        return 1;
    }
   
    FILE *customersFile = fopen("customer.txt", "r");
    if (customersFile == NULL) {
        printf("Fail to read customer.txt\n");
        return 1;
    }

    FILE *resultFile = fopen("result.txt", "w");

    float qtdLinhas = 0;
    float qtdNumeros = 0;
    char line1[5000];
    while(fgets(line1, sizeof(line1), customersFile)){
        qtdLinhas++;
        char *token = strtok(line1, ",");
        for(int i = 0; token != NULL; i++){
            qtdNumeros++;
            token = strtok(NULL, ",");
        }
    }
    qtdNumeros = qtdNumeros/qtdLinhas;

   float qtdArgumentosFloat = qtdArgumentos;
    if(qtdNumeros != qtdArgumentosFloat){
        printf("Incompatibility between customer.txt and command line\n");
        fclose(customersFile);
        fclose(resultFile);
        return 1;
    }
   
    int MAX_ROWS = qtdLinhas;
    int MAX_COLS = qtdArgumentos;
    //crie uma matriz 2d que armazena os valores do arquivo customers.txt
    customersFile = fopen("customer.txt", "r");
    if (customersFile == NULL) {
        printf("Fail to read customer.txt\n");
        return 1;
    }

    // Alocar memória para a matriz
    
    int **matrix = (int **)malloc(qtdLinhas * sizeof(int *));
    for (int i = 0; i < qtdLinhas; i++) {
        matrix[i] = (int *)malloc(MAX_COLS * sizeof(int));
    }

    // Ler o arquivo customers.txt e armazenar os valores na matriz
    char line[5000];
    for (int i = 0; i < qtdLinhas; i++) {
        if (fgets(line, sizeof(line), customersFile)) {
            char *token = strtok(line, ",");
            for (int j = 0; token != NULL; j++) {
                matrix[i][j] = atoi(token);
                token = strtok(NULL, ",");
            }
        }
    } 
    //percorra a matriz matrix e salve o maior valo encntrado em uma variavel
    int maiorValor = 0;
    for(int i = 0; i < qtdLinhas; i++){
        for(int j = 0; j < qtdArgumentos; j++){
            if(matrix[i][j] > maiorValor){
                maiorValor = matrix[i][j];
            }
        }
    }

    //crie uma matirz do mesmo tamanho da matriz criada anteriormente e inicie ela com 0, nomeie ela de allocated
    int **allocated = (int **)malloc(qtdLinhas * sizeof(int *));
    for (int i = 0; i < qtdLinhas; i++) {
        allocated[i] = (int *)malloc(MAX_COLS * sizeof(int));
    }
    for (int i = 0; i < qtdLinhas; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            allocated[i][j] = 0;
        }
    }

    //crie uma matirz do mesmo tamanho da matriz criada anteriormente e inicie ela com os mesmo valores da matriz incial, nomeie ela de need
    int **need = (int **)malloc(qtdLinhas * sizeof(int *));
    for (int i = 0; i < qtdLinhas; i++) {
        need[i] = (int *)malloc(MAX_COLS * sizeof(int));
    }
    for (int i = 0; i < qtdLinhas; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            need[i][j] = matrix[i][j];
        }
    }
    updateNeed(need, allocated, matrix, qtdLinhas, qtdArgumentos);


        int contCommands = -1; 
        int flagCommands = 0;
        int contador_qualquer = 0;
        fseek(commandsFile, 0, SEEK_SET);
        char ch;
        while ((ch = fgetc(commandsFile)) != EOF) {

            if(ch == '*'){
                contCommands = qtdArgumentos;//pra nao dar erro
            }else if (ch == ' '){
                contCommands ++;
            } else if (ch == '\n'|| feof(commandsFile)) {
                if(contCommands>qtdArgumentos){
                    flagCommands = 1;
                    if(flagCommands == 1){
                        printf("Incompatibility between commands.txt and command line\n");
                        fclose(commandsFile);
                        return 1;
                    }
                }else if(contCommands<qtdArgumentos){
                    flagCommands = 1;
                    if(flagCommands == 1){
                        printf("Incompatibility between commands.txt and command line\n");
                        fclose(commandsFile);
                        return 1;
                    }
                }
                contCommands = -1; 
            }
        }

    
    fseek(commandsFile, 0, SEEK_SET);//fseek volta o cursor pra cima
    //percorra o arquivo commands.txt e execute os comandos
    char line2[5000];
    while(fgets(line2, sizeof(line2), commandsFile)){
        int erroEncontrado = 0;
        char *token = strtok(line2, " ");
        if(strcmp(token, "RQ") == 0){
            int cliente = atoi(strtok(NULL, " "));
            int recursos[qtdArgumentos];
            for(int i = 0; i < qtdArgumentos; i++){
                recursos[i] = atoi(strtok(NULL, " "));
            }
            //verifique se o cliente existe
            if(cliente >= qtdLinhas){
                fprintf(resultFile, "The customer %d does not exist\n", cliente);
            }
            //verifique se o cliente tem recursos disponiveis
            int temRecursos = 1;
            for(int i = 0; i < qtdArgumentos; i++){
                if(recursos[i] > need[cliente][i]){
                    temRecursos = 0;
                }
            }
            if(temRecursos == 0){
                fprintf(resultFile, "The customer %d request ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "was denied because exceed its maximum need\n");
                erroEncontrado = 1;
            }
            //verifique se o cliente pode ser alocado
            int podeAlocar = 1;
            for(int i = 0; i < qtdArgumentos; i++){
                if(recursos[i] > Recursos[i]){
                    podeAlocar = 0;
                }
            }

           if(podeAlocar == 0 && erroEncontrado == 0){
                fprintf(resultFile, "The resources ");
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", Recursos[i]); // Assuming Recursos[] holds the available resources
                }
                fprintf(resultFile, "are not enough to customer %d request ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "\n");
                erroEncontrado = 1;
            }
            //verifique se o sistema esta em um estado seguro
            int estadoSeguro = isSafeState(need, Recursos, qtdLinhas, qtdArgumentos);
            if(estadoSeguro == 0 && erroEncontrado == 0){
                fprintf(resultFile, "The customer %d request ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "was denied because the system is in an unsafe state\n");
                erroEncontrado = 1;
            }

            //caso o cliente possa ser alocado, aloque os recursos para ele
            if(temRecursos == 1 && podeAlocar == 1 && estadoSeguro == 1 && erroEncontrado == 0){
                for(int i = 0; i < qtdArgumentos; i++){
                    Recursos[i] -= recursos[i];
                    allocated[cliente][i] += recursos[i];
                    need[cliente][i] -= recursos[i];
                }
                fprintf(resultFile, "Allocate to customer %d the resources ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "\n");
            }
        }
        else if(strcmp(token, "RL") == 0){
            int cliente = atoi(strtok(NULL, " "));
            int recursos[qtdArgumentos];
            for(int i = 0; i < qtdArgumentos; i++){
                recursos[i] = atoi(strtok(NULL, " "));
            }
            //verifique se o cliente existe
            if(cliente >= qtdLinhas){
                fprintf(resultFile, "The customer %d does not exist\n", cliente);
            }
            //verifique se o cliente tem recursos alocados
            int temRecursos = 1;
            for(int i = 0; i < qtdArgumentos; i++){
                if(recursos[i] > allocated[cliente][i]){
                    temRecursos = 0;
                }
            }
            if(temRecursos == 0){
                fprintf(resultFile, "The customer %d release ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "was denied because exceed its maximum allocation\n");
            }
            //caso o cliente possa ser liberado, libere os recursos dele
            if(temRecursos == 1){
                for(int i = 0; i < qtdArgumentos; i++){
                    Recursos[i] += recursos[i];
                    allocated[cliente][i] -= recursos[i];
                    need[cliente][i] += recursos[i];
                }
                fprintf(resultFile, "Release from customer %d the resources ", cliente);
                for(int i = 0; i < qtdArgumentos; i++){
                    fprintf(resultFile, "%d ", recursos[i]);
                }
                fprintf(resultFile, "\n");
            }
        }
        else if(strcmp(token, "*\n") == 0|| strcmp(token, "*") == 0){

        if(maiorValor<10){
            if(qtdArgumentos<5){
                fprintf(resultFile, "MAXIMUM |");
            }else if(qtdArgumentos>=5){
                fprintf(resultFile, "MAXIMUM ");
                for(int i = 0; i <=qtdArgumentos-5; i++){
                    fprintf(resultFile, "  ");
                }
                fprintf(resultFile, "|");
            }
            if(qtdArgumentos<6){
                fprintf(resultFile, " ALLOCATION |");
            }else if(qtdArgumentos==6){
                fprintf(resultFile, " ALLOCATION  |");
            }else if(qtdArgumentos>6){
                fprintf(resultFile, " ALLOCATION");
                for(int i = 0; i <=qtdArgumentos-6; i++){
                    fprintf(resultFile, "  ");
                }
                fprintf(resultFile, "|");
            }
            fprintf(resultFile, " NEED\n");
            for(int i = 0; i < qtdLinhas; i++){
                for(int j = 0; j < qtdArgumentos; j++){
                    fprintf(resultFile, "%d ", matrix[i][j]);
                }
            if(qtdArgumentos==1){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "    |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "  |");
                }else if(qtdArgumentos>=4){
                    fprintf(resultFile, "|");
                }
            
                for(int j = 0; j < qtdArgumentos; j++){
                    //se for o primeiro numero da linha, adicione um espaço vazio antes do |
                    if(j == 0){
                        fprintf(resultFile, " ");
                    }
                    fprintf(resultFile, "%d ", allocated[i][j]);
                }
                if(qtdArgumentos==1){
                    fprintf(resultFile, "         |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "       |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==4){
                    fprintf(resultFile, "   |");
                }else if(qtdArgumentos==5){
                    fprintf(resultFile, " |");
                }else if(qtdArgumentos>=6){
                    fprintf(resultFile, "|");
                }
                
                for(int j =0; j < qtdArgumentos; j++){
                    if(j == 0){
                        fprintf(resultFile, " ");
                    }
                    fprintf(resultFile, "%d ", need[i][j]);
                }
                fprintf(resultFile, "\n");
            }
            fprintf(resultFile, "AVAILABLE ");
            for(int i = 0; i < qtdArgumentos; i++){
                fprintf(resultFile, "%d ", Recursos[i]);
            }
            fprintf(resultFile, "\n");
            
        }
        else if(maiorValor>=10&&maiorValor<=99){
            int maiorLinha = 0;
            for(int i = 0; i < qtdLinhas; i++){
                int tamanhoLinha = 0;
                for(int j = 0; j < qtdArgumentos; j++){
                    if(matrix[i][j]>=10&&matrix[i][j]<=99){
                        tamanhoLinha += 3;
                    }else{
                        tamanhoLinha += 2;
                    }
                }
                if(tamanhoLinha > maiorLinha){
                    maiorLinha = tamanhoLinha;
                }
            }
            if(maiorLinha<=8){
                fprintf(resultFile, "MAXIMUM |");
            }
            else if(maiorLinha>8){
                fprintf(resultFile, "MAXIMUM ");
                for(int i = 0; i <=maiorLinha-8; i++){
                    fprintf(resultFile, " ");
                }
                fprintf(resultFile, " |");
            }
            //crie um codigo que perocorre cada linha da matriz allocated e ve a maior linha, por exemplo se a linha for composta por 0 10 9, o tamanho dela é 6 e se for 0 0 0 o tamanho dela é 5 
            int maiorLinhaAllocated = 0;
            for(int i = 0; i < qtdLinhas; i++){
                int tamanhoLinha = 0;
                for(int j = 0; j < qtdArgumentos; j++){
                    if(allocated[i][j]>=10&&allocated[i][j]<=99){
                        tamanhoLinha += 3;
                    }else{
                        tamanhoLinha += 2;
                    }
                }
                if(tamanhoLinha > maiorLinhaAllocated){
                    maiorLinhaAllocated = tamanhoLinha;
                }
            }
            maiorLinhaAllocated = maiorLinhaAllocated +1;
            //printf("maior linha2: %d\n", maiorLinhaAllocated);
            if(maiorLinhaAllocated<=11){
                fprintf(resultFile, " ALLOCATION |");
            }
            else if(maiorLinhaAllocated>11){
                fprintf(resultFile, " ALLOCATION");
                for(int i = 0; i <maiorLinhaAllocated-11; i++){
                    
                    fprintf(resultFile, " ");
                }
                fprintf(resultFile, "|");
            }
            fprintf(resultFile, " NEED\n");
            
            for(int i = 0; i < qtdLinhas; i++){
                for(int j = 0; j < qtdArgumentos; j++){
                    
                    int flag = 0;
                    for(int k = 0; k < qtdLinhas; k++){
                        if(matrix[k][j]>=10&&matrix[k][j]<=99){
                            flag = 1;
                        }
                    }
                    if(flag == 1){
                        if(matrix[i][j]<10){
                            fprintf(resultFile, " ");
                        }
                    }
                    fprintf(resultFile, "%d ", matrix[i][j]);
                }
                if(qtdArgumentos==1){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "    |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "  |");
                }else if(qtdArgumentos>=4){
                    fprintf(resultFile, "|");
                }
            
                for(int j = 0; j < qtdArgumentos; j++){
                    if(j == 0){
                        fprintf(resultFile, " ");
                    }
                    int flag = 0;
                    for(int k = 0; k < qtdLinhas; k++){
                        if(allocated[k][j]>=10&&allocated[k][j]<=99){
                            flag = 1;
                        }
                    }
                    if(flag == 1){
                        if(allocated[i][j]<10){
                            fprintf(resultFile, " ");
                        }
                    }
                    fprintf(resultFile, "%d ", allocated[i][j]);
                }
                if(qtdArgumentos==1){
                    fprintf(resultFile, "         |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "       |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==4){
                    fprintf(resultFile, "   |");
                }else if(qtdArgumentos==5){
                    fprintf(resultFile, " |");
                }else if(qtdArgumentos>=6){
                    fprintf(resultFile, "|");
                }
                
                for(int j =0; j < qtdArgumentos; j++){
                    if(j == 0){
                        fprintf(resultFile, " ");
                    }
                    int flag = 0;
                    for(int k = 0; k < qtdLinhas; k++){
                        if(need[k][j]>=10&&need[k][j]<=99){
                            flag = 1;
                        }
                    }
                    if(flag == 1){
                        if(need[i][j]<10){
                            fprintf(resultFile, " ");
                        }
                    }
                    fprintf(resultFile, "%d ", need[i][j]);
                }
                fprintf(resultFile, "\n");
            }
            fprintf(resultFile, "AVAILABLE ");
            for(int i = 0; i < qtdArgumentos; i++){
                fprintf(resultFile, "%d ", Recursos[i]);
            }
            fprintf(resultFile, "\n");
        }else{
            int maiorLinha = 0;
            for(int i = 0; i < qtdLinhas; i++){
                int tamanhoLinha = 0;
                for(int j = 0; j < qtdArgumentos; j++){
                    if(matrix[i][j] >= 100 && matrix[i][j] <= 999){
                        tamanhoLinha += 4; // Increment by 4 for three-digit numbers
                    } else if(matrix[i][j] >= 10 && matrix[i][j] <= 99){
                        tamanhoLinha += 3;
                    } else {
                        tamanhoLinha += 2;
                    }
                }
                if(tamanhoLinha > maiorLinha){
                    maiorLinha = tamanhoLinha;
                }
            }
      
            if(maiorLinha<=8){
                fprintf(resultFile, "MAXIMUM |");
            }
            else if(maiorLinha>8){
                fprintf(resultFile, "MAXIMUM ");
                for(int i = 0; i <=maiorLinha-8; i++){
                    fprintf(resultFile, " ");
                }
                fprintf(resultFile, " |");
            }
            int maiorLinhaAllocated = 0;
            for(int i = 0; i < qtdLinhas; i++){
                int tamanhoLinha = 0;
                for(int j = 0; j < qtdArgumentos; j++){
                    if(allocated[i][j] >= 100 && allocated[i][j] <= 999){
                        tamanhoLinha += 4; // Increment by 4 for three-digit numbers
                    } else if(allocated[i][j] >= 10 && allocated[i][j] <= 99){
                        tamanhoLinha += 3;
                    } else {
                        tamanhoLinha += 2;
                    }
                }
                if(tamanhoLinha > maiorLinhaAllocated){
                    maiorLinhaAllocated = tamanhoLinha;
                }
            }
            maiorLinhaAllocated = maiorLinhaAllocated +1;
            if(maiorLinhaAllocated<=11){
                fprintf(resultFile, " ALLOCATION |");
            }
            else if(maiorLinhaAllocated>11){
                fprintf(resultFile, " ALLOCATION");
                for(int i = 0; i <maiorLinhaAllocated-11; i++){
                    
                    fprintf(resultFile, " ");
                }
                fprintf(resultFile, "|");
            }
            fprintf(resultFile, " NEED\n");
            
            for(int i = 0; i < qtdLinhas; i++){
                for(int j = 0; j < qtdArgumentos; j++){
                int flag = 0;
                int flag2 = 0;
                for(int k = 0; k < qtdLinhas; k++){
                    if((matrix[k][j] >= 10 && matrix[k][j] <= 99) ){
                        flag = 1;
                    }
                }
                for(int k = 0; k < qtdLinhas; k++){
                    if((matrix[k][j] >= 100 && matrix[k][j] <= 999)){
                        flag2 = 1;
                    }
                }
                if(flag2 == 1){
                    if(matrix[i][j] < 10){
                        fprintf(resultFile, "  ");
                    } else if(matrix[i][j] < 100){
                        fprintf(resultFile, " ");
                    }
                }else if(flag == 1){
                    if(matrix[i][j] < 10){
                        fprintf(resultFile, " ");
                    }
                }
                fprintf(resultFile, "%d ", matrix[i][j]);
            }
                if(qtdArgumentos==1){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "    |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "  |");
                }else if(qtdArgumentos>=4){
                    fprintf(resultFile, "|");
                }
            
            for(int j = 0; j < qtdArgumentos; j++){
                if(j == 0){
                    fprintf(resultFile, " ");
                }
                int flag = 0;
                int flag2 = 0;
                for(int k = 0; k < qtdLinhas; k++){
                    if((allocated[k][j] >= 10 && allocated[k][j] <= 99) ){
                        flag = 1;
                    }
                }
                for(int k = 0; k < qtdLinhas; k++){
                    if((allocated[k][j] >= 100 && allocated[k][j] <= 999)){
                        flag2 = 1;
                    }
                }
                if(flag2 == 1){
                    if(allocated[i][j] < 10){
                        fprintf(resultFile, "  ");
                    } else if(allocated[i][j] < 100){
                        fprintf(resultFile, " ");
                    }
                }
                if(flag == 1){
                    if(allocated[i][j] < 10){
                        fprintf(resultFile, " ");
                    }
                }
                fprintf(resultFile, "%d ", allocated[i][j]);
            }
                if(qtdArgumentos==1){
                    fprintf(resultFile, "         |");
                }else if(qtdArgumentos==2){
                    fprintf(resultFile, "       |");
                }else if(qtdArgumentos==3){
                    fprintf(resultFile, "     |");
                }else if(qtdArgumentos==4){
                    fprintf(resultFile, "   |");
                }else if(qtdArgumentos==5){
                    fprintf(resultFile, " |");
                }else if(qtdArgumentos>=6){
                    fprintf(resultFile, "|");
                }
                
            for(int j = 0; j < qtdArgumentos; j++){
                if(j == 0){
                    fprintf(resultFile, " ");
                }
                //antes de escrever o valor da matriz, percorra toda ela e verifique se na coluna possui um valor maior que 9 e menor que 100
                int flag = 0;
                int flag2 = 0;
                for(int k = 0; k < qtdLinhas; k++){
                    if((need[k][j] >= 10 && need[k][j] <= 99)){
                        flag = 1;
                    }


                }
                for(int k = 0; k < qtdLinhas; k++){
                    if((need[k][j] >= 100 && need[k][j] <= 999)){
                        flag2 = 1;
                    }
                }

                //se a flag for igual a 1, significa que na coluna possui um valor maior que 9 e menor que 100, então verifique se o valor da matriz é menor que 10, se for adicione um espaço vazio antes do valor, se não for adicione dois espaços vazios antes do valor
                if(flag2 == 1){
                    if(need[i][j] < 10){
                        fprintf(resultFile, "  ");
                    } else if(need[i][j] < 100){
                        fprintf(resultFile, " ");
                    }
                }
                if(flag == 1){
                    if(need[i][j] < 10){
                        fprintf(resultFile, " ");
                    }
                }
                fprintf(resultFile, "%d ", need[i][j]);
            }
                fprintf(resultFile, "\n");
            }
            fprintf(resultFile, "AVAILABLE ");
            for(int i = 0; i < qtdArgumentos; i++){
                fprintf(resultFile, "%d ", Recursos[i]);
            }
            fprintf(resultFile, "\n");
            
        }
    }
    else{
        fprintf(resultFile, "Invalid command\n");
        
    }

    }
    // Fechar o arquivo customers.txt
    fclose(customersFile);
    // Fechar o arquivo result.txt
    fclose(resultFile);
    // Liberar a memória alocada para a matriz
    fclose(commandsFile);

    for (int i = 0; i < qtdLinhas; i++) {
        free(matrix[i]);
    }
    //libere a memoria alocada para a matriz allocated
    for (int i = 0; i < qtdLinhas; i++) {
        free(allocated[i]);
    }
    //libere a memoria alocada para a matriz need
    for (int i = 0; i < qtdLinhas; i++) {
        free(need[i]);
    }



    return 0;
}