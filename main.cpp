#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

HINSTANCE hInst;

#define MAX_VERTICES 10
int graph[MAX_VERTICES][MAX_VERTICES];
int numVertices;
int colors[MAX_VERTICES];
COLORREF colorArray[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255) };

int centerX[MAX_VERTICES];
int centerY[MAX_VERTICES];
int radius = 20;

BOOL isDragging = FALSE;
int dragIndex = -1;
POINT dragOffset;

void InitializeGraph() {
    // numero de verices
    numVertices = 9;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
        }
    }

    graph[0][1] = graph[1][0] = 1;
    graph[1][2] = graph[2][1] = 1;
    graph[1][3] = graph[3][1] = 1;
    graph[2][4] = graph[4][2] = 1;
    graph[3][4] = graph[4][3] = 1;
    graph[2][5] = graph[5][2] = 1;
    graph[3][5] = graph[5][3] = 1;
    graph[4][5] = graph[5][4] = 1;
    graph[7][4] = graph[4][7] = 1;
    graph[7][5] = graph[5][7] = 1;
    graph[7][6] = graph[6][7] = 1;
    graph[6][4] = graph[4][6] = 1;
    //mas 1 color
    graph[3][2] = graph[2][3] = 1;
    //mas 1 color

    graph[1][8] = graph[8][1] = 1;
    graph[3][8] = graph[8][3] = 1;
    graph[5][8] = graph[8][5] = 1;
    graph[6][8] = graph[8][6] = 1;
    graph[7][8] = graph[8][7] = 1;
    graph[0][8] = graph[8][0] = 1;
    graph[1][7] = graph[7][1] = 1;
    graph[1][6] = graph[6][1] = 1;
    graph[3][7] = graph[7][3] = 1;
    graph[1][5] = graph[5][1] = 1;

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
            WelshPowell();
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



