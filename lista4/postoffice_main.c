#include "weighted_median.h"

int main() {
    // grid    
    double x_coords[] = {1.0, 9.0};
    double y_coords[] = {2.0, 6.0};
    double pesos[]    = {0.4, 0.6};
    
    int n = sizeof(x_coords) / sizeof(x_coords[0]);

    // como a distancia de Manhattan (L1) e separavel, calculamos a mediana ponderada
    // de forma independente para o eixo X e para o eixo Y.
    double x_otimo = weighted_median(x_coords, pesos, n);
    double y_otimo = weighted_median(y_coords, pesos, n);

    // a distancia dos outros pontos ate ele, eh a menor possivel
    printf("Ponto otimo para minimizar a norma L1: (%.2f, %.2f)\n", x_otimo, y_otimo);

    return 0;
}