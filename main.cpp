#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

HINSTANCE hInst;

//Jorge Guillermo Olarte Quispe

#define MAX_VERTICES 20
int graph[MAX_VERTICES][MAX_VERTICES];
int numVertices;
int colors[MAX_VERTICES];
COLORREF colorArray[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255) };

int centerX[MAX_VERTICES];
int centerY[MAX_VERTICES];
int radius = 34;

BOOL isDragging = FALSE;
int dragIndex = -1;
POINT dragOffset;

void InitializeGraph() {
    numVertices = 11;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
        }
    }

//    graph[0][1] = graph[1][0] = 1;
//    graph[1][2] = graph[2][1] = 1;
//    graph[1][3] = graph[3][1] = 1;
//    graph[2][4] = graph[4][2] = 1;
//    graph[3][4] = graph[4][3] = 1;
//    graph[2][5] = graph[5][2] = 1;
//    graph[3][5] = graph[5][3] = 1;
//    graph[4][5] = graph[5][4] = 1;
//    graph[7][4] = graph[4][7] = 1;
//    graph[7][5] = graph[5][7] = 1;
//    graph[7][6] = graph[6][7] = 1;
//    graph[6][4] = graph[4][6] = 1;
//    //mas 1 color
//    graph[3][2] = graph[2][3] = 1;
//    //mas 1 color
//
//    graph[1][8] = graph[8][1] = 1;
//    graph[3][8] = graph[8][3] = 1;
//    graph[5][8] = graph[8][5] = 1;
//    graph[6][8] = graph[8][6] = 1;
//    graph[7][8] = graph[8][7] = 1;
//    graph[0][8] = graph[8][0] = 1;
//    graph[1][7] = graph[7][1] = 1;
//    graph[1][6] = graph[6][1] = 1;
//    graph[3][7] = graph[7][3] = 1;
//    graph[1][5] = graph[5][1] = 1;



//    a 0
    graph[0][1] = graph[1][0] = 1; //b 1
    graph[0][2] = graph[2][0] = 1; //c 2
    graph[0][3] = graph[3][0] = 1; //d 3
    graph[0][4] = graph[4][0] = 1; //e 4
    //f 5

    graph[0][6] = graph[6][0] = 1; //g 6
    graph[0][7] = graph[7][0] = 1; //h 7
    // i 8
    // j 9
    graph[0][10] = graph[10][0] = 1; //k 10



    graph[1][10] = graph[10][1] = 1;
    graph[1][3] = graph[3][1] = 1;
    graph[1][0] = graph[0][1] = 1;
    graph[1][5] = graph[5][1] = 1;
    graph[1][8] = graph[8][1] = 1;
    graph[1][9] = graph[9][1] = 1;





    graph[2][3] = graph[3][2] = 1;
    graph[2][4] = graph[4][2] = 1;
    graph[2][0] = graph[0][2] = 1;
    graph[2][6] = graph[6][2] = 1;
    graph[2][7] = graph[7][2] = 1;


    graph[3][4] = graph[4][3] = 1;
    graph[3][1] = graph[1][3] = 1;
    graph[3][0] = graph[0][3] = 1;
    graph[3][2] = graph[2][3] = 1;

    graph[4][10] = graph[10][4] = 1;
    graph[4][0] = graph[0][4] = 1;
    graph[4][3] = graph[3][4] = 1;
    graph[4][2] = graph[2][4] = 1;

    graph[5][1] = graph[1][5] = 1;
    graph[5][6] = graph[6][5] = 1;
    graph[5][8] = graph[8][5] = 1;
    graph[5][9] = graph[9][5] = 1;


    graph[6][2] = graph[2][6] = 1;
    graph[6][7] = graph[7][6] = 1;
    graph[6][0] = graph[0][6] = 1;
    graph[6][5] = graph[5][6] = 1;

    graph[7][2] = graph[2][7] = 1;
    graph[7][0] = graph[0][7] = 1;
    graph[7][6] = graph[6][7] = 1;

    graph[8][1] = graph[1][8] = 1;
    graph[8][5] = graph[5][8] = 1;
    graph[8][9] = graph[9][8] = 1;

    graph[9][8] = graph[8][9] = 1;
    graph[9][1] = graph[1][9] = 1;
    graph[9][5] = graph[5][9] = 1;

    graph[10][0] = graph[0][10] = 1;
    graph[10][1] = graph[1][10] = 1;
    graph[10][4] = graph[4][10] = 1;

    // aleatorio para despues arrastrar
    srand((unsigned int)time(NULL));
    for (int i = 0; i < numVertices; i++) {
        centerX[i] = rand() % 300 + 50;
        centerY[i] = rand() % 200 + 50;
    }
}

