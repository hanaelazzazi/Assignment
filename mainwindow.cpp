#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib> //for rand and srand functions
#include <QtDebug> //for qDebug
#include <ctime> //for the time function
#include <QVector> //for Qvector
#include <chrono> //for chrono function



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //to connect clicked signals of the buttons to their slots
    connect(ui->Generate, &QPushButton::clicked, this, &MainWindow::on_Generate_clicked);
    connect(ui->Normal, &QPushButton::clicked, this, &MainWindow::on_Normal_clicked);
    connect(ui->Binary, &QPushButton::clicked, this, &MainWindow::on_Binary_clicked);
    connect(ui->MergeSort, &QPushButton::clicked, this, &MainWindow::on_MergeSort_clicked);
    connect(ui->STLSort, &QPushButton::clicked, this, &MainWindow::on_STLSort_clicked);

    //initalize min. and max. numbers from rand array
    min_value=0;
    max_value = 100;

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Binary_clicked()
{
    ui->search->setText("Using: Binary Search...");
}

void MainWindow::on_Normal_clicked()
{
    ui->search->setText("Using: Normal Search...");
}

void merge(QVector<int> &array, int left, int middle, int right) {
    //merge two sorted subarrays into a single sorted array
    QVector<int> leftArray(middle - left + 1);
    QVector<int> rightArray(right - middle);

    //copy left subarray into the left array vector
    for (int i = 0; i < leftArray.size(); i++) {
        leftArray[i] = array[left + i];
    }
    //copy write subarray into the right array vector
    for (int i = 0; i < rightArray.size(); i++) {
        rightArray[i] = array[middle + i + 1];
    }

    int i = 0;
    int j = 0;
    int k = left;

    //merge the left and right subarrays into a single sorted array
    while (i < leftArray.size() && j < rightArray.size()) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }

        k++;
    }
    //copy the main elemnts from the left subarray to the main array
    while (i < leftArray.size()) {
        array[k] = leftArray[i];
        i++;
        k++;
    }
    //copy the main elemnts from the right subarray to the main array
    while (j < rightArray.size()) {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(QVector<int>& array, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        mergeSort(array, left, middle); //recursive function to sort the left array
        mergeSort(array, middle + 1, right); //recursive function to sort the right array
        merge(array, left, middle, right); //merge the two sorted subarrays
    }
}

void MainWindow::on_MergeSort_clicked() {
    ui->sort->setText("Sorting Using Merge Sort");

    ui->sortmsg->setText("Array is Sorted");

    //start the time
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    mergeSort(m_array, 0, m_array.size() - 1);

    //end the timer
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();

    //calculate the elapsed time
    std::chrono::nanoseconds elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    //update the sortTime member variable
    this->sortTime = elapsed_time.count();

    //display the sorted array
    displayArrayInListWidget(m_array);
}


void MainWindow::on_STLSort_clicked()
{
    ui->sort->setText("Sorting Using STL Sort");

    ui->sortmsg->setText("Array is Sorted");

    auto start_time = std::chrono::steady_clock::now();

    std::sort(m_array.begin(), m_array.end()); //sort the array using STL sort

    auto end_time = std::chrono::steady_clock::now();

    displayArrayInListWidget(m_array);

    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    qDebug() << "STL sort time: " << elapsed_time << " nanoseconds";

    this->sortTime = elapsed_time;
}

void MainWindow::generateRandomArray(int size, int min_value, int max_value)
{
    m_array.resize(size);

    for (int i = 0; i < size; i++)
    {
        m_array[i] = rand() % ((max_value + 1) - min_value) + min_value; //generate a random number between min_value and max_value and add it to the array
    }
    qDebug() << "Generated array of size" << size << ": " << m_array;

}

void MainWindow::on_size_cursorPositionChanged()
{

    ui->sizemsg->setText(ui->size->text());

}

int randInt(int low, int high)
{
    return rand() % ((high + 1) - low) + low; //generate a random integer between low and high
}

