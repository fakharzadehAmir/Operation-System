#include <stdio.h>

int main() {
    int size, dist_i = -1, dist_j = -1;
    scanf("%d", &size);
    int pixels[size][size], correctPic[size][size], newPic[size][size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &pixels[i][j]);
            if (pixels[i][j] == 0) {
                dist_i = i;
                dist_j = j;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (dist_j == 0 && dist_i == 0) {
                correctPic[i][j] = pixels[i][j];
            } else if (dist_j == dist_i) {
                correctPic[i][j] = pixels[dist_i - i][dist_j - j];
            } else if (dist_i < dist_j) {
                correctPic[i][j] = pixels[dist_j - j][i - dist_i];
            } else {
                correctPic[i][j] = pixels[j - dist_j][dist_i - i];
            }
        }
    }

    if (dist_i != dist_j) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                newPic[i][j] = correctPic[size - 1 - i][size - 1 - j];
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (dist_i == dist_j) {
                printf("%d ", correctPic[i][j]);
            } else {
                printf("%d ", newPic[i][j]);
            }
        }
        printf("\n");
    }

    return 0;
}
