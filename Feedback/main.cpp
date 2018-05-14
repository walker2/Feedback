#include <iostream>
#include <random>
#include <vector>
#include <QtGui/QColor>
#include "qcustomplot.h"

void two_one();

void two_two();

void two_three_a();

void two_three_b();

void two_four();

void two_five();

void plot(std::vector<double> &x, std::vector<double> &y, QCustomPlot *plot, QColor color, int plotNum,
          const std::string &plotName) {
    QVector<double> X = QVector<double>::fromStdVector(x);
    QVector<double> Y = QVector<double>::fromStdVector(y);

    plot->resize(512, 512);
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));
    plot->addGraph();
    plot->graph(plotNum)->setName(QString::fromStdString(plotName));
    plot->graph(plotNum)->setPen(QPen(color));
    plot->graph(plotNum)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    plot->graph(plotNum)->setData(X, Y);

    plot->graph(plotNum)->rescaleAxes();
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    plot->show();
}

float AvgInWaitingAlgorithm(float p, int64_t numberOfMessages, int maxNumber = 0, float p_rcp = 0) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    int64_t tries = 0;
    int64_t sumOfTries = 0;

    bool receipt = false;
    for (int message = 0; message < numberOfMessages;) {
        // "Sending message"
        tries++;

        // Simulating error
        if (dist(rng) <= p) {
            // If was error then send "-"
            receipt = false;
        } else {
            // Simulate error in receipt
            if (dist(rng) <= p_rcp) {
                receipt = false;
            } else {
                receipt = true;
            }
        }

        // Get the receipt
        if (receipt == true || maxNumber != 0 && tries >= maxNumber) {
            // Proceed to next message
            message++;
            sumOfTries += tries;
            tries = 0;
        }
    }

    float avgNumOfTries = sumOfTries / (float) numberOfMessages;

    return avgNumOfTries;
}

float UsageInWaitingAlgorithm(float p, float tau, int64_t numberOfMessages) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    int64_t tries = 0;

    bool receipt = false;
    bool newMessage = true;
    for (int message = 1; message <= numberOfMessages;) {
        // "Sending message"
        if (newMessage) {
            //std::cout << message;
        } else {
            //std::cout << " ";
        }
        //std::cout << "|\t\t|" << std::endl;

        //for (int i = 0; i < tau - 2; i++)
        //std::cout << " |\t\t|" << std::endl;

        //std::cout << " |\t";

        // Simulating error
        if (dist(rng) <= p) {
            // If was error then send "-"
            //std::cout << "e_______|x" << std::endl;
            receipt = false;
        } else {
            //std::cout << "ok______|" << message << std::endl;
            receipt = true;
        }

        //std::cout << " |______|\t|" << std::endl;
        // Get the receipt
        if (receipt == true) {
            // Proceed to next message
            message++;
            newMessage = true;
        } else {
            newMessage = false;
        }

        tries++;
    }

    return (numberOfMessages / (float) tries) / (1 + tau);
}

float UsageInReturnAlgorithm(float p, float tau, int64_t numberOfMessages) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    int64_t tries = 0;
    int64_t cycle = 0;
    int64_t cycleSum = 0;

    bool wasError = false;
    int lostMsgNum = 0;
    int tmpMsg = 0;

    for (int message = 1;;) {
        // Simulating error
        if (wasError && lostMsgNum < tau) {
            //std::cout << message << "|\t";
            //std::cout << "~\t|x" << std::endl;
            lostMsgNum++;
        } else {
            if (wasError) {
                message = tmpMsg;
            }
            //std::cout << message << "|\t";

            if (dist(rng) <= p) {
                // If was error then send "-"
                //std::cout << "e\t|x" << std::endl;
                wasError = true;
                lostMsgNum = 0;
                tmpMsg = message;
                cycleSum += tau + 1;
            } else {
                cycleSum++;

                //std::cout << "ok\t|" << message << " T: " << cycle << std::endl;

                if (message == numberOfMessages) { break; }
                cycle = 0;
                wasError = false;
            }
        }

        message++;
        tries++;
    }

    return numberOfMessages / (float) cycleSum;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    /* 2.1 Среднее число передач в алгоритме с ожиданием при неограниченном числе повторных передач */
    two_one();

    /* 2.2 Среднее число передач в алгоритме с ожиданием при ограниченном числе повторных передач */
    two_two();

    /* 2.3 Среднее число передач в алгоритме с ожиданием при наличии ошибок в обратном канал с неогр. и огр. повт. передач*/
    two_three_a();

    two_three_b();

    /* 2.4 Определение коэффициента использования канала при τ > 0 для алгоритма с ожиданием   `*/
    two_four();

    /* 2.5 Моделирование алгоритма с возвратом */
    two_five();

    a.exec();
}

void two_five() {
    int count = 0;

    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.5");
    for (int tau = 2; tau < 6; tau++)
    {
        std::vector<double> p_vec;
        std::vector<double> usage_vec_ex;
        for (double p = 0.05; p < 1; p += 0.05)
        {
            p_vec.push_back(p);
            usage_vec_ex.push_back(UsageInReturnAlgorithm(p, tau, 100000));
        }
        std::string name = "tau = " + std::to_string(tau);
        plot(p_vec, usage_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue + count), count++, name);
    }
   /* float p5 = 0.4f;
    float tau5 = 3;
    std::cout << "Usage koeff:\t\t" << UsageInReturnAlgorithm(p5, tau5, 10) << std::endl;
    std::cout << "Should be:\t\t" << (1 - p5) / (1 + p5 * tau5) << std::endl;*/
}