void WelshPowell() {
    int degrees[MAX_VERTICES];
    int order[MAX_VERTICES];
    for (int i = 0; i < numVertices; i++) {
        degrees[i] = 0;
        for (int j = 0; j < numVertices; j++) {
            if (graph[i][j]) degrees[i]++;
        }
        order[i] = i;
    }
    for (int i = 0; i < numVertices - 1; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (degrees[order[i]] < degrees[order[j]]) {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    for (int i = 0; i < numVertices; i++) {
        colors[i] = -1;// sin color
    }

    int currentColor = 0;
    for (int i = 0; i < numVertices; i++) {
        int u = order[i];
        if (colors[u] == -1) {
            colors[u] = currentColor;
            for (int j = i + 1; j < numVertices; j++) {
                int v = order[j];
                if (colors[v] == -1) {
                    int canColor = 1;
                    for (int k = 0; k < numVertices; k++) {
                        if (graph[v][k] && colors[k] == currentColor) {
                            canColor = 0;
                            break;
                        }
                    }
                    if (canColor) {
                        colors[v] = currentColor;
                    }
                }
            }
            currentColor++;
        }
    }
}


int minDegreeVertex(int degrees[], int colored[]) {
    int minDegree = MAX_VERTICES;
    int minVertex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (!colored[i] && degrees[i] < minDegree) {
            minDegree = degrees[i];
            minVertex = i;
        }
    }
    return minVertex;
}

void MatulaMarbleIsaacson() {
    int degrees[MAX_VERTICES];
    int colored[MAX_VERTICES] = {0};
    for (int i = 0; i < numVertices; i++) {
        degrees[i] = 0;
        for (int j = 0; j < numVertices; j++) {
            if (graph[i][j]) degrees[i]++;
        }
    }

    int currentColor = 0;
    for (int i = 0; i < numVertices; i++) {
        int u = minDegreeVertex(degrees, colored);
        if (u == -1) break;

        colors[u] = currentColor;
        colored[u] = 1;
        for (int j = 0; j < numVertices; j++) {
            if (graph[u][j]) degrees[j]--;
        }

        for (int j = 0; j < numVertices; j++) {
            if (!colored[j] && !graph[u][j]) {
                int canColor = 1;
                for (int k = 0; k < numVertices; k++) {
                    if (graph[j][k] && colors[k] == currentColor) {
                        canColor = 0;
                        break;
                    }
                }
                if (canColor) {
                    colors[j] = currentColor;
                    colored[j] = 1;
                    for (int k = 0; k < numVertices; k++) {
                        if (graph[j][k]) degrees[k]--;
                    }
                }
            }
        }

        currentColor++;
    }
}




void GreedyColoring() {
    for (int i = 0; i < numVertices; i++) {
        colors[i] = -1;
    }

    colors[0] = 0;

    bool available[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        available[i] = true;
    }

    for (int u = 1; u < numVertices; u++) {
        for (int i = 0; i < numVertices; i++) {
            if (graph[u][i] && colors[i] != -1) {
                available[colors[i]] = false;
            }
        }

        int cr;
        for (cr = 0; cr < MAX_VERTICES; cr++) {
            if (available[cr]) {
                break;
            }
        }

        colors[u] = cr;

        for (int i = 0; i < numVertices; i++) {
            if (graph[u][i] && colors[i] != -1) {
                available[colors[i]] = true;
            }
        }
    }
}

void DrawGraph(HDC hdc) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (graph[i][j]) {
                MoveToEx(hdc, centerX[i], centerY[i], NULL);
                LineTo(hdc, centerX[j], centerY[j]);
            }
        }
    }

    for (int i = 0; i < numVertices; i++) {
        HBRUSH hBrush = CreateSolidBrush(colorArray[colors[i]]);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Ellipse(hdc, centerX[i] - radius, centerY[i] - radius, centerX[i] + radius, centerY[i] + radius);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);

        char buffer[3];
        sprintf(buffer, "%d", i);
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, centerX[i] - 5, centerY[i] - 10, buffer, strlen(buffer));
    }
}


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_INITDIALOG: {
            InitializeGraph();
//            MatulaMarbleIsaacson();
            GreedyColoring();
//            WelshPowell();
            char buffer[256];
            for (int i = 0; i < numVertices; i++) {
                sprintf(buffer, "Vertex %d -> Color %d\n", i, colors[i]);
                SendDlgItemMessage(hwndDlg, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer);
            }
        }
        return TRUE;

        case WM_CLOSE: {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case IDOK:
                    EndDialog(hwndDlg, 0);
                break;
            }
        }
        return TRUE;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwndDlg, &ps);
            DrawGraph(hdc);
            EndPaint(hwndDlg, &ps);
        }
        return TRUE;

        case WM_LBUTTONDOWN: {
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            for (int i = 0; i < numVertices; i++) {
                if ((pt.x - centerX[i]) * (pt.x - centerX[i]) + (pt.y - centerY[i]) * (pt.y - centerY[i]) <= radius * radius) {
                    isDragging = TRUE;
                    dragIndex = i;
                    dragOffset.x = pt.x - centerX[i];
                    dragOffset.y = pt.y - centerY[i];
                    SetCapture(hwndDlg);
                    break;
                }
            }
        }
        return TRUE;

        case WM_MOUSEMOVE: {
            if (isDragging) {
                POINT pt;
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                centerX[dragIndex] = pt.x - dragOffset.x;
                centerY[dragIndex] = pt.y - dragOffset.y;
                InvalidateRect(hwndDlg, NULL, TRUE);
            }
        }
        return TRUE;

        case WM_LBUTTONUP: {
            if (isDragging) {
                isDragging = FALSE;
                ReleaseCapture();
            }
        }
        return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    hInst = hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}



