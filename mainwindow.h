#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <cstdlib>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_size_cursorPositionChanged();

    int randInt(int low, int high);

    void generateRandomArray(int size, int min_value, int max_value);

    void displayArrayInListWidget(const QVector<int>& array);

    void merge(QVector<int> &array, int left, int middle, int right);

    void quickSort(QList<int>& items, int low, int high);

    void bubbleSort(QList<int>& items, int size);

    int partition(QList<int>& items, int low, int high);

private slots:
    void on_FindIt_clicked();

    void on_quick_sort_clicked();

    void on_bubble_sort_clicked();

    void on_clear_clicked();

    void on_Generate_clicked();

    void on_Binary_clicked();

    void on_Normal_clicked();

    void on_MergeSort_clicked();

    void on_STLSort_clicked();

private:
    Ui::MainWindow *ui;
    int ARRAY_SIZE;
    int min_value;
    int max_value;
    QVector<int> m_array;
    qint64 sortTime;

};


inline int MainWindow::randInt(int low, int high)
{
    return rand() % ((high + 1) - low) + low;
}

#endif // MAINWINDOW_H
