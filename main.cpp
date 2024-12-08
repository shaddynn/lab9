 #include database.h
 #include QApplication
 int main(int argc, char argv[])
 {
 QApplication a(argc, argv);
 DataBase w;
 w.show();
 return a.exec();
 }