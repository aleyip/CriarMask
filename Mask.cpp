// Mask.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <direct.h>
#include <filesystem>
#include <stack>

struct point {
    int x;
    int y;

    point(int xi, int yi) : x(xi), y(yi) {};
};

namespace fs = std::filesystem;
int main(int argc, char *argv[])
{
    if (argc < 7) {
        std::cout << "Programa para aplicar threshold em todos os arquivos de uma pasta de entrada e salvar em uma pasta de saida, os arquivos de saida serao salvos com o mesmo nome dos de entrada." << std::endl;
        std::cout << "Numeros de argumentos incorretos.\n Necessita dos seguintes argumentos:\n"<<
            "1- Endereco da pasta de entrada\n"<<
            "2- Endereco da pasta de saida\n"<<
            "3- Largura da imagem de saida\n"<<
            "4- Altura da imagem de saida\n"
            "5- Valor do threshold de 0 a 255\n"<<
            "6- Preenche buracos no meio da imagem, 1 para aplicar, 0 para nao.\n" << std::endl;
        return 0;
    }

    std::string pathOut = std::string(argv[2]);
    std::string pathIn = std::string(argv[1]);

    if (pathOut.compare(pathIn) == 0) {
        std::cout << "Nome da pasta de entrada e de saida nao deve ser o mesmo." << std::endl;
        return 0;
    }

    fs::create_directory(pathOut);

    for (const auto& entry : fs::directory_iterator(pathIn)) {
        std::string fileName = entry.path().filename().string();
        std::cout << fileName << std::endl;
        cv::Mat img = cv::imread(pathIn + '\\' + fileName);

        switch (img.channels()) {
        case 3:
            cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
            break;
        case 4:
            cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);
            break;
        }

        cv::threshold(img, img, atoll(argv[5]), 255, cv::THRESH_BINARY_INV);

        cv::Mat aux = cv::Mat(img.size(), CV_8UC1, cv::Scalar(255));
        if (atoll(argv[6]) == 1) {
            //fill(img, aux, 0, 0);
            std::stack<point> pilha;
            pilha.push(point(0, 0));
            pilha.push(point(img.cols - 1, 0));
            pilha.push(point(img.cols - 1, img.rows - 1));
            pilha.push(point(0, img.rows - 1));
            while (!pilha.empty()) {
                point p = pilha.top();
                pilha.pop();
                //printf("%d %d", p.x, p.y);
                //printf(" %d %d\n", aux.at<uchar>(p.y, p.x), img.at<uchar>(p.y, p.x));
                if (p.x < 0 || p.x >= img.cols || p.y < 0 || p.y >= img.rows) continue;
                if (aux.at<uchar>(p.y, p.x) == 0 || img.at<uchar>(p.y, p.x) == 255) continue;
                aux.at<uchar>(p.y, p.x) = 0;
                pilha.push(point(p.x - 1, p.y));
                pilha.push(point(p.x + 1, p.y));
                pilha.push(point(p.x, p.y - 1));
                pilha.push(point(p.x, p.y + 1));
            }
        }
        else aux = img;

        int targetWidth = atoll(argv[3]);
        int targetHeight = atoll(argv[4]);
        if (img.rows >= targetHeight && img.cols >= targetWidth)
            cv::resize(img, img, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_AREA);
        else if (img.rows >= targetHeight) {
            cv::resize(img, img, cv::Size(img.cols, targetHeight), 0, 0, cv::INTER_LINEAR);
            cv::imwrite(pathOut + '\\' + fileName + "_.jpg", img);
            cv::resize(img, img, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_AREA);
        }
        else if (img.cols >= targetWidth) {
            cv::resize(img, img, cv::Size(targetWidth, img.rows), 0, 0, cv::INTER_LINEAR);
            cv::imwrite(pathOut + '\\' + fileName + "_.jpg", img);
            cv::resize(img, img, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_AREA);
        }
        else
            cv::resize(img, img, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_LINEAR);

        cv::imwrite(pathOut + '\\' + fileName, aux);
    }
}
