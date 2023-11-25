#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "mainprogram.hh"

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update_view();
    void output_text(std::ostringstream &output);
    void output_text_end();

    bool check_stop_pressed() const;

public slots:
    void execute_line();
    void cmd_selected(int idx);
    void number_selected(const QString &number);
    void select_file();
    void clear_input_line();
    void fit_view();
    void scene_selection_change();
    void clear_selection();

private:
    Ui::MainWindow *ui = nullptr;

    QGraphicsScene* gscene_ = nullptr;

    MainProgram mainprg_;

    bool stop_pressed_ = false;

    bool selection_clear_in_progress = false;
};

#endif // MAINWINDOW_HH