void MainWindow::displayArrayInListWidget(const QVector<int>& array) {
    ui->listWidget->clear(); //clear the QListWidget

    for (int i = 0; i < array.size(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QString::number(array[i]));
        ui->listWidget->addItem(item); //add the item to the QListWidget
    }
}

void MainWindow::on_Generate_clicked()
{
    //get size of array from UI
    int size = ui->size->text().toInt();

    ui->sizemsg->setText(QString("%1").arg(size));

    generateRandomArray(size, min_value, max_value);

    displayArrayInListWidget(m_array);
}

void MainWindow::on_FindIt_clicked()
{
    int numToFind = ui->findmsg->text().toInt();

    //get array from UI list widget
    QVector<int> array(ui->listWidget->count());
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem* item = ui->listWidget->item(i);
        array[i] = item->text().toInt();
    }

    clock_t sortStart = clock();

    std::sort(array.begin(), array.end());

    clock_t sortEnd = clock();
    double sortTimeElapsed = double(sortEnd - sortStart) / CLOCKS_PER_SEC;

    //perform search on array to find number
    int index = -1;
    clock_t searchStart = clock();
    int left = 0;
    int right = array.size() - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (array[mid] == numToFind) {
            index = mid;
            break;
        } else if (array[mid] < numToFind) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    //perfom normal search on array to find number
    clock_t searchEnd = clock();

    double searchTimeElapsed = double(searchEnd - searchStart) / CLOCKS_PER_SEC;
    //display search result in pop up message box
    QString message;
    if (index == -1) {
        message = QString("Number %1 not found in the array").arg(numToFind);
    } else {
        message = QString("Number %1 found at index %2 in the array").arg(numToFind).arg(index);
    }
    message += QString("\n\nSorting Time was %1 nanoseconds\nSearch Time was %2 nanoseconds").arg(sortTimeElapsed*1e9).arg(searchTimeElapsed*1e9);
    QMessageBox::information(this, "Search Result", message);
}

void MainWindow::on_quick_sort_clicked()
{
    ui->sort->setText("Sorting Using Quick Sort");
    int size = ui->listWidget->count();
    QList<int> items;
    for(int i=0; i<size; i++){
        items.append(ui->listWidget->item(i)->text().toInt());
    }
    //quiksort algorithm
    quickSort(items, 0, size-1);
    ui->listWidget->clear();
    for(int i=0; i<size; i++){
        ui->listWidget->addItem(QString::number(items[i]));
    }
}

void MainWindow::on_bubble_sort_clicked()
{
    ui->sort->setText("Sorting Using Bubble Sort");
    int size = ui->listWidget->count();
    QList<int> items;
    ui->sortmsg->setText("Array is Sorted");
    for(int i=0; i<size; i++){
        items.append(ui->listWidget->item(i)->text().toInt());
    }
    //bubble sort algorthim
    bubbleSort(items, size);
    ui->listWidget->clear();
    for(int i=0; i<size; i++){
        ui->listWidget->addItem(QString::number(items[i]));
    }
}

//quicksort implement
void MainWindow::quickSort(QList<int>& items, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(items, low, high);
        quickSort(items, low, pivotIndex - 1);
        quickSort(items, pivotIndex + 1, high);
    }
}
//part of quicksort implementation
int MainWindow::partition(QList<int>& items, int low, int high)
{
    int pivot = items[high];
    int i = low - 1;
    ui->sortmsg->setText("Array is Sorted");
    for (int j = low; j <= high - 1; j++)
    {
        if (items[j] < pivot)
        {
            i++;
            std::swap(items[i], items[j]);
        }
    }
    std::swap(items[i + 1], items[high]);
    return (i + 1);
}
//bubble sort implementation
void MainWindow::bubbleSort(QList<int>& items, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (items[j] > items[j + 1])
            {
                std::swap(items[j], items[j + 1]);
            }
        }
    }
}


void MainWindow::on_clear_clicked()
{
    ui->sort->clear();
    ui->size->clear();
    ui->search->clear();
    ui->listWidget->clear();
    ui->sizemsg->clear();
    ui->arraySortmsg->clear();
    ui->findmsg->clear();
    ui->sortmsg->setText("Unsorted Array");
}

