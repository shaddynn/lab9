 #ifndef DATABASE_H
 #define DATABASE_H
 #include <QMainWindow>
 #include <QtSql>
 QT_BEGIN_NAMESPACE
 namespace Ui { class DataBase; }
 QT_END_NAMESPACE
 class DataBase : public QMainWindow
 {
 Q_OBJECT
 public:
 DataBase(QWidget *parent = nullptr);
 ~DataBase();
 private slots:
 void on_open_DB_button_clicked();
 void on_table_comboBox_activated(int index);
 void on_del_but_clicked();
 void on_add_but_clicked();
 void on_filt_ratingl5_clicked();
 void on_filt_ratingm5_clicked();
 void on_filt_stop_clicked();
 void on_sort_ASC_clicked();
 void on_sort_DESC_clicked();
 void on_sort_stop_clicked();
 void refresh_model();
 void disable_del_add_filt_sort_menu();
 void enable_del_add_filt_sort_menu();
 private:
 Ui::DataBase *ui;
 QSqlDatabase m_db;//объект базы данных
 QSqlQuery* query; //указатель на запрос
 QSqlTableModel* model; // указатель на таблицу данных в приложении
 };
 #endif // DATABASE_H