void two_four() {
    int count = 0;

    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.4");
    for (int tau = 2; tau < 6; tau++)
    {
        std::vector<double> p_vec;
        std::vector<double> usage_vec_ex;
        for (double p = 0.05; p < 1; p += 0.05)
        {
            p_vec.push_back(p);
            usage_vec_ex.push_back(UsageInWaitingAlgorithm(p, tau, 100000));
        }
        std::string name = "tau = " + std::to_string(tau);
        plot(p_vec, usage_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue + count), count++, name);
    }
    /*float p4 = 0.5f;
    float tau4 = 2;

    std::cout << "Usage koeff:\t\t" << UsageInWaitingAlgorithm(p4, tau4, 9) << std::endl;
    std::cout << "Should be:\t\t" << (1 - p4) / (1 + tau4) << std::endl;*/
}

void two_three_b() {
    int n = 150;
    int count = 0;
    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.3 (b)");
    for (double p_rcp = 0.0; p_rcp <= 0.9; p_rcp += 0.3) {
        std::vector<double> p_vec;
        std::vector<double> n_vec_ex;
        std::vector<double> n_vec_th;
        for (double p = 0.05; p < 1; p += 0.05) {
            p_vec.push_back(p);
            n_vec_ex.push_back(AvgInWaitingAlgorithm(p, 100000, n, p_rcp));
            n_vec_th.push_back(1 / ((1 - p) * (1 - p_rcp)));
        }
        std::string name = "p_rcp = " + std::to_string(p_rcp) + " with n = " + std::to_string(n);
        plot(p_vec, n_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue + count), count++, name);
    }
    /*float p3_2 = 0.3f;
    float p3_2_rcp = 0.11f;
    float n3 = 8;
    std::cout << "Waiting, limited, p_rcp:\t" << AvgInWaitingAlgorithm(p3_2, 100000, n3, p3_2_rcp) << std::endl;
    std::cout << "Should be:\t\t" << (1 - std::pow(p3_2, n3)) / ((1 - p3_2) * (1 - p3_2_rcp)) << std::endl;*/
}

void two_three_a() {
    int count = 0;
    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.3 (a)");
    for (double p_rcp = 0.0; p_rcp <= 0.9; p_rcp += 0.3) {
        std::vector<double> p_vec;
        std::vector<double> n_vec_ex;
        std::vector<double> n_vec_th;
        for (double p = 0.05; p < 1; p += 0.05) {
            p_vec.push_back(p);
            n_vec_ex.push_back(AvgInWaitingAlgorithm(p, 100000, 0, p_rcp));
            n_vec_th.push_back(1 / ((1 - p) * (1 - p_rcp)));
        }
        std::string name = "p_rcp = " + std::to_string(p_rcp);
        plot(p_vec, n_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue + count), count++, name);
    }

    /*float p3_1 = 0.4f;
    float p3_1_rcp = 0.2f;
    std::cout << "Waiting, unlimited, p_rcp:\t" << AvgInWaitingAlgorithm(p3_1, 100000, 0, p3_1_rcp) << std::endl;
    std::cout << "Should be:\t\t" << 1 / ((1 - p3_1) * (1 - p3_1_rcp)) << std::endl;*/
}

void two_two() {
    int count = 0;
    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.2");
    for (int n = 5; n < 75; n += 10) {
        std::vector<double> p_vec;
        std::vector<double> n_vec_ex;
        std::vector<double> n_vec_th;
        for (double i = 0.05; i < 1; i += 0.05) {
            p_vec.push_back(i);
            n_vec_ex.push_back(AvgInWaitingAlgorithm(i, 100000, n));
            n_vec_th.push_back((1 - std::pow(i, n)) / (1 - i));
        }
        std::string name = "n = " + std::to_string(n);
        plot(p_vec, n_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue + count), count++, name);
    }

    /*float p2 = 0.8f;
    float n2 = 2;
    std::cout << "Waiting, limited:\t\t" << AvgInWaitingAlgorithm(p2, 100000, n2) << std::endl;
    std::cout << "Should be:\t\t" << (1 - std::pow(p2, n2)) / (1 - p2) << std::endl;*/
}

void two_one() {
    std::vector<double> p_vec;
    std::vector<double> n_vec_ex;
    std::vector<double> n_vec_th;
    auto firstPlot = new QCustomPlot();
    firstPlot->setWindowTitle("2.1");
    for (double i = 0.05; i < 1; i += 0.05) {
        p_vec.push_back(i);
        n_vec_ex.push_back(AvgInWaitingAlgorithm(i, 100000));
        n_vec_th.push_back(1 / (1 - i));
    }
    plot(p_vec, n_vec_ex, firstPlot, Qt::GlobalColor(Qt::blue), 0, "Experimental");
    plot(p_vec, n_vec_ex, firstPlot, Qt::GlobalColor(Qt::red), 1, "Theoretical");

    /*float p1 = 0.8f;
    std::cout << "Waiting, unlimited:\t\t" << AvgInWaitingAlgorithm(p1, 100000) << std::endl;
    std::cout << "Should be:\t\t" << 1 / (1 - p1) << std::endl;*/
}