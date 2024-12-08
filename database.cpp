#include "database.h"
#include "ui_database.h"
#include <QtSql>
using namespace std;
DataBase::DataBase(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataBase)
{
    ui->setupUi(this);
    disable_del_add_filt_sort_menu();
}
DataBase::~DataBase()
{
    delete ui;
    delete query;
    delete model;
}
void DataBase::on_open_DB_button_clicked()
{
    QString dbName = "";
    dbName = ui->input_db_name->text();
    m_db = QSqlDatabase::addDatabase("QSQLITE"); //соединение объекта базы данных с СУБД
    if (dbName == "") {
        m_db.setDatabaseName("myDB"); //определениеименибазыданных
    }
    else {
        m_db.setDatabaseName(dbName);
    }
    query = new QSqlQuery(m_db); // созданиеобъектадля запроса
    if(!m_db.open()) { // проверка на ошибку при открытии или создании базы данных
        throw "can't open database";
    }
    if(!m_db.tables().contains("historical_figures")) {
        query->clear(); //очистка запроса
        query->exec("CREATE TABLE historical_figures(id INTEGER PRIMARY KEY NOT NULL, name VARCHAR(25), birthday VARCHAR(20), deathday VARCHAR(20), contribution VARCHAR(60), rating INTEGER, position VARCHAR(25));"); //исполнение запроса на добавление записи
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(1, 'Iosif Stalin', '1878', '1953', 'President of Soviet Union', 3, 'politician');");
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(2, 'Isaak Newton', '1643', '1727', 'The Law of universe gravitation', 10, 'physicist');");
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(3, 'Steve Jobs', '1955', '2011', 'Apple', 9, 'designer');");
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(4, 'Mihail Kutuzov', '1745', '1835', 'Patriotic War of 1812', 8, 'comander');");
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(5, 'Albert Einstein', '1879', '1955', 'The theory of relativity', 10, 'physicist');");
        query->clear();
        query->exec("INSERT INTO historical_figures(id, name, birthday, deathday, contribution, rating, position) VALUES(6, 'Napoleon Bonapart', '1769', '1821', 'France Emperor', 4, 'commander');");
    }
    enable_del_add_filt_sort_menu();
    refresh_model();
}
void DataBase::on_table_comboBox_activated(int index)
{
    model = new QSqlTableModel(this, m_db); // создание редактируемой модели базы данных
    QString s = ui->table_comboBox->currentText();
    model->setTable(s); // созданиемодели таблицыPerson
    model->select(); // заполнение модели данными
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // выбор стратегии сохранения изменений в базе данных - сохранение происходит при переходе к другому полю
    ui->tableView->setModel(model); // соединение модели и ее табличного представления в форме
    if (s == "historical_figures") {
        enable_del_add_filt_sort_menu();
    } else {
        disable_del_add_filt_sort_menu();
    }
}
void DataBase::on_del_but_clicked()
{
    if (ui->del_id->text() == "") {
        qInfo() << "no ID";
        return;
    }
    query->clear();
    query->exec("DELETE FROM historical_figures WHERE id=" + ui->del_id->text());
    refresh_model();
}
void DataBase::on_add_but_clicked()
{
    if (ui->add_id->text() == "") {
        qInfo() << "no ID";
        return;
    }
    query->clear();
    QString bufL = "INSERT INTO historical_figures(id";
    QString bufR = " VALUES(" + ui->add_id->text();
    if (ui->add_name->text() != "") {
        bufL += ", name";
        bufR += ", '" + ui->add_name->text() + "'";
    }
    if (ui->add_birthday->text() != "") {
        bufL += ", birthday";
        bufR += ", '" + ui->add_birthday->text() + "'";
    }
    if (ui->add_deathday->text() != "") {
        bufL += ", deathday";
        bufR += ", '" + ui->add_deathday->text() + "'";
    }
    if (ui->add_contribution->text() != "") {
        bufL += ", contribution";
        bufR += ", '" + ui->add_contribution->text() + "'";
    }
    if (ui->add_rating->text() != "") {
        bufL += ", rating";
        bufR += ", " + ui->add_rating->text();
    }
    if (ui->add_position->text() != "") {
        bufL += ", position";
        bufR += ", '" + ui->add_position->text() + "'";
    }
    bufL += ")";
    bufR += ");";
    qInfo() << "buf = " + bufL + bufR;
    query->exec(bufL + bufR);
    refresh_model();
}
void DataBase::on_filt_ratingl5_clicked()
{
    model->setFilter("rating<5");
    model->select();
    ui->tableView->setModel(model);
}
void DataBase::on_filt_ratingm5_clicked()
{
    model->setFilter("rating>5");
    model->select();
    ui->tableView->setModel(model);
}
void DataBase::on_filt_stop_clicked()
{
    refresh_model();
}
void DataBase::on_sort_ASC_clicked()
{
    QString s = ui->sort_comboBox->currentText();
    int n = 0;
    if (s == "имя") {//
        n = 1;
    } else if (s == "год рождения") {
        n = 2;
    } else if (s == "год смерти") {
        n = 3;
    } else if (s == "достижение") {
        n = 4;
    } else if (s == "рейтинг") {
        n = 5;
    } else if (s == "должность") {
        n = 6;
    }
    model->setSort(n, Qt::SortOrder::AscendingOrder);
    model->select();
    ui->tableView->setModel(model);
}
void DataBase::on_sort_DESC_clicked()
{
    QString s = ui->sort_comboBox->currentText();
    int n = 0;
    if (s == "имя") {//
        n = 1;
    } else if (s == "год рождения") {
        n = 2;
    } else if (s == "год смерти") {
        n = 3;
    } else if (s == "достижение") {
        n = 4;
    } else if (s == "рейтинг") {
        n = 5;
    } else if (s == "должность") {
        n = 6;
    }
    model->setSort(n, Qt::SortOrder::DescendingOrder);
    model->select();
    ui->tableView->setModel(model);
}
void DataBase::on_sort_stop_clicked()
{
    refresh_model();
}
void DataBase::refresh_model()
{
    model = new QSqlTableModel(this, m_db); // создание редактируемой модели базы данных
    model->setTable("historical_figures");
    model->select(); // заполнение модели данными
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // выбор стратегии сохранения изменений в базе данных-сохранение происходит при переходе к другому полю
    ui->tableView->setModel(model); // соединение модели и ее табличного представления вформе
    for (int i = ui->table_comboBox->count(); i > 0; i--) {
        ui->table_comboBox->removeItem(i-1);
    }
    ui->table_comboBox->insertItems(0, m_db.tables());
    ui->table_comboBox->setCurrentIndex(0);
}
void DataBase::disable_del_add_filt_sort_menu()
{
    ui->del->setDisabled(true);
    ui->del_label->setDisabled(true);
    ui->del_id->setDisabled(true);
    ui->del_but->setDisabled(true);
    ui->add->setDisabled(true);
    ui->add_label0->setDisabled(true);
    ui->add_label1->setDisabled(true);
    ui->add_label2->setDisabled(true);
    ui->add_label3->setDisabled(true);
    ui->add_label4->setDisabled(true);
    ui->add_label5->setDisabled(true);
    ui->add_label6->setDisabled(true);
    ui->add_id->setDisabled(true);
    ui->add_name->setDisabled(true);
    ui->add_birthday->setDisabled(true);
    ui->add_deathday->setDisabled(true);
    ui->add_contribution->setDisabled(true);
    ui->add_rating->setDisabled(true);
    ui->add_position->setDisabled(true);
    ui->add_but->setDisabled(true);
    ui->filt->setDisabled(true);
    ui->filt_ratingl5->setDisabled(true);
    ui->filt_ratingm5->setDisabled(true);
    ui->filt_stop->setDisabled(true);
    ui->sort->setDisabled(true);
    ui->sort_comboBox->setDisabled(true);
    ui->sort_ASC->setDisabled(true);
    ui->sort_DESC->setDisabled(true);
    ui->sort_stop->setDisabled(true);
}
void DataBase::enable_del_add_filt_sort_menu()
{
    ui->del->setDisabled(false);
    ui->del_label->setDisabled(false);
    ui->del_id->setDisabled(false);
    ui->del_but->setDisabled(false);
    ui->add->setDisabled(false);
    ui->add_label0->setDisabled(false);
    ui->add_label1->setDisabled(false);
    ui->add_label2->setDisabled(false);
    ui->add_label3->setDisabled(false);
    ui->add_label4->setDisabled(false);
    ui->add_label5->setDisabled(false);
    ui->add_label6->setDisabled(false);
    ui->add_id->setDisabled(false);
    ui->add_name->setDisabled(false);
    ui->add_birthday->setDisabled(false);
    ui->add_deathday->setDisabled(false);
    ui->add_contribution->setDisabled(false);
    ui->add_rating->setDisabled(false);
    ui->add_position->setDisabled(false);
    ui->add_but->setDisabled(false);
    ui->filt->setDisabled(false);
    ui->filt_ratingl5->setDisabled(false);
    ui->filt_ratingm5->setDisabled(false);
    ui->filt_stop->setDisabled(false);
    ui->sort->setDisabled(false);
    ui->sort_comboBox->setDisabled(false);
    ui->sort_ASC->setDisabled(false);
    ui->sort_DESC->setDisabled(false);
    ui->sort_stop->setDisabled(false);
}