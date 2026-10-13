#include "weighted_median.h"

int main() {
    // pontos dispersos no plano (x, y) com seus pesos
    // os pesos somam 1
    double x[] = {1.0,  3.0,  4.0,  6.0,  7.0,  9.0, 10.0};
    double y[] = {8.0,  2.0,  6.0,  1.0,  9.0,  4.0,  7.0};
    double pesos[]    = {0.12, 0.18, 0.15, 0.08, 0.22, 0.10, 0.15};
    
    // quantidade de pontos
    int n = sizeof(x) / sizeof(double);

    // como a distancia de Manhattan (L1) e separavel, calculamos a mediana ponderada
    // de forma independente para o eixo X e para o eixo Y.
    double x_otimo = weighted_median(x, pesos, n);
    double y_otimo = weighted_median(y, pesos, n);

    // a distancia dos outros pontos ate ele, tem o menor custo possivel
    printf("Ponto otimo: (%.2f, %.2f)\n", x_otimo, y_otimo);

    return 0;
}